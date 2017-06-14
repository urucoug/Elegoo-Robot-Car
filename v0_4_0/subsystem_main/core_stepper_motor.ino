/*
	This file contains the core stepper motor files
	-Utilizes the "AccelStepper" library
	-Minimizes the stepper motor footprint on subsystem_main.ino, since the custom robot may or may not have a stepper motor
*/

/*	
	Electrical setup of the 28BYJ-48 Stepper Motor and accompanying H-bridge:
	-Plug stepper motor into the H-bridge that it comes with
	-Connect positive terminal of the robot car battery to the (+) male pin of the H-bridge
	-Connect negative terminal of the robot car battery to the (-) male pin of the H-bridge
	-IN1, IN2, IN3, and IN4 on H-bridge should be connected to digital pins 3, 4, 5, and 6, respectively
	--these pins must be used, due to the AccelStepper library used for the stepper motor
		
	Code setup in subsystem_main.ino:
	If this .ino file is to be used, the following code should be placed in subsystem_main:
	-Above "setup()":
	#include <AccelStepper.h>
	
	-In "setup()":
	// --- Initialization of Stepper Motor ---
	AccelStepper stepper_initialize = getStepper1();	// The stepper file is declared and stored on core_stepper_motor; this creates a temporary version of that stepper object
	stepper_initialize.setMaxSpeed(1000.0);				// value inside bracket can be altered to set max speed of stepper motor rotations
	stepper_initialize.setAcceleration(100.0);			// value inside bracket can be altered to set max acceleration of stepper motor rotations
	stepper_initialize.setSpeed(1000);					// value inside bracket can be altered to set initial speed of stepper motor rotations
	setStepper1(stepper_initialize);					// This sets the temporary stepper object as the real object inside core_stepper_motor.ino
	// --- End of initialization of Stepper Motor
*/

#define HALFSTEP 8
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
static AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);		// by declaring this varible "static", it becomes a global variable for this file only. The way I access this file on subsystem_main.ino is to use the globally visible "getStepper1" and "setStepper1" functions, which are inside core_stepper_motor.ino, and thus can access the variable
//Servo myservo; // create servo object to control servo

static int timesToMove = 3;				// How many times to complete rotation sequence, for testing purposes
static int stepsPerRotation = 4076;		// Number of steps there are in half-step mode for the stepper motor; 4076 is the approximate number for the 28byj-48 Stepper Motor
static float storedError = 0;			// Needed for Stepper motor, incrementAngle function, to improve accuracy

AccelStepper getStepper1() {
	// having a getStepper function allows us to keep a lot of the code on this file instead of on other functions
	return stepper1;	
}

void setStepper1(AccelStepper stepper1_input) {
	stepper1 = stepper1_input;
}

// ----------
// This function rotates the stepper motor a number of steps
//   Positive is clockwise; negative, counterclockwise
//   There are approximately 4076 steps is a rotation
// ----------
void moveStepsNow(AccelStepper stepper1, signed long numSteps) {
	// Assuming a gear ratio of 1:63.684; so, theoretically, 64x63.684 steps = 4076 steps per revolution
	stepper1.moveTo(numSteps);

	while (abs(stepper1.distanceToGo()) > 0) {
		stepper1.run();
		//Serial.println(stepper1.distanceToGo());
	}
	Serial.println((String)"Moving " + numSteps + (String)" steps\n");
}

// This will be the same as 'moveStepsNow', except non-blocking (Aka, stepper1.run() will need to occur in Arduino_Nano_main.ino)
void moveStepsNonBlocking(AccelStepper stepper1, signed long numSteps) {
    
}

// ----------
// This function rotates the stepper motor according to the specified angle
//   Positive is clockwise; negative, counterclockwise
//   Numbers greater than 360 are allowed
//   As this function translates the angle into a number of steps, an error is tracked that is added to the next operation that does a similar translation
// ----------
void incrementAngleNow(AccelStepper stepper1, signed long angle1) {
	// There are 360 degrees per rotation, and 4076 steps per rotation
	float stepsToMove_float = (float)angle1/360*stepsPerRotation;
	
	// stepsToMove will be truncated because the step function takes only whole steps; 
	//   to eliminate stackup of error, the truncated decimal is added to the next 
	//   operation that translates between angle and steps
	stepsToMove_float = stepsToMove_float + storedError;
	
	storedError = stepsToMove_float-trunc(stepsToMove_float);
	signed long stepsToMove_int = trunc(stepsToMove_float);
	
	// // The lines below are for troubleshooting if necessary
	// Serial.println((String)"Angle passed to function: " + angle1 + (String)"; Steps to move, float: " + stepsToMove_float);
	// Serial.println((String)"Steps commanded: " + stepsToMove_int + (String)"; Stored error: " + storedError +"\n");

	// Move the number of steps calculated through the already-existant moveStepsNow function
	moveStepsNow(stepper1,stepsToMove_int);
}

void incrementAngleNonBlocking(AccelStepper stepper1, signed long angle1) {
	// This will be the same as incrementAngleNow, except it won't block
}

void absoluteAngleNow(AccelStepper stepper1, signed long angle1) {
	// Specifies an angle the servo should move to relative to the starting position
	// Motor executes code immediately, without advancing the Arduino_nano_main.ino loop() until the motor has reached its final position
	
	// This will require keeping track of the angle in a global variable for all movements
}

void absoluteAngleNonBlocking(AccelStepper stepper1, signed long angle1) {
	// Same as absoluteAngleNow(), but non-blocking
}
