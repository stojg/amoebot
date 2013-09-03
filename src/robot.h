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
		IRScanner.moveTo(90);
		delay(200);
		this->robotState = r_forward;
		this->timer = millis();

	}

	void stop() {
		this->setSpeedRight(0);
		this->setSpeedLeft(0);
	}

	void run() {
		unsigned long elapsedTime = millis() - this->timer;
		
		int distance;
		switch (this->robotState) {
			case r_avoid:
				if(random(2) > 0) {
					this->robotState = r_left;
				} else {
					this->robotState = r_right;
				}
				break;
			
			case r_left:
				this->turnLeft();
				if(elapsedTime > 1000) {
					distance = IRScanner.getCurrentDistance();
					if(distance > 20) {
						this->timer = millis();
						this->robotState = r_forward;
					} else {
						this->timer = millis();
						this->robotState = r_left;
					}
				}
				break;
				
			case r_right:
				distance = IRScanner.getCurrentDistance();
				this->turnRight();
				if(elapsedTime > 1000) {
					distance = IRScanner.getCurrentDistance();
					if(distance > 20) {
						this->timer = millis();
						this->robotState = r_forward;
					} else {
						this->timer = millis();
						this->robotState = r_right;
					}
				}
				break;
		
			case r_forward:
				distance = IRScanner.getCurrentDistance();
				if(distance < 20) {
					this->timer = millis();
					this->robotState = r_avoid;
				} else {
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
		if(digitalRead(13)) {
			this->LeftFrontMotor.setSpeed(0);
			this->LeftBackMotor.setSpeed(0);
			return;
		}
		this->LeftFrontMotor.setSpeed(speed);
		this->LeftBackMotor.setSpeed(-speed);
	}

	void setSpeedRight(int speed) {
		if(digitalRead(13)) {
			this->RightFrontMotor.setSpeed(0);
			this->RightBackMotor.setSpeed(0);
			return;
		}
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
		r_forward, r_avoid, r_left, r_right
	};
	state_type robotState;
};

#endif
