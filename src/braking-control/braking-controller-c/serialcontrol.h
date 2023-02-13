#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <Arduino.h>

static size_t old_time_millis = 0;
void print_integer_slow(int integerInput, size_t delayMilliseconds) {
  size_t current_time = millis();
  if (current_time > old_time_millis + delayMilliseconds) {
    Serial.println(integerInput);
    old_time_millis = current_time;
  }
}

void set_requested_position(ssize_t *requestedPositionVariable) {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    *requestedPositionVariable = input.toInt();
  }
}

#endif