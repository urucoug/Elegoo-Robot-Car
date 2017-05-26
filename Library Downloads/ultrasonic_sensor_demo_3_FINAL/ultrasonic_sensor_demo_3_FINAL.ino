/*
ultrasonic_sensor_demo
-a demo code of how to read the ultrasonic sensor 

By Gabriel Staples
http://www.ElectricRCAircraftGuy.com 
-click "Contact me" at the top of my website to find my email address 
Started: 8 May 2017 
Updated: 8 May 2017 

Helpful References:
-https://www.arduino.cc/en/Tutorial/Smoothing

Required Libraries:
- http://playground.arduino.cc/Code/NewPing - this library works VERY WELL to read the ultrasonic sensors, but is UNNECESSARILY BLOCKING and UNNECESSARILY USES TIMER2. One of these days I'll fix this by writing my own library that relies on Pin Change interrupts instead, but DOESN'T USE ANY TIMERS and IS NOT BLOCKING. 
- https://github.com/ElectricRCAircraftGuy/eRCaGuy_GetMedian - self-explanatory name; is required to implement an external "moving window" type median filter 

*/

//------------------------------------------------------------------------------
//setup
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("\nBegin\n");
}

//------------------------------------------------------------------------------
//loop
//------------------------------------------------------------------------------
void loop()
{
  unsigned int dist_mm = getUltrasonicDistance(); //mm; distance returned by the ultrasonic distance sensor 
} //end of loop 

