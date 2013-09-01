#include "pin_reference.h"
#include "motor.h"
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
	int reading = analogRead(5);
	float val = fmap(reading, 0, 1023, 0.0, 5.0);
	//Serial.println(val); 
	robot.run();
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}