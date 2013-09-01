/* 
 * File:   scanner.h
 * Author: stig
 *
 * Created on August 31, 2013, 1:30 PM
 */

#ifndef DISTANCE_SCANNER_H
#define	DISTANCE_SCANNER_H
#define DISTANCE_SCANNER_STEPS 11
#define DISTANCE_SCANNER_MIN 20
#define DISTANCE_SCANNER_MAX 160
#define DISTANCE_SCANNER_INCREMENTS 14

#include <Servo.h>
#include "ir.h"

class DistanceScanner {
public:
	
	DistanceScanner(Servo &servo, IR &irSensor) :
	irSensor(irSensor),
	myservo(servo) {
		this->startTime = millis();
		this->scanToRight = false;
		this->state = moving;
		this->position = 0;
		this->scanDegree = DISTANCE_SCANNER_MIN;
		this->increments = DISTANCE_SCANNER_INCREMENTS;
	}
	
	void moveTo(int degree) {
		int step = (degree-DISTANCE_SCANNER_MIN)/this->increments;
		this->position = constrain(step, 0, DISTANCE_SCANNER_STEPS-1);
		myservo.write(degree);
	}
	
	void run() {
		unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - this->startTime;
		int scanStep;
		switch (this->state) {
			// wait to stabilize analog reading
			case idle:
				if(elapsedTime >= 50) {
					this->startTime = currentTime;
					this->state = scanning;
				}
				break;
			// Read distance
			case scanning:
					this->measurePoints[this->position] = irSensor.getDistance();
					this->startTime = currentTime;
					this->state = moving;
				break;
			// Move the servo to position
			case moving:
				// Wait until it have moved
				if(elapsedTime >= this->increments*this->servoSpeed) {
					this->updateServo();
					this->state = idle;
					this->startTime = currentTime;
				}
				break;
        }
		
		return;
	}
	
	/**
	 * 100ms per 60 deg
	 * 
	 * 1.6666... ms per deg
     */
	void updateServo() {
		
		if (this->scanDegree >= DISTANCE_SCANNER_MAX) {
			this->scanDegree = DISTANCE_SCANNER_MAX;
			this->scanToRight = false;
		}
		if (this->scanDegree <= DISTANCE_SCANNER_MIN) {
			this->scanDegree = DISTANCE_SCANNER_MIN;
			this->scanToRight = true;
		}
		
		if (this->scanToRight) {
			this->scanDegree = this->scanDegree + (int)this->increments;
		} else {
			this->scanDegree = this->scanDegree - (int)this->increments;
		}
		//Serial.println(this->scanDegree);
		this->moveTo(this->scanDegree);
	}
	
	int* getClosestObject() {
		int closest[2] = {1000, 90};
		for(int i=0;i<DISTANCE_SCANNER_STEPS-1;i++) {
	        if(this->measurePoints[i] != 0 && this->measurePoints[i]<closest[0]) {
				// distance
				closest[0] = this->measurePoints[i];
				// degree
				closest[1] = DISTANCE_SCANNER_MIN + i * this->increments;
			}
		}
		return closest;
	}
	
private:
	unsigned long startTime;
	bool scanToRight;
	int scanDegree;
	Servo myservo;
	IR irSensor;
	enum state_t { scanning, moving, idle };
    state_t state;
	int increments;
	static float const servoSpeed = 1.7;
	float measurePoints[DISTANCE_SCANNER_STEPS]; 
	int position;
};

#endif	/* SCANNER_H */
