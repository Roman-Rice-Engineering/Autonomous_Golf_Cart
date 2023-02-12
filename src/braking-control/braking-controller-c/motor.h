#ifndef MOTOR_H
#define MOTOR_H

/*
  This code module is designed for esp8266 based
  boards, it includes code to control the drive
  of a single motor.
  
  motor_setup(uint8_t motorPin0, uint8_t motorPin1)
    --- Set up pins for motor

  motor_drive(uint8_t driveDirection)
    --- Drive motor, input takes a number as direction
    --- Defined directions: MOTOR_OFF, MOTOR_CW, MOTOR_CCW
  
*/

#include <Arduino.h>

#define MOTOR_OFF 0
#define MOTOR_CW 1
#define MOTOR_CCW 2

static uint8_t motor_pin_0;
static uint8_t motor_pin_1;

void motor_setup(uint8_t motorPin0, uint8_t motorPin1) {
  pinMode(motorPin0, OUTPUT);
  motor_pin_0 = motorPin0;
  pinMode(motorPin1, OUTPUT);
  motor_pin_1 = motorPin1;
}

void motor_drive(uint8_t driveDirection) {
  if (driveDirection == MOTOR_OFF) {
    digitalWrite(motor_pin_0, LOW);
    digitalWrite(motor_pin_1, LOW);
  } else if (driveDirection == MOTOR_CW) {
    digitalWrite(motor_pin_0, HIGH);
    digitalWrite(motor_pin_1, LOW);
  } else if (driveDirection == MOTOR_CCW) {
    digitalWrite(motor_pin_0, LOW);
    digitalWrite(motor_pin_1, HIGH);
  }
}

#endif