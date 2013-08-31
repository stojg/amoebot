#ifndef ROBOT_H
#define ROBOT_H
#include "IR.h"
#include "moving_average.h"
#include "motor_driver_rover.h"
#include <Servo.h>

class Robot {
public:

	/*
	 * @brief Class constructor.
	 */
	Robot(IR & irsensor, Motor & lfMotor, Motor & lbMotor, Motor & rfMotor, Motor & rbMotor, Servo & scanner) : irSensor(irsensor), distanceAverage(80),
	LeftFrontMotor(lfMotor),
	LeftBackMotor(lbMotor),
	RightFrontMotor(rfMotor),
	RightBackMotor(rbMotor),
	ScanServo(scanner) {

	}

	/**
	 * Initialize the robot
	 */
	void initialize() {

		this->scanToRight = false;
		scanDegree = 90;
		ScanServo.write(scanDegree);
		delay(200);
	}

	void wander() {
		this->setHeading(0);
		this->setSpeed(100);
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
		this->scan();

		int distance = this->getClosestObstacle();
		if (distance < 23) {
			if (this->closestObstacleDirection < 90) {
				this->SetSpeedRight(-128);
				this->SetSpeedLeft(128);
			} else {
				this->SetSpeedRight(128);
				this->SetSpeedLeft(-128);
			}

		} else {
			this->SetSpeedLeft(128);
			this->SetSpeedRight(128);
		}
	}

private:

	void scan() {
		if (millis() - scannerTime < 4) {
			return;
		}
		scannerTime = millis();

		if (this->scanToRight) {
			this->scanDegree++;
		} else {
			this->scanDegree--;
		}

		if (this->scanDegree >= 160) {
			this->scanToRight = false;

		}

		if (this->scanDegree <= 20) {
			this->closestObstacleDistance = 1000;
			this->scanToRight = true;
		}

		ScanServo.write(this->scanDegree);

		int distance = distanceAverage.add(irSensor.getDistance());
		if (distance > 60) {
			return;
		}
		if (distance < this->closestObstacleDistance) {
			this->closestObstacleDistance = distance;
			this->closestObstacleDirection = this->scanDegree;
		}
		//Serial.print(this->closestObstacleDistance);
		//Serial.print("cm ");
		//Serial.println(this->closestObstacleDirection);
	}

	long scannerTime;
	int scanDegree;
	boolean scanToRight;

	int getClosestObstacle() {
		return this->closestObstacleDistance;
	}

	void SetSpeedLeft(int speed) {
		//this->LeftFrontMotor.setSpeed(speed);
		//this->LeftBackMotor.setSpeed(-speed);
	}

	void SetSpeedRight(int speed) {
		// this->RightFrontMotor.setSpeed(speed);
		// this->RightBackMotor.setSpeed(-speed);
	}

	IR irSensor;
	int heading;
	unsigned int speed;
	int closestObstacleDistance;
	int closestObstacleDirection;
	Motor LeftFrontMotor;
	Motor LeftBackMotor;
	Motor RightFrontMotor;
	Motor RightBackMotor;
	Servo ScanServo;
	MovingAverage<unsigned int, 3> distanceAverage;
};

#endif
