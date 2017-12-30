/*
ultrasonic_sensor_demo
-a demo code of how to read the ultrasonic sensor 

By Gabriel Staples
http://www.ElectricRCAircraftGuy.com 
-click "Contact me" at the top of my website to find my email address 
Started: 8 May 2017 
Updated: 9 June 2017 

Helpful References:
-https://www.arduino.cc/en/Tutorial/Smoothing

Required Libraries:
- http://playground.arduino.cc/Code/NewPing - this library works VERY WELL to read the ultrasonic sensors, but is UNNECESSARILY BLOCKING and UNNECESSARILY USES TIMER2. One of these days I'll fix this by writing my own library that relies on Pin Change interrupts instead, but DOESN'T USE ANY TIMERS and IS NOT BLOCKING. 
- https://github.com/ElectricRCAircraftGuy/eRCaGuy_GetMedian - self-explanatory name; is required to implement an external "moving window" type median filter 

*/

//Library includes:
#include <NewPing.h>
#include <eRCaGuy_GetMedian.h> //for finding median values 

//Debug defines 
#define SERIAL_PLOTTING_ON //comment out to turn serial plotting prints OFF

//Ultrasonic range-finder "ping" sensor 
const byte TRIGGER_PIN = A5;
const byte ECHO_PIN = A4;
const unsigned int MAX_DISTANCE = 500; //cm; max distance we want to ping for. Maximum sensor distance is rated at 400~500cm.
const float MICROSECONDS_PER_INCH = 74.6422; //us per inch; sound travelling through air at sea level ~GS
const unsigned int DESIRED_PING_PD = 39000; //us; this is the desired sample period; NB: THIS MUST *NOT* BE TOO SHORT, OR ELSE THE PING SENSOR READINGS BECOME *REALLY* UNSTABLE AND LOUSY! See the NewPing library, for instance, which uses a value of 29000us (29ms) (see v1.8 of NewPing.h, line 159) as their fixed sample period when doing their median filter! Also see this datasheet here, "C:\Gabe\Gabe - RC-extra\Arduino\Products (Hardware)\Sensors & Dataloggers (input only)\Distance\Ultrasonic Rangefinder\HC-SR04Users_Manual - Google Docs%%%%%.pdf", which shows in the diagram on pg. 7 that a pulse of 38ms corresponds to "no obstacle". Therefore, let's use a DESIRED_SAMPLE_PD just over 38ms (39ms is good). Note: p. 7 also says "Please make sure the surface of object to be detect should have at least 0.5 meter^2 for better performance." This corresponds to an object ~0.7 x 0.7 m *minimum* in order to provide an adequate reflection surface for best measurements. 
#define UNK_DIST NO_ECHO //alias for NO_ECHO, meaning the sensor is too far away from the measured surface 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //create NewPing object 

//-----------------------------------------------------------------------------------------
//setUpUltrasonicRangeFinder
//-----------------------------------------------------------------------------------------
void setUpUltrasonicRangeFinder()
{
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
}

//------------------------------------------------------------------------------
//getUltrasonicDistance()
//-returns the distance to an object in front of the ultrasonic "ping" sensor, in mm 
//-I recommend you use this call if your code is *blocking* (ex: you use "delay")
//-input the number of blocking samples you'd like to take; the value returned will be 
// the median value of this number of samples 
//------------------------------------------------------------------------------
unsigned int getUltrasonicDistance(byte numBlockingSamples)
{
  //Format: sonar.ping_median(iterations); - Do multiple pings (default=5), discard out of range pings and return median in microseconds; source: http://playground.arduino.cc/Code/NewPing
  unsigned int dist_mm = microsecondsToMm(sonar.ping_median(numBlockingSamples)); //mm 
  if (dist_mm==0) //dist is so far away it can't be read, so the newPing lib returns 0, so make it large instead 
    dist_mm = MAX_DISTANCE*10; //mm
  return dist_mm;
}

//------------------------------------------------------------------------------
//getUltrasonicDistance()
//-returns the distance to an object in front of the ultrasonic "ping" sensor, in mm 
//-I recommend you use this call only if your code is *non-blocking*
//-to use this call, since the function is overloaded, simply don't include an input parameter 
//------------------------------------------------------------------------------
unsigned int getUltrasonicDistance()
{
  //Do some basic Digital Signal Processing (DSP):
  //-Get a new distance reading as fast as possible, and load it into a "moving window" raw data buffer; we will then find the median of the data in that buffer and load the median value into a "moving window" median value buffer; lastly, we will come up with a moving average "smoothed" value (which now has increased resolution) by averaging all values in the median value buffer, and we will use that average as the value to output. 
  
  //Raw data sample buffer 
  const byte PING_RAW_BUF_LEN = 5; 
  static unsigned int pingRawBuffer[PING_RAW_BUF_LEN];
  //Median data sample buffer 
  const byte PING_MEDIAN_BUF_LEN = 5; 
  static unsigned int pingMedianBuffer[PING_MEDIAN_BUF_LEN];
  
  //Q: What's the median-filtered and smoothed (average-filtered) sample freq? A: For a sample freq of 1/39ms = 25.64 Hz, and a PING_RAW_BUF_LEN of 5 and a PING_MEDIAN_BUF_LEN of 5, we know it takes 9 samples to fully refresh the buffers. This is because the first 5 samples fully replace the pingRawBuffer, *and* we get a good median sample from only new data on that 5th sample, which becomes the first sample in pingMedianBuffer. That means we need 4 more in pingMedianBuffer to completely overwrite its data, so total samples to refresh all data = 5 + 4 = 9 samples. A raw sample rate of 25.64 Hz / 9 samples = 2.85 Hz *guaranteed* frequency response. However, the fact of the matter is that after only 3 good samples in a row, the median value will be one of the 3 new samples out of the 5 possible samples in the pingRawBuffer. Therefore, it is *possible* that we will have only new data comprising our new smoothed value by the time we get only 3 + 4 = 7 samples. This means our frequency response under ideal conditions is 25.64 Hz / 7 samples = 3.66 Hz at best--and as stated before: 2.85 Hz at worst. 
  // float freqResponse = (float)1e6/(float)DESIRED_PING_PD/(PING_RAW_BUF_LEN + PING_MEDIAN_BUF_LEN - 1); //Hz 
  
  //For smoothing the median samples 
  static byte pingMedBuf_i = 0; //pingMedianBuffer index 
  static unsigned long pingMedTotal = 0; //us; the running total of the median samples 
  static unsigned int pingMedAvg = 0; //us; the average of the median samples 
  static unsigned int dist_mm; //mm; distance obtained from pingMedAvg
  
  //Take raw samples no faster than the desired max sample freq
  static unsigned long tStartPing = micros(); //us; timestamp 
  unsigned long tNow = micros(); //us 
  unsigned long dt = tNow - tStartPing; //us 
  if (dt >= DESIRED_PING_PD)
  {
    tStartPing = tNow; //us; update 
    unsigned int tPing = sonar.ping(); //us; ping time; note: a 0 returned means the distance is outside of the set distance range (MAX_DISTANCE)
    
    //initialize the buffers
    static bool buffersInitialized = false;
    if (buffersInitialized==false)
    {
      buffersInitialized = true; //update 
      for (byte i=0; i<PING_RAW_BUF_LEN; i++)
        pingRawBuffer[i] = tPing;
      for (byte i=0; i<PING_MEDIAN_BUF_LEN; i++)
        pingMedianBuffer[i] = tPing;
    }
    
    //load new value into moving window pingRawBuffer 
    static byte pingBuf_i = 0; //pingRawBuffer index 
    pingRawBuffer[pingBuf_i] = tPing; //us 
    pingBuf_i++;
    if (pingBuf_i>=PING_RAW_BUF_LEN)
      pingBuf_i = 0; //reset 
    
    //find the median and do the data smoothing
    
    //copy pingRawBuffer, sort the copy in place, & find the median
    unsigned int pingRawBuffer_cpy[PING_RAW_BUF_LEN];
    for (byte i=0; i<PING_RAW_BUF_LEN; i++)
    {
      pingRawBuffer_cpy[i] = pingRawBuffer[i];
    }
    unsigned int pingMedian = Median.getMedian(pingRawBuffer_cpy, PING_RAW_BUF_LEN); //us; median ping time

    //median data smoothing (moving average window) (see: https://www.arduino.cc/en/Tutorial/Smoothing)
    pingMedTotal -= pingMedianBuffer[pingMedBuf_i]; //subtract the last reading 
    pingMedianBuffer[pingMedBuf_i] = pingMedian; //us; load a new reading into the buffer 
    pingMedTotal += pingMedianBuffer[pingMedBuf_i]; //add the reading to the total 
    //advance to the next position in the array 
    pingMedBuf_i++; 
    if (pingMedBuf_i>=PING_MEDIAN_BUF_LEN)
      pingMedBuf_i = 0; //reset 
    //calculate the average median 
    pingMedAvg = pingMedTotal/PING_MEDIAN_BUF_LEN; //us; average of the values in the median buffer
    
    //Convert the us avg to mm distance 
    //unsigned int dist_mm = microsecondsToMm(pingMedAvg);
    dist_mm = microsecondsToMm(pingMedAvg);
    if (dist_mm==0) //dist is so far away it can't be read, so the newPing lib returns 0, so make it large instead 
      dist_mm = MAX_DISTANCE*10; //mm
    
    #ifdef SERIAL_PLOTTING_ON
    //For Serial Plotting 
    Serial.print(microsecondsToMm(tPing)); Serial.print(", "); //mm; raw sample 
    Serial.print(microsecondsToMm(pingMedian)); Serial.print(", "); //mm; median sample 
    Serial.println(dist_mm); //mm; avg median sample 
    #endif 
  }
  
  Serial.print("Returning "); Serial.print(dist_mm); Serial.println(" mm");
  
  return dist_mm; //mm 
}

//------------------------------------------------------------------------------
//microsecondsToMm
//------------------------------------------------------------------------------
unsigned int microsecondsToMm(unsigned long us)
{
  return (unsigned int)((float)us/MICROSECONDS_PER_INCH/2.0*25.4 + 0.5); //mm; distance; divide by 2 since the ultrasonic pressure wave must travel there AND back, which is 2x the distance you are measuring; do +0.5 to round to the nearest integer during truncation from float to unsigned int 
}
