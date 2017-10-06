/*
All code in this file is part of the "core functions," which you shouldn't have to touch
*/

/*
Drive "Core" functions
 - this file contains functions to help the car drive forward, backwards, rotate to left or right, and stop
*/

//Debug defines 
#define SERIAL_PLOTTING_ON //comment out to turn serial plotting prints OFF

//motor pins
// - there are 4x DC motors:
// - 6 pins control the DC motor H-bridges (1 H-bridge per pair of motors) to tell the motors to go forward or backward, and at what speed

//These 4 pins directly control the 4 outputs of the H-bridges (2 outputs per H-bridge), which in turn controls the direction of the motors
const byte IN1_PIN = 6;
const byte IN2_PIN = 7;
const byte IN3_PIN = 8;
const byte IN4_PIN = 9;
//These 2 pins enable each H-bridge (ie: the 2 outputs from H-bridge A, or the 2 outputs from H-bridge B) 
// - ENA_PIN can be used to control the speed of motor 1
// - ENB_PIN can be used to control the speed of motor 2
const byte ENA_PIN = 5; //enable pin for H-bridge connected to motor A
const byte ENB_PIN = 11; //enable pin for H-bridge connected to motor B

const byte DEFAULT_SPEED = 110; //PWM value; can be 0-255, inclusive 

//-----------------------------------------------------------------------------------------
//setUpCarMotors
//-prepares the pins for usage as OUTPUTS to control the H-bridges which control the car's motors 
//-----------------------------------------------------------------------------------------
void setUpCarMotors()
{
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
}

//-----------------------------------------------------------------------------------------
//carForward
// - drive the car forward at the specified speed (PWM value 0 to 255)
// - This will make the robot continue going forwards until a different command is given involving the motors (stop, forward, left, right, or back up)
//-----------------------------------------------------------------------------------------
void carForward(int speed)
{
  //constrain speed to valid values
  speed = constrain(speed, 0, 255); //PWM speed value
  
  //command all H-bridge pins 
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  
  #ifdef SERIAL_PLOTTING_ON
    //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
    Serial.print("going forward; speed = "); Serial.println(speed);
  #endif
}

//-----------------------------------------------------------------------------------------
//carBack
// - drive the car backward at the specified speed (PWM value 0 to 255)
// - This will make the robot continue going forwards until a different command is given involving the motors (stop, forward, left, right, or back up)
//-----------------------------------------------------------------------------------------
void carBack(int speed)
{
  //constrain speed to valid values
  speed = constrain(speed, 0, 255); //PWM speed value
  
  //command all H-bridge pins 
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  
  #ifdef SERIAL_PLOTTING_ON
    //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
    Serial.print("going backward; speed = "); Serial.println(speed);
  #endif
}

//-----------------------------------------------------------------------------------------
//carLeft
// - turn the car left at the specified speed (PWM value 0 to 255)
// - This will make the robot continue going forwards until a different command is given involving the motors (stop, forward, left, right, or back up)
//-----------------------------------------------------------------------------------------
void carLeft(int speed)
{
  //constrain speed to valid values
  speed = constrain(speed, 0, 255); //PWM speed value
  
  //command all H-bridge pins 
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW); 
  
  #ifdef SERIAL_PLOTTING_ON
    //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
    Serial.print("going left; speed = "); Serial.println(speed);
  #endif
}

//-----------------------------------------------------------------------------------------
//carRight
// - turn the car right at the specified speed (PWM value 0 to 255)
// - This will make the robot continue going forwards until a different command is given involving the motors (stop, forward, left, right, or back up)
//-----------------------------------------------------------------------------------------
void carRight(int speed)
{
  //constrain speed to valid values
  speed = constrain(speed, 0, 255); //PWM speed value
  
  //command all H-bridge pins 
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  
  #ifdef SERIAL_PLOTTING_ON
    //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
    Serial.print("going right; speed = "); Serial.println(speed);
  #endif
}

//-----------------------------------------------------------------------------------------
//carStop
// - stop the robot car 
//-----------------------------------------------------------------------------------------
void carStop()
{
  digitalWrite(ENA_PIN, LOW);
  digitalWrite(ENB_PIN, LOW);
  
  #ifdef SERIAL_PLOTTING_ON
    //this prints to the serial monitor; not necessary for functionality of code; if you want to see what it does, open up 'Tools > Serial Monitor' and leave it open while your program is running
    Serial.println("stopping"); 
  #endif
}





