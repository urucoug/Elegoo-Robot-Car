/*
Robotics Merit Badge robot car
Utilizes Elegoo Robot on Amazon: https://www.amazon.com/Elegoo-Ultrasonic-Bluetooth-Intelligent-Educational/dp/B01M2Z9ZTV/ref=as_li_ss_tl?s=aps&ie=UTF8&qid=1495592752&sr=1-1-catcorr&keywords=elegoo+robot+car&linkCode=ll1&tag=wwwel-20&linkId=e8867429e668b295e301b8e5722a127e
 - $74 as of 29 May 2017 
 
Project by Kevin Nufer
and Gabriel Staples - http://www.ElectricRCAircraftGuy.com

Project Started: May 2017 
Last Updated: 29 May 2017 

SEE "Elegoo-Robot-Car - History.txt" FOR HISTORY 

HELPFUL REFERENCES:
Servo:
 - https://www.arduino.cc/en/Reference/Servo
Ultrasonic Rangefinder ("ping" sensor) References & Learning:
 - https://www.arduino.cc/en/tutorial/ping
 - https://www.arduino.cc/en/Reference/PulseIn
 
*/

//Include necessary libraries 
#include <Servo.h> //add the servo library, so we can use pre-made functions to control it

//Macros
#define setServoAngle(myServo, angle) myServo.write(angle)
//the below define allows you to type either "avoidObstacles" OR "goStraightOrLeftOrRight" in your code to call this function 
#define goStraightOrLeftOrRight avoidObstacles

//Global Variables, constants, & objects 

//servo to aim the ultrasonic rangefinder
Servo myServo; //create servo object to control servo

//car variables we may want to access in this file (declare with keyword "extern" when they are defined "externally" [in a different file] elsewhere)
extern const byte DEFAULT_SPEED; //PWM value; can be 0-255, inclusive
// const byte MIN_SPEED = 0;
// const byte MAX_SPEED = 255;

int carSpeed = DEFAULT_SPEED;  // can be set from 0 to 255, 0 being completely off, 255 completely on--Pulse Width Modulation (PWM); original program defaulted to 150, which is a little fast; 85 is about the minimum that will turn the wheels

//for ultrasonic rangefinder
byte numSamples = 5;

//-----------------------------------------------------------------------------------------
//setup
//-this function will run once each time the Arduino is turned on
//-"setup" is necessary for any Arduino program
//-----------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  myServo.attach(3); //attach servo on pin 3
  setUpUltrasonicRangeFinder();
  setUpCarMotors();
  carStop(); //Start your code off with the vehicle stopped
}


//-----------------------------------------------------------------------------------------
//loop
//-this function will execute over and over as long as the Arduino has power
//-"loop" is necessary for any Arduino program
//-----------------------------------------------------------------------------------------
void loop()
{
  //Now, either avoid obstacles and drive around, OR do something else of your choosing.
  //Just comment out unwanted lines of code as necessary 
  
  // avoidObstacles();
  
  //OR 
  //See if there are any obstacles ahead, and if there are (ex: if you put your hand in front of the robot), then do something 
  //carForward(carSpeed);
  myServo.write(90); //set servo to this angle (valid angles are 0 to 180 deg)
  newDelay(50); //ms delay 
  unsigned int distance = getUltrasonicDistance(numSamples); //mm 
  Serial.print("dist (mm) = "); Serial.println(distance);
  //if the ultrasonic rangefinder determines distance is less than __ mm, do the following
  if (distance <= 200) 
  {
    carStop();

    makeTheRobotShakeItsHeadNo();
    //OR 
    //doADanceMove();
    newDelay(1000);
    
    //carForward(carSpeed);
  } 
  else //(distance > __)
  {
	  Serial.println("continuing forward");  
  }
} //end of loop 


//=========================================================================================
//"Secondary" functions
// - These are any special functions you need for your own functionality of your bot
// - The only functions required by Arduino are "setup" and "loop"; all functions below are 
//   custom-made to make it easier to do repetitive things
//=========================================================================================

//-----------------------------------------------------------------------------------------
//makeTheRobotShakeItsHeadNo
//-This function will make it look like the robot is shaking its head "no", by wagging the 
// servo left and right 
//-----------------------------------------------------------------------------------------
void makeTheRobotShakeItsHeadNo() 
{
  //first, store the current servo angle into a variable 
  byte savedAngle = myServo.read(); //deg; read current angle 
  //now wag back and forth 2 times 
  myServo.write(0); //deg 
  newDelay(200); //ms 
  myServo.write(180); 
  newDelay(200);
  myServo.write(0);
  newDelay(200);
  myServo.write(180);
  newDelay(1000);
  //now set the angle back to what it was when you started 
  myServo.write(savedAngle);
}

//-----------------------------------------------------------------------------------------
//doADanceMove
//-This function will make the robot dance around in the way I tell it to
//-----------------------------------------------------------------------------------------
void doADanceMove() 
{
  //sweep head from right to left
  for (int i = 0; i <= 180; i = i + 10) 
  {
    myServo.write(i); //set this angle (in deg, from 0 to 180)
    newDelay(30);
  }
  
  //sweep head from left to right
  for (int i = 180; i >= 0; i = i - 10) 
  {
    myServo.write(i); //set this angle (in deg, from 0 to 180)
    newDelay(30);
  }
  
  //turn head back forward
  myServo.write(90);

  //pivot to the left then right, 4 times
  for (int i = 0; i < 4; i++) 
  {
    carLeft(carSpeed);
    newDelay(600);
    carRight(carSpeed);
    newDelay(400);
  }

  //spin in circles to the left, at full speed, with head turned right
  myServo.write(180);
  carLeft(255);
  newDelay(2000);
  carStop();
  newDelay(200);

  //spin in circles to the right, at full speed, head turned left
  myServo.write(0);
  carRight(255);
  newDelay(3000);

  //stop the robot (so it doesn't get dizzy continuing spinning to the right)
  carStop();

  //shake the robot's head a little like it's dizzy
  myServo.write(45);  //turn the robot head to 45 degrees (from far right)
  newDelay(300);
  myServo.write(135); //turn the robot head to 135 degrees (from far right)
  newDelay(300);
  myServo.write(45);
  newDelay(300);
  myServo.write(135);
  newDelay(300);
  myServo.write(90); //point straight forward again 
}

//-----------------------------------------------------------------------------------------
//avoidObstacles
//-AKA: goStraightOrLeftOrRight
//-This function checks which direction has an obstacle that is further away and turns in that direction
//-it checks left, forward, and right, then makes a decision and drives in that direction 
//-----------------------------------------------------------------------------------------
void avoidObstacles() 
{
  //look left and measure distance 
  myServo.write(180);
  newDelay(1500);
  unsigned int leftDistance = getUltrasonicDistance(numSamples); //mm
  
  //look forward and measure distance 
  myServo.write(90);
  newDelay(1500);
  unsigned int middleDistance = getUltrasonicDistance(numSamples); //mm
  
  //look right and measure distance 
  myServo.write(0);
  newDelay(1500);
  unsigned int rightDistance = getUltrasonicDistance(numSamples); //mm 

  //look forward again 
  myServo.write(90);
  newDelay(1500);
  
  //now find the maximum distance and go in that direction, unless it is too close, in which case we should back up 
  
  //macro-defined constants for use below 
  #define LEFT 0
  #define MIDDLE 1
  #define RIGHT 2
  
  //first, find the max distance and which direction it is in 
  unsigned int distArray[] = {leftDistance, middleDistance, rightDistance}; //mm 
  byte distArrayLen = sizeof(distArray)/sizeof(unsigned int);
  unsigned int maxDist = 0; //mm 
  byte maxDist_i; //array index of where maxDist is found 
  for (byte i=0; i<distArrayLen; i++)
  {
    if (maxDist<distArray[i])
    {
      maxDist = distArray[i];
      maxDist_i = i;
    }
  }
  
  //print the maxDist and its direction 
  //-see here for how to use a switch case: https://www.arduino.cc/en/Reference/SwitchCase
  switch (maxDist_i)
  {
    case LEFT:
      Serial.print("maxDist is to the LEFT at "); 
    case MIDDLE:
      Serial.print("maxDist is STRAIGHT AHEAD at ");
    case RIGHT:
      Serial.print("maxDist is to the RIGHT at ");
  }
  Serial.print(maxDist); Serial.println(" mm.");
  
  //now either go in that direction, or back up if necessary 
  //if too close (closer than ___mm in all directions), just back up 
  if (maxDist<=200)
  {
    carBack(carSpeed);
    newDelay(180);
  }
  //if maxDist is to the left, turn left 
  else if (maxDist_i==LEFT)
  {
    carLeft(carSpeed);
    newDelay(360);
  }
  //if maxDist is to the right, turn right 
  else if (maxDist_i==RIGHT)
  {
    carRight(carSpeed);
    newDelay(360);
  }
  //else maxDist is straight ahead, so don't do anything 
  
  //now continue forward 
  carForward(carSpeed);
  newDelay(180);
} //end of avoidObstacles 

//-----------------------------------------------------------------------------------------
//newDelay
//-this delay function is still blocking, but it allows background work, such as sampling 
// from the ultrasonic range-finder, to still occur *while* delaying!
//-----------------------------------------------------------------------------------------
void newDelay(unsigned long delay_ms)
{
  unsigned long tStart = millis(); //ms
  unsigned long tNow = tStart; //ms
  while (tNow - tStart < delay_ms)
  {
    // getUltrasonicDistance(); //keep the Ultrasonic rangefinder sampling in the background
    tNow = millis(); //ms; update
  }
}


