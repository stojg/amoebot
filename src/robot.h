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
		delay(1000);
		IRScanner.moveTo(20);
		delay(200);
		this->robotState = r_scanning;
		this->timer = millis();

	}

	void stop() {
		this->SetSpeedRight(0);
		this->SetSpeedLeft(0);
	}

	void run() {
		unsigned long currentTime = millis();
		unsigned long elapsedTime = currentTime - this->timer;
		int *closest = IRScanner.getClosestObject();

		int distance = closest[0];
		int direction = closest[1];
		
		IRScanner.run();

		switch (this->robotState) {
				// wait to stabilize analog reading
			case r_scanning:
				this->SetSpeedLeft(0);
				this->SetSpeedRight(0);
				if (elapsedTime >= 1000) {
					this->timer = currentTime;
					this->robotState = r_moving;
				}
				break;
			case r_moving:
				// Wait until it have moved
				if (distance < 20) {
					if (direction < 90) {
						this->SetSpeedRight(-255);
						this->SetSpeedLeft(255);
					} else {
						this->SetSpeedRight(255);
						this->SetSpeedLeft(-255);
					}
				} else {
					this->SetSpeedLeft(255);
					this->SetSpeedRight(255);
				}
				
				if (elapsedTime >= 10000) {
					this->timer = currentTime;
					this->robotState = r_scanning;
				}
				break;
			default:
				break;
		}
	}

private:

	unsigned long timer;

	void SetSpeedLeft(int speed) {
		this->LeftFrontMotor.setSpeed(speed);
		this->LeftBackMotor.setSpeed(-speed);
	}

	void SetSpeedRight(int speed) {
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
		r_scanning, r_moving
	};
	state_type robotState;
};

#endif
