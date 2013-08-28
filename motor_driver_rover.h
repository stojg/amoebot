/**
 * @file motor_driver_rover.h
 * @brief Motor device driver for the Rover 5 motor shield.
 * @author Stig Lindqvist <stojg.lindqvist@gmail.coom>
 */

#include "motor_driver.h"
#include "Arduino.h"

    class Motor : public MotorDriver {
    public:
        /*
         * @brief Class constructor.
         * @param number the DC motor number to control, from 1 to 4.
         */
        Motor(int speed_pin, int direction_pin) : MotorDriver(), speed_pin(speed_pin), direction_pin(direction_pin), currentSpeed(0) {
        }

        void setSpeed(int speed) {
            currentSpeed = speed;
            if (speed >= 0) {
                digitalWrite(direction_pin, HIGH);
                analogWrite(speed_pin, speed);
            } else {
                digitalWrite(direction_pin, LOW);
                analogWrite(speed_pin, -speed);
            }
        }

        int getSpeed() const {
            return currentSpeed;
        }

    private:
        int currentSpeed;
        int speed_pin;
        int direction_pin;
    };

