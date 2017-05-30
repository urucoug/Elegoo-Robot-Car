//
// All code in this file are the "core functions," which you shouldn't have to touch unless something is buggy, although reading the functions below will help you understand the code. There are a few things in the code to be optimized, so my plan is to fix those over here and have you guys replace this file when I fix them.
//

// -------------------------------------------------------------------
// "Core" functions
//  --> These are the most basic tasks you can do with your robot--drive different directions, use the rangefinder, and set the servo
// -------------------------------------------------------------------

// Make the robot go forward
// --> This will make the robot continue going forwards until a different command is given involving the motors (stop, forward, left, right, or back up)
void _mForward()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("go forward!");  //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
}

// Back the robot car up
// --> This will make the robot continue going backwards until a different command is given involving the motors (stop, forward, left, right, or back up)
void _mBack()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("go back!");  //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
}

// Turn the robot car left
// --> This will make the robot continue turning left until a different command is given involving the motors (stop, forward, left, right, or back up)
void _mLeft()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW); 
  Serial.println("go left!");  //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
}

// Turn the robot car right
// --> This will make the robot continue turning right until a different command is given involving the motors (stop, forward, left, right, or back up)
void _mRight()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("go right!");  //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
} 

// Stop the robot car
void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("Stop!");  //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
} 

// Find the distance in front of the rangefinder
unsigned int _distanceTest()   
{
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance/58;       
  return (int)Fdistance;
} 

// Set the angle of your servo
// --> 0 degrees is all the way left; 90 degrees is to the middle; 180 degrees is all the way right
// --> You might have more than one servo, so this function allows you to specify which servo you're talking about
void _setServoAngle(Servo servo1, int angle){
  servo1.write(angle);
    //Serial.println((String)"Commanded angle: " + angle);
}
