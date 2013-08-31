/* 
 * File:   scanner.h
 * Author: stig
 *
 * Created on August 31, 2013, 1:30 PM
 */


#ifndef SCANNER_H
#define	SCANNER_H
#include <Servo.h>

class Scanner {
public:
	
	Scanner(int pin): servopin(pin) {
		myservo.attach(pin);
		myservo.write(180);
	}
	
	void degree(unsigned int degree) {
		myservo.write(degree);
	}
	
private:
	int servopin;
	Servo myservo;
	
};

#endif	/* SCANNER_H */
