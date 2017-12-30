#include <Servo.h>
#include <eRCaGuy_ButtonReader-master>
Servo subsysServo1; // create servo object to control servo

// Pins used
int reloadButtonPin = 2;    // Attach button to digital pin 2
int servoPin = 7;           // Attach rubber band shooter servo to pin 7

// Variables used to control range of motion of rubber band servo
int origTestRangeInitial = 30;
int origTestRangeFinal = 100;
int origTestRangeIncrement = 5;

// Variable to tell state of button press to reload rubber band servo
int pressStateReload = 0;   // 0 is not pressed; 1 is pressed; 2 is in transit; reset back to 0 after rubber bands are reloaded

void setup() {
	Serial.begin(115200);
  
  // // attach servo on pin designated for rubber band servo
	// subsysServo1.attach(servoPin);   
  
  // set designated pin as input for reading button
  pinMode(reloadButtonPin, INPUT_PULLUP);
  
  // Setup internal LED on Arduino as output
  pinMode(13, OUTPUT);  
}

void loop() {	

	// delay(2000);

	// // Insert the linkage slowly into the lug
	// for (int i=origTestRangeInitial; i<=origTestRangeFinal;i=i+origTestRangeIncrement) {
		// setServoAngle(subsysServo1,i);
		// delay(1500);
	// }
	
	// delay(10000);
	// setServoAngle(subsysServo1,70);
	// delay(2500);
	// setServoAngle(subsysServo1,30);
	// delay(2500);
  
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(reloadButtonPin);
  //print out the value of the pushbutton
  Serial.println(sensorVal);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
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
		
	