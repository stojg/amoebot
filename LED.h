/*
||
|| @file LED.h
|| @version 1.1
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making/using LEDs
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#ifndef LED_H
#define LED_H
#include "Arduino.h"

class LED{
  public:
    LED(int ledPin);
	bool getState();
    void on();
	void off();
	void toggle();
	void blink(unsigned int time, int times=1);
	void setValue(int val);
        void blinkNoBlock(unsigned int time);
        void updateBlinkNoBlock();
	void fadeIn(unsigned int time);
	void fadeOut(unsigned int time);
  private:
	bool status;
	int pin;
	unsigned long blinkNoBlock_millis;
	unsigned int blinkNoBlock_time;
};

#endif
