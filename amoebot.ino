#include "pin_reference.h"
#include "motor_driver_rover.h"
#include "robot.h"


IR irsensor(0);
Robot robot(irsensor);

void setup()
{

  robot.initialize();
}

void loop()
{ 
  robot.run();
}
