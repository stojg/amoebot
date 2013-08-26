#ifndef ROBOT_H
#define ROBOT_H

namespace Stojg
{
    class Robot {
    public:
        /*
         * @brief Class constructor.
         */
        Robot() : leftMotor(MOTOR_1_DIRECTION_PIN, MOTOR_1_SPEED_PIN), rightMotor(MOTOR_1_DIRECTION_PIN, MOTOR_1_SPEED_PIN) {
            initialize();
        }

        /*
         * @brief Initialize the robot state.
         */
        void initialize() {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
        }

        /*
         * @brief Update the state of the robot based on input from sensor and remote control.
         *  Must be called repeatedly while the robot is in operation.
         */
        void run() {
          
        }

    private:
        Motor leftFrontMotor;
        Motor rightFrontMotor;
        Motor leftBottomMotor;
        Motor rightBottomMotor;
    };
};

#endif

