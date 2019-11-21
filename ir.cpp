// @license ir.cpp
// MIT License
// (c) 2018 miyakelp


#include <Arduino.h>
#include "IR.hpp"


ESP8266DashIR::IRLED::IRLED(int ledPin, int receiverPin):
  ledPin(ledPin),
  receiverPin(receiverPin)
{
  pinMode(ledPin, OUTPUT);
  pinMode(receiverPin, INPUT);
  digitalWrite(ledPin, LOW);
}


int ESP8266DashIR::IRLED::readPattern(unsigned int *data, int bufferSize) {
  unsigned int startTime = 0;
  unsigned int currentTime = 0;
  unsigned int receivedVal = 0;
  int bufferIndex = 0;
  
  while(digitalRead(receiverPin) == 1) {
    delay(0);
  }

  startTime = micros();
  currentTime = startTime;
  while(currentTime - startTime < IR_RECEIVER_TIMEOUT) {
    currentTime = micros();
    if (digitalRead(receiverPin) != receivedVal) {
      data[bufferIndex++] = currentTime - startTime;
      startTime = currentTime;
      receivedVal = (~receivedVal) & 0x01;

      if (bufferIndex >= bufferSize) {
        return -1;
      }
    }
  }

  return bufferIndex;
}


void ESP8266DashIR::IRLED::sendPattern(const unsigned int *pattern, const unsigned int patternLength) {
  unsigned int startTime = 0;
  int pinState = LOW;

  for (int patternIndex = 0; patternIndex < patternLength; patternIndex++) {
    startTime = micros();
    while (micros() - startTime < pattern[patternIndex]) {
      digitalWrite(ledPin, patternIndex & 0x01 ? LOW : HIGH);
      delayMicroseconds(9);
      digitalWrite(ledPin, LOW);
      delayMicroseconds(13);
    }
  }
  digitalWrite(ledPin, LOW);
}


