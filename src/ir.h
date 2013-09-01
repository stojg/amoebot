#ifndef IR_H
#define IR_H

/**
 * This class handles taking measurements from the sharp GP2Y0A02YK0F
 * @link https://sparkfun.com/datasheets/Sensors/Infrared/gp2y0a02yk_e.pdf
 * It has can measure distance between 20cm - 150cm and have a response time
 * of 38.3ms±9.6ms (max 47.9ms)
 *
 */
class IR {
public:

	/*
	 * @brief Class constructor.
	 */
	IR(int pin) : irpin(pin), lastMillis(millis()), lastDistance(60), responseTime(50) {
		pinMode(irpin, OUTPUT);
	}

	float getDistance() {
		int currentTime = millis();
		if (currentTime - lastMillis < responseTime) {
			return lastDistance;
		}

		int value = analogRead(irpin);
		lastDistance = value;

		lastDistance = 10650.08 * pow(value, -0.935) - 10;
		lastMillis = currentTime;
		return lastDistance;
	}

private:
	int irpin;
	unsigned int lastMillis;
	float lastDistance;
	int responseTime;
};
#endif

