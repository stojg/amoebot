#ifndef ROBOT_H
#define ROBOT_H
#include "IR.h"
#include "moving_average.h"
#include "motor_driver_rover.h"
#include "distance_scanner.h"
#include <Servo.h>

class Robot {
public:

	/*
	 * @brief Class constructor.
	 */
	Robot(Motor &lfMotor, Motor &lbMotor, Motor &rfMotor, Motor &rbMotor, DistanceScanner &scanner ) : 
	LeftFrontMotor(lfMotor),
	LeftBackMotor(lbMotor),
	RightFrontMotor(rfMotor),
	RightBackMotor(rbMotor),
	IRScanner(scanner)
	{
		
		//this->timer = millis();
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

	void wander() {
		//this->setHeading(0);
		//this->setSpeed(100);
	}

	void stop() {
		this->setSpeed(0);
	}

	// Right hand coordination system

	void setHeading(int deg) {
		this->heading = deg;
	}

	void setSpeed(unsigned int percentage) {
		this->speed = percentage;
	}

	void run() {
		unsigned long currentTime = millis();
		unsigned long elapsedTime = currentTime - this->timer;
		int *closest = IRScanner.getClosestObject();;
		
		int distance = closest[0];
		int direction = closest[1];
		
		switch (this->robotState) {
			// wait to stabilize analog reading
			case r_scanning:
				if(elapsedTime >= 2) {
					IRScanner.run();
					
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
				if(elapsedTime >= 2) {
					this->timer = currentTime;
					this->robotState = r_scanning;
				}
				break;
			default:
				Serial.println(this->robotState);
				break;
        }
	}

private:

	unsigned long timer;

	int getClosestObstacle() {
		return this->closestObstacleDistance;
	}

	void SetSpeedLeft(int speed) {
		this->LeftFrontMotor.setSpeed(speed);
		this->LeftBackMotor.setSpeed(-speed);
	}

	void SetSpeedRight(int speed) {
		this->RightFrontMotor.setSpeed(speed);
		this->RightBackMotor.setSpeed(-speed);
	}

	int heading;
	unsigned int speed;
	int closestObstacleDistance;
	int closestObstacleDirection;
	Motor LeftFrontMotor;
	Motor LeftBackMotor;
	Motor RightFrontMotor;
	Motor RightBackMotor;
	Servo ScanServo;
	DistanceScanner IRScanner;
	enum state_type { r_scanning, r_moving };
    state_type robotState;
};

#endif
