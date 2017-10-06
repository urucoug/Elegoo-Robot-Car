/*
All code in this file is part of the "core functions," which you shouldn't have to touch
*/

/*
Electrical setup of MG995 Servo:
- Orange wire: connect to Arduino Nano pin 7 (setup is in subsystem_main.ino if this needs to change)
--The servo can be connected to any analog (except A6 or A7) or digital pin; just make sure to deconflict with the other actuators/sensors if you use other than the default
- Red wire: connect to positive terminal of buck converter, regulated to 5V; don't connect to your Arduino; pulling too much amperage will fry the linear power regulator
- Brown wire: connect to negative terminal of buck converter

Code setup in subsystem_main.ino:
	If this .ino file is to be used, the following code should be placed in subsystem_main:
	-Above "setup()":
	#include <Servo.h>	
	
	-In "setup()":
	subsysServo1.attach(7);// attach servo on pin 7 to servo object
 

Servo References & Learning:
- The MG995 servo has a range from 0-120 degrees
*/


//-----------------------------------------------------------------------------------------
//setUpUltrasonicRangeFinder
//-----------------------------------------------------------------------------------------
void setServoAngle(Servo myServo, int angle) {	
	// I'm writing this as a function to keep this generic, and troubleshoot as necessary

	// consider constraining input to 0 - 120 degrees for angle inputs	
	myServo.write(angle);
	Serial.println("Angle passed: "+(String)angle);
}



