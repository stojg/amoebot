#include "pin_reference.h"
#include "motor_driver_rover.h"
#include "robot.h"

Stojg::Robot robot();

void setup()
{

	robot.initialize();
}

void loop()
{
	robot.run();
}
