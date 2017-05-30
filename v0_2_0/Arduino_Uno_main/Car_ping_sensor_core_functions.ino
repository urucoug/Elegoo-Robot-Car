//
// All code in this file are the "core functions," which you shouldn't have to touch
//

// -------------------------------------------------------------------
// Car ping sensor "Core" functions
//  --> just one function so far; get distance in front of ping sensor
// -------------------------------------------------------------------

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
