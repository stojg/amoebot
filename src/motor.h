/**
 * @file motor_driver_rover.h
 * @brief Motor device driver for the Rover 5 motor shield.
 * @author Stig Lindqvist <stojg.lindqvist@gmail.coom>
 */

#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:

	/*
	 * @brief Class constructor.
	 */
	Motor(int speed_pin, int direction_pin) : speedPin(speed_pin), directionPin(direction_pin), currentSpeed(0) {
		pinMode(direction_pin, OUTPUT);
		pinMode(speed_pin, OUTPUT);
	}

	/**
	 * Set speed
	 * 
     * @param speed -255 to 255
     */
	void setSpeed(int speed) {
		if (speed == 0) {
			analogWrite(speedPin, 0);
			this->currentSpeed = 0;
			return;
		}

		int motorspeed = constrain(abs(speed), 0, 255);

		this->currentSpeed = motorspeed;

		if (speed >= 0) {
			digitalWrite(directionPin, HIGH);
			analogWrite(speedPin, motorspeed);
		} else {
			digitalWrite(directionPin, LOW);
			analogWrite(speedPin, motorspeed);
		}
	}

	int getSpeed() const {
		return currentSpeed;
	}

private:
	int currentSpeed;
	int speedPin;
	int directionPin;
};

#endif
