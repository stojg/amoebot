#include "pin_reference.h"
#include "motor_driver_rover.h"
#include "robot.h"
#include "distance_scanner.h"
#include <Servo.h>


Motor lfMotor(MOT_LF_SPEED, MOT_LF_DIRECTION);
Motor lbMotor(MOT_LB_SPEED, MOT_LB_DIRECTION);
Motor rfMotor(MOT_RF_SPEED, MOT_RF_DIRECTION);
Motor rbMotor(MOT_RB_SPEED, MOT_RB_DIRECTION);
IR irSensor(PA_IR);
Servo myservo;
DistanceScanner scanner(myservo, irSensor);

Robot robot(lfMotor, lbMotor, rfMotor, rbMotor, scanner);

void setup() {
	Serial.begin(9600);
	myservo.attach(SERVO_IR);
	robot.initialize();
}

void loop() { 
    robot.run();

}
