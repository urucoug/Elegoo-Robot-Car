#include <AccelStepper.h>
//#include <Servo.h> // add the servo library, so we can use pre-made functions to control it

// For AccelStepper
#define HALFSTEP 8

// Stepper Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
//Servo myservo; // create servo object to control servo

int timesToMove = 3;			// How many times to complete rotation sequence, for testing purposes
int stepsPerRotation = 4076;	// Number of steps there are in half-step mode for the stepper motor; 4076 is the approximate number for the 28byj-48 Stepper Motor
float storedError = 0;			// Needed for Stepper motor, _incrementAngle function, to improve accuracy

void setup() {
	Serial.begin(115200); // This is for ease of debugging; starts the Serial Monitor (no effect on the functionality of your code; only helps you see what it's doing)

  //// --- Initialize Servo (if applicable)
  //myservo.attach(3);// attach servo on pin 3 to servo object
  
	// --- Initialization of Stepper Motor ---
	stepper1.setMaxSpeed(1000.0);
	stepper1.setAcceleration(100.0);
	stepper1.setSpeed(1000);
	// --- End of initialization of Stepper Motor
	
}//--(end setup )---

void loop() {
	
	//while (timesToMove > 0){
		//_moveStepsNow(stepper1, 4076);	
		
		_incrementAngleNow(stepper1, 71);	// Moves the stepper motor clockwise 71 degrees
		delay(500);
		_incrementAngleNow(stepper1, 43);	// Moves the stepper motor clockwise 43 degrees
		delay(500);
		_incrementAngleNow(stepper1, -278);	// Moves the stepper motor counterclockwise 278 degees
		
		delay(500);
		//_moveStepsNow(stepper1, -2038);
		//timesToMove = timesToMove - 1;
	//}
}
