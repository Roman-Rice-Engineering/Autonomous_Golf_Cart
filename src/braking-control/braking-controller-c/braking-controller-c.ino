/*
  This code module is designed for esp8266 based
  boards, it controls the position of a dc motor 
  and allows for the motor to home until a limit
  switch at the press of a button.
*/


#include <stdint.h>


/* Define input encoder sensitivity */
#define INPUT_ENCODER_SENSITIVITY 1000


/* Define digital pin connections */
#define HOME_BUTTON 5

#define MOTOR_ENCODER_1 13
#define MOTOR_ENCODER_2 12

#define LIMIT_SWITCH 2

#define MOTOR_DRIVE_1 14
#define MOTOR_DRIVE_2 16


const ssize_t motor_motion_limit = 10000;
volatile ssize_t encoder_position = 0;
volatile ssize_t requested_encoder_position = 0;


void ICACHE_RAM_ATTR motor_encoder_isr() {
  if (digitalRead(MOTOR_ENCODER_2) == LOW) {
    encoder_position++;
  } else {
    encoder_position--;
  }
}


#define OFF 0
#define CW 1
#define CCW 2
void drive_motor(int setting) {
  if (setting == OFF) {
    digitalWrite(MOTOR_DRIVE_1, LOW);
    digitalWrite(MOTOR_DRIVE_2, LOW);
  } else if (setting == CW) {
    digitalWrite(MOTOR_DRIVE_1, HIGH);
    digitalWrite(MOTOR_DRIVE_2, LOW);
  } else if (setting == CCW) {
    digitalWrite(MOTOR_DRIVE_1, LOW);
    digitalWrite(MOTOR_DRIVE_2, HIGH);
  }
}


void home_motor() {
  drive_motor(CCW);
  while (digitalRead(LIMIT_SWITCH) == 1) {
    encoder_position = 0;
    requested_encoder_position = 0;
    ESP.wdtFeed();
  }
}


void setup() {
  pinMode(MOTOR_DRIVE_1, OUTPUT);
  pinMode(MOTOR_DRIVE_2, OUTPUT);
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(HOME_BUTTON, INPUT_PULLUP);
  pinMode(MOTOR_ENCODER_1, INPUT);
  pinMode(MOTOR_ENCODER_2, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOTOR_ENCODER_1), motor_encoder_isr, RISING);
  Serial.begin(9600);
}


long oldtime = 0;
void loop() {
  /* Drive motor to get requested position and real position to be near equal */
  if (encoder_position < requested_encoder_position - 10) {
    drive_motor(CW);
  } else if (encoder_position > requested_encoder_position + 10) {
    drive_motor(CCW);
  } else {
    drive_motor(OFF);
  }

  /* Print debug without delaying loop*/
  if (oldtime < millis() - 100) {
    oldtime = millis();
    Serial.println(encoder_position);
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      if (input != "") {
        requested_encoder_position = input.toInt();
        if(requested_encoder_position < 0){
          requested_encoder_position = 0;
        }else if(requested_encoder_position > motor_motion_limit){
          requested_encoder_position = motor_motion_limit;
        }
      }
    }

    /* Check if homing button was pressed */
    if (digitalRead(HOME_BUTTON) == 0) {
      home_motor();
    }
  }
}
