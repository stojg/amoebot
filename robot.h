#ifndef ROBOT_H
#define ROBOT_H
#include "IR.h"
#include "moving_average.h"
#include "motor_driver_rover.h"
#include "mic.h"

class Robot {
  
    public:
        /*
         * @brief Class constructor.
         */
        Robot(IR & irsensor, Mic & micSensor, Motor & lfMotor, Motor & lbMotor, Motor & rfMotor, Motor & rbMotor) : irSensor(irsensor),  micSensor(micSensor), distanceAverage(80),
          LeftFrontMotor(lfMotor),
          LeftBackMotor(lbMotor),
          RightFrontMotor(rfMotor),
          RightBackMotor(rbMotor)
        {
          
        }

        void initialize() {
            Serial.begin(9600);
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
          int distance = distanceAverage.add(irSensor.getDistance());
          
          if(this->speed > 0) {
            this->SetSpeedLeft(this->speed);
            this->SetSpeedRight(this->speed);            
          } else {
            this->SetSpeedLeft(0);
            this->SetSpeedRight(0);            
          }
          //float dir = cos(this->heading);
          //Serial.println(dir);
          
        }

    private:
    
      void SetSpeedLeft(int speed) {
        this->LeftFrontMotor.setSpeed(speed);
        this->LeftBackMotor.setSpeed(-speed);
      }
      
      void SetSpeedRight(int speed) {
        this->RightFrontMotor.setSpeed(speed);
        this->RightBackMotor.setSpeed(-speed);
      }
    
      IR irSensor;
      int heading;
      unsigned int speed;
      Motor LeftFrontMotor;
      Motor LeftBackMotor;
      Motor RightFrontMotor;
      Motor RightBackMotor;
      Mic micSensor;
      MovingAverage<unsigned int, 10> distanceAverage;
      MovingAverage<int, 10> volumeAverage;
    };

#endif
