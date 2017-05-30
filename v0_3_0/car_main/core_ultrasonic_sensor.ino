/*
All code in this file is part of the "core functions," which you shouldn't have to touch
*/

/*
Ultrasonic Rangefinder ("ping" sensor) References & Learning:
 - https://www.arduino.cc/en/tutorial/ping
 - https://www.arduino.cc/en/Reference/PulseIn
*/

//The ultrasonic rangefinder requires two pins: A4 and A5 (digital pins 18 and 19)
const byte ECHO_PIN = A4;
const byte TRIGGER_PIN = A5;

const float MICROSECONDS_PER_INCH = 74.6422; //us per inch; sound travelling through air at sea level ~GS

//-----------------------------------------------------------------------------------------
//setUpUltrasonicRangeFinder
//-----------------------------------------------------------------------------------------
void setUpUltrasonicRangeFinder()
{
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
}

//-----------------------------------------------------------------------------------------
//microsecondsToMm
//-----------------------------------------------------------------------------------------
unsigned int microsecondsToMm(unsigned long us)
{
  return (unsigned int)((float)us/MICROSECONDS_PER_INCH/2.0*25.4 + 0.5); //mm; distance; divide by 2 since the ultrasonic pressure wave must travel there AND back, which is 2x the distance you are measuring; do +0.5 to round to the nearest integer during truncation from float to unsigned int 
}

//-----------------------------------------------------------------------------------------
//getUltrasonicDistance
//-returns the distance to an object in front of the ultrasonic "ping" sensor, in mm 
//-----------------------------------------------------------------------------------------
unsigned int getUltrasonicDistance() 
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long pulseTime = pulseIn(ECHO_PIN, HIGH); //us; pulse time--ie: time it takes for the ultrasonic wave to travel there and back again 

  return microsecondsToMm(pulseTime); //mm; measured distance 
} 
