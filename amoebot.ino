#include "pin_reference.h"
#include "motor_driver_rover.h"
#include "robot.h"
#include "mic.h"
#include <Servo.h>

IR irSensor(PA_IR);
Mic micSensor(PA_MIC);
Motor lfMotor(MOT_LF_SPEED, MOT_LF_DIRECTION);
Motor lbMotor(MOT_LB_SPEED, MOT_LB_DIRECTION);
Motor rfMotor(MOT_RF_SPEED, MOT_RF_DIRECTION);
Motor rbMotor(MOT_RB_SPEED, MOT_RB_DIRECTION);
Servo scanServo;

Robot robot(irSensor, lfMotor, lbMotor, rfMotor, rbMotor, scanServo);

void setup() {
  scanServo.attach(SERVO_IR);
  robot.initialize();
}

void loop() { 
    robot.wander();
    robot.run();
}
