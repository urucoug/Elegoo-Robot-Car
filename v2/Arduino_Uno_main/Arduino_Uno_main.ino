// *****************************************************
// Robotics Merit Badge robot car
// Utilizes Elegoo Robot on Amazon: https://www.amazon.com/Elegoo-Ultrasonic-Bluetooth-Intelligent-Educational/dp/B01M2Z9ZTV/ref=as_li_ss_tl?s=aps&ie=UTF8&qid=1495592752&sr=1-1-catcorr&keywords=elegoo+robot+car&linkCode=ll1&tag=wwwel-20&linkId=e8867429e668b295e301b8e5722a127e
// Project by Kevin Nufer and Gabriel Staples
//
// Originally taken from Elegoo's "Obstacle Avoidance Car," but substantially modified
// www.elegoo.com, 2016.09.23
// *****************************************************

#include <Servo.h> // add the servo library, so we can use pre-made functions to control it
Servo myservo; // create servo object to control servo

// 
// Define variables to be used as pin numbers
//

//// The ultrasonic rangefinder requires two analog pins: A4 and A5 (pins 18 and 19)
int Echo = A4;  
int Trig = A5; 

//
// 6 pins control the DC motor H-bridge to tell the motors to go forward or backward and at what speed
//
// These 4 pins control the direction of the motors
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
// ENA is the pin which sends the signal for the speed of motor 1; ENB controls speed of motor 2
int ENA = 5;
int ENB = 11;
int ABS = 150;  // can be set from 0 to 255, 0 being completely off, 255 completely on--Pulse Width Modulation (PWM); original program defaulted to 150, which is a little fast; 85 is about the minimum that will turn the wheels 

// These varables are used in the original program to make a decision whether to go write or left
unsigned int rightDistance = 0;
unsigned int leftDistance = 0;
unsigned int middleDistance = 0;


// --------------------------------------------------------
// "setup" will run once each time the Arduino is turned on
// --> "setup" is necessary for any Arduino program
// --------------------------------------------------------
void setup() 
{ 
	Serial.begin(115200);     
  myservo.attach(3);// attach servo on pin 3 to servo object

	//
	// "Connect the hardware to the pins"; configure the pins so the hardware they represent works properly
	//
	
// Attach ultrasonic rangefinder
	pinMode(Echo, INPUT);    
	pinMode(Trig, OUTPUT);  

	// Assign pins for H-bridge that drives the DC motors that turn the wheels
	pinMode(in1,OUTPUT);
	pinMode(in2,OUTPUT);
	pinMode(in3,OUTPUT);
	pinMode(in4,OUTPUT);

	// Set the speed of the motors
	pinMode(ENA,OUTPUT);
	pinMode(ENB,OUTPUT);
	
	// Start your code off with the vehicle stopped
	_mStop();
} 


// ------------------------------------------------------------------
// "loop" will execute over and over as long as the Arduino has power
// --> "loop" is necessary for any Arduino program
// ------------------------------------------------------------------
void loop() 
{ 
    //
	  // See if there are any obstacles ahead
	  // 
	   _setServoAngle(myservo,90);
    delay(500);
    middleDistance = _distanceTest();
    Serial.println(middleDistance);
  
    // if the ultrasonic rangefinder determines distance is less than 20...
    if (middleDistance<=20) { 
		_mStop();
		_doADanceMove();
		//_makeTheRobotShakeItsHeadNo();
		//_chooseLeftOrRight();
		delay(1000);
	
	}  
    else {  // if the distance is not less than 20...
        //_mForward();                  
	}
}

//
// The only functions required by Arduino are "setup" and "loop"; all functions below are custom-made to make it easier to do repetitive things
//



// -------------------------------------------------------------------
// "Secondary" functions
// --> These are any special functions you need for your own functionality of your bot; probably call a lot of "core" functions here
//-------------------------------------------------------------------

// This function will make it look like the robot is shaking its head "no"
void _makeTheRobotShakeItsHeadNo(){
	_setServoAngle(myservo, 0);
	delay(200);
	_setServoAngle(myservo, 180);
	delay(200);
	_setServoAngle(myservo, 0);
	delay(200);
	_setServoAngle(myservo, 180);
	delay(200);
	_setServoAngle(myservo, 90);
}

// This function will make the robot dance around in the way I tell it to
void _doADanceMove() {
	// sweep head from right to left
	for (int i=0;i<=180;i=i+10) {
		_setServoAngle(myservo,i);
		delay(30);
	}
	// sweep head from left to right
	for (int i=180;i>=0;i=i-10) {
		_setServoAngle(myservo,i);		
		delay(30);
	}
	// turn head back forward
	_setServoAngle(myservo,90);
	
	// pivot to the left then right, 4 times
	for (int i=0; i<=3; i++) {
		_mLeft();
		delay(600);
		_mRight();
		delay(400);
	}

	// Turn up the speed on the motors to spin faster
	// First, remember what the speed was beforehand, so you can turn it back to the original speed later
	int rememberSpeed = ABS;
	// Next, change the speed to the fastest possible
	ABS = 255;
	// spin in circles to the left, head turned right
	_setServoAngle(myservo, 180);
	_mLeft();
	delay(2000);
	
	_mStop();
	delay(200);
	
	// spin in circles to the right, head turned left
	_setServoAngle(myservo, 0);
	_mRight();
	delay(3000);
	
	// stop the robot (so it doesn't get dizzy continuing spinning to the right)
	_mStop();
	
	// shake the robot's head a little like it's dizzy
	_setServoAngle(myservo, 45);  // turn the robot head to 45 degrees (from far right)
	delay(300);
	_setServoAngle(myservo, 135); // turn the robot head to 135 degrees (from far right)
	delay(300);
	_setServoAngle(myservo, 45);
	delay(300);
	_setServoAngle(myservo, 135);
	delay(300);
	_setServoAngle(myservo, 90);
	
	// Turn the robot back to its original speed
	ABS = rememberSpeed;
}

// This function checks which direction has an obstacle that is further away and turns in that direction
void _chooseLeftOrRight() {
		delay(500);
		_setServoAngle(myservo,0);         
		delay(1000);      
		rightDistance = _distanceTest();

		delay(500);
		_setServoAngle(myservo,90);             
		delay(1000);
		_setServoAngle(myservo,180);             
		delay(1000); 
		leftDistance = _distanceTest();

		delay(500);
		_setServoAngle(myservo,90);             
		delay(1000);
		if (rightDistance>leftDistance) {
			_mRight();
			delay(360);
		}
		else if (rightDistance<leftDistance) {
			_mLeft();
			delay(360);
		}
		else if ((rightDistance<=20)||(leftDistance<=20)) {
			_mBack();
			delay(180);
		}
		else {
			_mForward();
		}
}


