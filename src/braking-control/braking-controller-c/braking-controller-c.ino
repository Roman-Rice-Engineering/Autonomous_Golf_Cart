/*
  This code module is designed for esp8266 based
  boards, it controls the position of a dc motor 
  and allows for the motor to home until a limit
  switch at the press of a button.
*/

#include "encoder.h"
#include "motor.h"
#include "serialcontrol.h"

#define MOTOR_MOTION_LIMIT 10000

/* Define digital pin connections */
#define HOME_BUTTON 5

#define ENCODER_PIN_0 13
#define ENCODER_PIN_1 12

#define LIMIT_SWITCH 2

#define MOTOR_PIN_0 14
#define MOTOR_PIN_1 16

const ssize_t motor_motion_limit = MOTOR_MOTION_LIMIT;
ssize_t requested_encoder_position = 0;

void home_motor() {
  while (digitalRead(LIMIT_SWITCH) == 1) {
    motor_drive(MOTOR_CCW);
    ESP.wdtFeed();
  }
  encoder_position_reset();
  requested_encoder_position = 0;
}

void setup() {
  motor_setup(MOTOR_PIN_0, MOTOR_PIN_1);
  encoder_setup(ENCODER_PIN_0, ENCODER_PIN_1);
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(HOME_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  /* Drive motor to get requested position and real position to be near equal */
  if (encoder_position_get() < requested_encoder_position - 10) {
    motor_drive(MOTOR_CW);
  } else if (encoder_position_get() > requested_encoder_position + 10) {
    motor_drive(MOTOR_CCW);
  } else {
    motor_drive(MOTOR_OFF);
  }

  /* Print debug without delaying loop*/
  print_integer_slow(encoder_position_get(), 500);

  /* Handle setting motor position target */
  set_requested_position(&requested_encoder_position);
  if (requested_encoder_position < 0) {
    requested_encoder_position = 0;
  }
  if (requested_encoder_position > motor_motion_limit) {
    requested_encoder_position = motor_motion_limit;
  }

  /* Check if homing button was pressed */
  if (digitalRead(HOME_BUTTON) == 0) {
    home_motor();
  }
}
