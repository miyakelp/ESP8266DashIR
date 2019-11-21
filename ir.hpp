// @license ir.hpp
// MIT License
// (c) 2018 miyakelp


#ifndef _IR
#define _IR

#define IR_RECEIVER_TIMEOUT 50000 // us

namespace ESP8266DashIR
{
  class IRLED
  {
  public:
    IRLED(const int ledPin, const int receiverPin);
    int readPattern(unsigned int *buffer, int bufferSize);
    void sendPattern(const unsigned int *pattern, const unsigned int patternLength);

  private:
    const int ledPin;
    const int receiverPin;
  };
}

#endif

