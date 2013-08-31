/**
 * @file motor_driver_rover.h
 * @brief Motor device driver for the Rover 5 motor shield.
 * @author Stig Lindqvist <stojg.lindqvist@gmail.coom>
 */

#ifndef MOTOR_DRIVER_ROVER_H
#define MOTOR_DRIVER_ROVER_H
#include "motor_driver.h"
#include "Arduino.h"

    class Motor : public MotorDriver {
    public:
        /*
         * @brief Class constructor.
         * @param number the DC motor number to control, from 1 to 4.
         */
        Motor(int speed_pin, int direction_pin) : MotorDriver(), speed_pin(speed_pin), direction_pin(direction_pin), currentSpeed(0) {
          pinMode(direction_pin, OUTPUT);
          pinMode(speed_pin, OUTPUT);
        }

        void setSpeed(int speed) {
          
            if(speed == 0) {
              analogWrite(speed_pin, 0);
              currentSpeed = 0;
              return;
            }
            
            int motorspeed = constrain(abs(speed),0,255);
            
            currentSpeed = motorspeed;
            
            if (speed >= 0) {
                digitalWrite(direction_pin, HIGH);
                analogWrite(speed_pin, motorspeed);
            } else {
                digitalWrite(direction_pin, LOW);
                analogWrite(speed_pin, motorspeed);
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

#endif
