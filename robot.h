#ifndef ROBOT_H
#define ROBOT_H
#include "IR.h"
#include "moving_average.h"

class Robot {
  
    public:
        /*
         * @brief Class constructor.
         */
        Robot(IR & irsensor) : irSensor(irsensor),  distanceAverage(80) {
          
        }

        void initialize() {
            Serial.begin(9600);
        }

        void run() {
          int distance = distanceAverage.add(irSensor.getDistance());
          Serial.println(distance);
          delay(100);
        }

    private:
      IR irSensor;
      MovingAverage<unsigned int, 5> distanceAverage;
    };

#endif
