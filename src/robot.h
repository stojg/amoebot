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
		int degree;
		switch (this->robotState) {
			case r_avoid:
				degree = IRScanner.getCurrentDegree();
				this->timer = millis();
				if(degree > 110) {
					this->robotState = r_right;
				} else if(degree < 70) {
					this->robotState = r_left;
				} else {
					this->robotState = r_pivot;
				}
				break;
				
			case r_pivot:
					this->turnLeft(128);
					if(elapsedTime > 300) {
						this->timer = millis();
						this->robotState = r_forward;
					}
				break;
			
			case r_left:
				degree = IRScanner.getCurrentDegree();
				distance = IRScanner.getCurrentDistance();
				this->turnLeft(128);
				if(elapsedTime > 100) {
					if(distance > 20) {
						this->timer = millis();
						this->robotState = r_forward;
					} else {
						this->timer = millis();
					}
				}
				break;
				
			case r_right:
				distance = IRScanner.getCurrentDistance();
				distance = IRScanner.getCurrentDistance();
				this->turnRight(128);
				if(elapsedTime > 100) {
					if(distance > 20) {
						this->timer = millis();
						this->robotState = r_forward;
					} else {
						this->timer = millis();
					}
				}
				break;
		
			case r_forward:
				IRScanner.sweepAndMeasure();
				distance = IRScanner.getCurrentDistance();
				if(distance < 20) {
					this->timer = millis();
					this->robotState = r_avoid;
				} else {
					this->forward(255);
				}
				break;
			default:
				break;
		}
	}

private:

	unsigned long timer;
	
	void turnLeft(int speed) {
		this->setSpeedLeft(speed);
		this->setSpeedRight(-speed);
	}
	
	void turnRight(int speed) {
		this->setSpeedLeft(-speed);
		this->setSpeedRight(speed);
	}
	
	void backLeft(int speed) {
		this->setSpeedLeft(-speed/4);
		this->setSpeedRight(-speed);
	}
	
	void backRight(int speed) {
		this->setSpeedLeft(-speed);
		this->setSpeedRight(-speed/4);
	}
	
	void back(int speed) {
		this->setSpeedLeft(-speed);
		this->setSpeedRight(-speed);
	}
	
	void forward(int speed) {
		this->setSpeedLeft(speed);
		this->setSpeedRight(speed);
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
		r_forward, r_avoid, r_left, r_right, r_pivot
	};
	state_type robotState;
};

#endif
