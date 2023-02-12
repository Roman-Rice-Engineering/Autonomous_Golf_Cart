#ifndef ENCODER_H
#define ENCODER_H

/*
  This code module is designed for esp8266 based
  boards, it includes code to interface with a
  single encoder.

  Encoder position is tracked automatically from the moment that encoder_setup() is called!


  encoder_setup(uint8_t encoderPin0, uint8_t encoderPin1)
    --- Attach interrupt and declare what two pins are used

  encoder_position_get()
    --- Returns current position count of encoder
  
  encoder_position_reset()
    --- Resets encoder position count to 0
*/

#include <Arduino.h>

static volatile ssize_t encoder_position;
static uint8_t encoder_compare_pin;

static void ICACHE_RAM_ATTR encoder_isr() {
  if (digitalRead(encoder_compare_pin) == LOW) {
    encoder_position++;
  } else {
    encoder_position--;
  }
}

void encoder_setup(uint8_t encoderPin0, uint8_t encoderPin1) {
  pinMode(encoderPin0, INPUT);
  pinMode(encoderPin1, INPUT);

  encoder_compare_pin = encoderPin1;

  attachInterrupt(encoderPin0, encoder_isr, RISING);
}

void encoder_position_reset() {
  encoder_position = 0;
}

ssize_t encoder_position_get() {
  return encoder_position;
}

#endif