#ifndef ROBOT_H
#define ROBOT_H
#include "IR.h"
#include "moving_average.h"
#include "motor.h"
#include "distance_scanner.h"
#include <Servo.h>

class Robot {
public:

	/*
	 * @brief Class constructor.
	 */
	Robot(Motor &lfMotor, Motor &lbMotor, Motor &rfMotor, Motor &rbMotor, DistanceScanner &scanner) :
	LeftFrontMotor(lfMotor),
	LeftBackMotor(lbMotor),
	RightFrontMotor(rfMotor),
	RightBackMotor(rbMotor),
	IRScanner(scanner) {

	}

	/**
	 * Initialize the robot
	 */
	void initialize() {
		IRScanner.moveTo(20);
		delay(200);
		this->robotState = r_scanning;
		this->timer = millis();

	}

	void stop() {
		this->setSpeedRight(0);
		this->setSpeedLeft(0);
	}

	void run() {
		unsigned long currentTime = millis();
		unsigned long elapsedTime = currentTime - this->timer;
		int *closest;

		int distance = closest[0];
		int direction = closest[1];
		
		if(!digitalRead(13)) {
			this->robotState = r_idle;
		}
		
		switch (this->robotState) {
			case r_idle:
				this->stop();
				Serial.println("idle");
				if(digitalRead(13)) {
					this->robotState = r_scan;
					this->timer = millis();
				}
				break;

			case r_avoid:
				distance = IRScanner.getCurrentDistance();
				direction = IRScanner.getCurrentDegree(); 
				Serial.print("avoid ");
				Serial.print(distance);
				Serial.print("cm ");
				Serial.println(direction);
				if(distance > 30) {
					this->timer = millis();
					this->robotState = r_scan;
				} else if(distance < 25) {
					if(direction < 90) {
						this->backLeft();
					} else {
						this->backRight();
						
					}
				} else {
					if(direction < 90) {
						this->turnLeft();
					} else {
						this->turnRight();
					}
				}
				break;
				
			case r_scan:
				this->stop();
				IRScanner.sweepAndMeasure();
				closest = IRScanner.getClosestObject();
				if(elapsedTime > 500) {
					this->robotState = r_forward;
					this->timer = millis();
				}
				break;
				
			case r_forward:
				closest = IRScanner.getClosestObject();
				Serial.print("forward ");
				Serial.print(closest[0]);
				Serial.print("cm ");
				Serial.println(closest[1]);
				if(closest[0] < 25) {
					this->robotState = r_avoid;
				} else {
					IRScanner.sweepAndMeasure();
					this->forward();
				}
				break;
			default:
				break;
		}
	}

private:

	unsigned long timer;
	
	void turnLeft() {
		this->setSpeedLeft(255);
		this->setSpeedRight(-255);
	}
	
	void turnRight() {
		this->setSpeedLeft(-255);
		this->setSpeedRight(255);
	}
	
	void backLeft() {
		this->setSpeedLeft(-60);
		this->setSpeedRight(-255);
	}
	
	void backRight() {
		this->setSpeedLeft(-255);
		this->setSpeedRight(-60);
	}
	
	void back() {
		this->setSpeedLeft(-255);
		this->setSpeedRight(-255);
	}
	
	void forward() {
		this->setSpeedLeft(255);
		this->setSpeedRight(255);
	}

	void setSpeedLeft(int speed) {
		this->LeftFrontMotor.setSpeed(speed);
		this->LeftBackMotor.setSpeed(-speed);
	}

	void setSpeedRight(int speed) {
		this->RightFrontMotor.setSpeed(speed);
		this->RightBackMotor.setSpeed(-speed);
	}

	Motor LeftFrontMotor;
	Motor LeftBackMotor;
	Motor RightFrontMotor;
	Motor RightBackMotor;
	Servo ScanServo;
	DistanceScanner IRScanner;
	enum state_type {
		r_scanning, r_forward, r_idle, r_avoid, r_scan
	};
	state_type robotState;
};

#endif
