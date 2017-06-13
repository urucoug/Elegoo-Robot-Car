/*
All code in this file is part of the "core functions," which you shouldn't have to touch
*/

/*
Servo References & Learning:
- The MG995 servos that were purchased as part of this project have a range from 0-120 degrees; if using a different servo, use different values
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



