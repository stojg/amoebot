/* 
 * File:   scanner.h
 * Author: Stig Lindqvist
 *
 */

#ifndef DISTANCE_SCANNER_H
#define	DISTANCE_SCANNER_H
#define DISTANCE_SCANNER_STEPS 6
#define DISTANCE_SCANNER_MIN 20
#define DISTANCE_SCANNER_MAX 160
#define DISTANCE_SCANNER_INCREMENTS 28
#define DISTANCE_SCANNER_SPEED 2
#include <Servo.h>
#include "ir.h"
#include "moving_average.h"

class DistanceScanner {
public:

	/**
	 * Constructor
	 * 
     * @param servo
     * @param irSensor
     */
	DistanceScanner(Servo &servo, IR &irSensor) :
	irSensor(irSensor),
	myservo(servo) {
		this->startTime = millis();
		this->sweepUp = false;
		this->state = moving;
		this->currentPosition = 0;
		this->currentDegree = DISTANCE_SCANNER_MIN;
	}

	/**
	 * Move the servo to a position
	 * 
     * @param degree
     */
	void moveTo(int degree) {
		int step = (degree - DISTANCE_SCANNER_MIN) / DISTANCE_SCANNER_INCREMENTS;
		this->currentPosition = constrain(step, 0, DISTANCE_SCANNER_STEPS - 1);
		myservo.write(degree);
	}

	/**
	 * Progress the sweep and scan 
     */
	void run() {
		unsigned long currentTime = millis();
		unsigned long elapsedTime = currentTime - this->startTime;
		switch (this->state) {
			// wait to stabilize analog reading
			case idle:
				if (elapsedTime >= 10) {
					this->startTime = currentTime;
					this->state = scanning;
				}
				break;
			// Read distance
			case scanning:
				this->rangeData[this->currentPosition].add(irSensor.getDistance());
				this->startTime = currentTime;
				this->state = moving;
				break;
			// Move the servo to position
			case moving:
				// Wait until it have finished moving
				if (elapsedTime >= DISTANCE_SCANNER_INCREMENTS * DISTANCE_SCANNER_SPEED) {
					this->sweep();
					this->state = idle;
					this->startTime = currentTime;
				}
				break;
		}
		return;
	}

	/**
	 * Sweep the servo
	 */
	void sweep() {

		if (this->currentDegree >= DISTANCE_SCANNER_MAX) {
			this->currentDegree = DISTANCE_SCANNER_MAX;
			this->sweepUp = false;
		}
		if (this->currentDegree <= DISTANCE_SCANNER_MIN) {
			this->currentDegree = DISTANCE_SCANNER_MIN;
			this->sweepUp = true;
		}

		if (this->sweepUp) {
			this->currentDegree = this->currentDegree + DISTANCE_SCANNER_INCREMENTS;
		} else {
			this->currentDegree = this->currentDegree - DISTANCE_SCANNER_INCREMENTS;
		}
		this->moveTo(this->currentDegree);
	}

	/**
	 * Get an array that consists of the closest object and the heading of it
	 * 
     * @return 
     */
	int* getClosestObject() {
		
		int closest[2] = {1000, 90};
		for (int i = 0; i < DISTANCE_SCANNER_STEPS - 1; i++) {
			if (this->rangeData[i] != 0 && this->rangeData[i] < closest[0]) {
				// distance
				closest[0] = this->rangeData[i].get();
				// degree
				closest[1] = DISTANCE_SCANNER_MIN + i * DISTANCE_SCANNER_INCREMENTS;
			}
		}
		return closest;
	}

private:
	Servo myservo;
	IR irSensor;
	
	unsigned long startTime;
	bool sweepUp;
	
	int currentDegree;
	int currentPosition;

	enum state_type {
		scanning, moving, idle
	};
	state_type state;
	MovingAverage<unsigned int, 2> rangeData[DISTANCE_SCANNER_STEPS];
	
};

#endif /* DISTANCE_SCANNER_H */
