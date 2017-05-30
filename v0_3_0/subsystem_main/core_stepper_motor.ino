
// ----------
// This function rotates the stepper motor a number of steps
//   Positive is clockwise; negative, counterclockwise
//   There are approximately 4076 steps is a rotation
// ----------
void _moveStepsNow(AccelStepper stepper1, signed long numSteps) {
	// Assuming a gear ratio of 1:63.684; so, theoretically, 64x63.684 steps = 4076 steps per revolution
	stepper1.moveTo(numSteps);

	while (abs(stepper1.distanceToGo()) > 0) {
		stepper1.run();
		//Serial.println(stepper1.distanceToGo());
	}
	Serial.println((String)"Moving " + numSteps + (String)" steps\n");
}

// This will be the same as '_moveStepsNow', except non-blocking (Aka, stepper1.run() will need to occur in Arduino_Nano_main.ino)
void _moveStepsNonBlocking(AccelStepper stepper1, signed long numSteps) {
    
}

// ----------
// This function rotates the stepper motor according to the specified angle
//   Positive is clockwise; negative, counterclockwise
//   Numbers greater than 360 are allowed
//   As this function translates the angle into a number of steps, an error is tracked that is added to the next operation that does a similar translation
// ----------
void _incrementAngleNow(AccelStepper stepper1, signed long angle1) {
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

	// Move the number of steps calculated through the already-existant _moveStepsNow function
	_moveStepsNow(stepper1,stepsToMove_int);
}

void _incrementAngleNonBlocking(AccelStepper stepper1, signed long angle1) {
	// This will be the same as _incrementAngleNow, except it won't block
}

void _absoluteAngleNow(AccelStepper stepper1, signed long angle1) {
	// Specifies an angle the servo should move to relative to the starting position
	// Motor executes code immediately, without advancing the Arduino_nano_main.ino loop() until the motor has reached its final position
	
	// This will require keeping track of the angle in a global variable for all movements
}

void _absoluteAngleNonBlocking(AccelStepper stepper1, signed long angle1) {
	// Same as _absoluteAngleNow(), but non-blocking
}
