#include <ffft.h>

#ifndef MIC_H
#define MIC_H

class Mic {
   public:
  
  Mic(int pin): micPin(pin) {
    minValue = 1024;
    maxValue = 0;
    sampleSpan = 10;
    timer = millis();
    pinMode(pin, INPUT_PULLUP);
  }
  
  int getVolume() {
    currentValue = analogRead(micPin);

    if (currentValue < minValue) {
        minValue = currentValue;
    } 
    if (currentValue > maxValue) {
        maxValue = currentValue;
    }

    if (millis() - timer >= sampleSpan) {
        volume = maxValue - minValue;
        resetValues();
    }
    
    return volume;
  }
  
  private:
    int micPin;
    int minValue;
    int maxValue;
    int volume;
    unsigned long timer;
    int sampleSpan;
    int currentValue;
    
    void resetValues() {
      maxValue = 0;
      minValue = 1024;
      timer = millis(); 
  }
  
};

#endif

