#include <Servo.h>
Servo subsysServo1; // create servo object to control servo

int origTestRangeInitial = 30;
int origTestRangeFinal = 100;
int origTestRangeIncrement = 5;

void setup() {
	Serial.begin(115200);
	subsysServo1.attach(7);// attach servo on pin 7 to servo object 
}

void loop() {	

	delay(2000);

	// Insert the linkage slowly into the lug
	for (int i=origTestRangeInitial; i<=origTestRangeFinal;i=i+origTestRangeIncrement) {
		setServoAngle(subsysServo1,i);
		delay(1500);
	}
	
	delay(10000);
	setServoAngle(subsysServo1,70);
	delay(2500);
	setServoAngle(subsysServo1,30);
	delay(2500);
}






// int microMovementsEnd = 25;
// int microMovementIncrement = 5;

/* // Micro movement mode
	for (int i=microMovementsEnd; i<=origTestRangeInitial;i=i+microMovementIncrement) {
		setServoAngle(subsysServo1,i);
		delay(2500);
	} */
	
	
	// // Remove the linkage slowly out of the lug
	// for (int i=origTestRangeFinal; i>=origTestRangeInitial;i=i-origTestRangeIncrement) {
		// setServoAngle(subsysServo1,i);
		// delay(2500);
	// }
	
	/* // Final motion in micro movement mode
	for (int i=origTestRangeInitial; i>=microMovementsEnd;i=i-microMovementIncrement) {
		setServoAngle(subsysServo1,i);
		delay(2500);
	} */
		
	