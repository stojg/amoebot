#include "pin_reference.h"
#include "motor_driver_rover.h"
#include "robot.h"
#include "scanner.h"
#include <Servo.h>

IR irSensor(PA_IR);
Motor lfMotor(MOT_LF_SPEED, MOT_LF_DIRECTION);
Motor lbMotor(MOT_LB_SPEED, MOT_LB_DIRECTION);
Motor rfMotor(MOT_RF_SPEED, MOT_RF_DIRECTION);
Motor rbMotor(MOT_RB_SPEED, MOT_RB_DIRECTION);
Servo scanServo;

Robot robot(irSensor, lfMotor, lbMotor, rfMotor, rbMotor, scanServo);

void setup() {
	Serial.begin(9600);
	scanServo.attach(SERVO_IR);
	robot.initialize();
}

void loop() { 
    robot.wander();
    robot.run();
}
