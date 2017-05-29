//
// All code in this file are the "core functions," which you shouldn't have to touch
//

// -------------------------------------------------------------------
// "Core" functions for servo controlling the direction the car's ping sensor direction
//  --> just one function so far
// -------------------------------------------------------------------

// Set the angle of your servo
// --> 0 degrees is all the way left; 90 degrees is to the middle; 180 degrees is all the way right
// --> You might have more than one servo, so this function allows you to specify which servo you're talking about
void _setServoAngle(Servo servo1, int angle){
  servo1.write(angle);
    //Serial.println((String)"Commanded angle: " + angle);
}
