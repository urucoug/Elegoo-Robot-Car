#include <AccelStepper.h>		// delete this line of code if you don't have a core_stepper_motor.ino file; needs to be in this file due to setup()

void setup() {
	Serial.begin(115200); // This is for ease of debugging; starts the Serial Monitor (no effect on the functionality of your code; only helps you see what it's doing)

  //// --- Initialize Servo (if applicable)
  //myservo.attach(3);// attach servo on pin 3 to servo object
  
	// --- Initialization of Stepper Motor ---
	AccelStepper stepper_initialize = getStepper1();	// The stepper file is declared and stored on core_stepper_motor; this creates a temporary version of that stepper object
	stepper_initialize.setMaxSpeed(1000.0);
	stepper_initialize.setAcceleration(100.0);
	stepper_initialize.setSpeed(1000);
	setStepper1(stepper_initialize);					// This sets the temporary stepper object as the real object inside core_stepper_motor.ino
	// --- End of initialization of Stepper Motor
	
}//--(end setup )---

void loop() {
	
	//while (timesToMove > 0){
		//_moveStepsNow(stepper1, 4076);	
		
		incrementAngleNow(getStepper1(), 90);	// Moves the stepper motor clockwise 71 degrees
		delay(500);
		incrementAngleNow(getStepper1(), -45);	// Moves the stepper motor clockwise 43 degrees
		delay(500);
		incrementAngleNow(getStepper1(), 360);	// Moves the stepper motor counterclockwise 278 degees
		
		delay(500);
		//_moveStepsNow(stepper1, -2038);
		//timesToMove = timesToMove - 1;
	//}
}
