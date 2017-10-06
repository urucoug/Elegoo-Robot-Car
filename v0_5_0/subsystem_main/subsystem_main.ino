//#include <AccelStepper.h>		// delete this line of code if you don't have a core_stepper_motor.ino file; needs to be in this file due to setup()
#include <Servo.h> 				// delete this line of code if you aren't using a servo on your subsystem

Servo subsysServo1; // create servo object to control servo
//Servo subsysServo2; // use this line if you're attaching a second servo

void setup() {
	Serial.begin(115200); // This is for ease of debugging; starts the Serial Monitor (no effect on the functionality of your code; only helps you see what it's doing)

	// --- Initialization of Servo ---
	//   - delete this section of code if there are no servos attached to your Arduino Nano -
	subsysServo1.attach(7);// attach servo on pin 7 to servo object
	//subsysServo2.attach(8);// attach servo on pin 8 to servo object (use this line if you're attaching a second servo)
	// --- End of initialization of servo ---
  
//	// --- Initialization of Stepper Motor ---
//	//     - delete this section of code if there is no stepper motor attached to your Arduino Nano -
//	AccelStepper stepper_initialize = getStepper1();	// The stepper file is declared and stored on core_stepper_motor; this creates a temporary version of that stepper object
//	stepper_initialize.setMaxSpeed(1000.0);
//	stepper_initialize.setAcceleration(100.0);
//	stepper_initialize.setSpeed(1000);
//	setStepper1(stepper_initialize);					// This sets the temporary stepper object as the real object inside core_stepper_motor.ino
//	// --- End of initialization of Stepper Motor ---
	
}//--(end setup )---

void loop() {
	
	// Demo of the capability to control the various actuators
	
//	// Turn the electromagnet on
//	electromagnetOn();
	
	// Move the servo
	setServoAngle(subsysServo1,0);
	delay(1000);
	setServoAngle(subsysServo1,120);
	delay(2000);

//	//Move the Stepper motor
//	incrementAngleNow(getStepper1(), 1000);	// Moves the stepper motor clockwise 71 degrees
//	delay(300);
//	
//	// Turn the electromagnet off
//	electromagnetOff();
//	
//	// Turn the stepper motor in the opposite direction
//	incrementAngleNow(getStepper1(), -500);	// Moves the stepper motor clockwise 43 degrees
//	delay(2000);
}
