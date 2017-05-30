/*
getMedian_example.ino
By Gabriel Staples
http://www.ElectricRCAircraftGuy.com 
My contact info is available by clicking the "Contact Me" tab at the top of my website.
Written: 12 April 2016 
Updated: 12 April 2016 

LICENSE: GNU GPLV3 or later (refer to .h file and attached license for details)
*/

#include <eRCaGuy_GetMedian.h>

void setup()
{
  Serial.begin(115200);
  
  //create some arrays to sort and from which to find the median 
  byte myByteArray[] = {1,52,23,6}; //(6 + 23)/2 = 14.5 --> 14 
  int8_t myCharArray[] = {-12,120,78}; //78
  unsigned int myUnsignedIntArray[] = {65200,23,23432,22300}; //(23432+22300)/2 = 22866
  int myIntArray[] = {-10234,23423,297,237,-13}; //237
  unsigned long myUnsignedLongArray[] = {43208732,232342323,932323290}; //232342323
  long myLongArray[] = {7,-879809273,8097,-200000,8000000}; //7
  float myFloatArray[] = {-234.2082, 20832.0823, -2767.4, 9888.2389}; //(9888.2389+-234.2082)/2 = 4827.01535 --> 4827.01562, due to only 4-byte float (vs 8-byte double) decimal precision error 
  
  //sort the arrays in place, and find the medians
  byte myByteMedian = Median.getMedian(myByteArray, sizeof(myByteArray)/sizeof(byte));
  int8_t myCharMedian = Median.getMedian(myCharArray, sizeof(myCharArray)/sizeof(int8_t));
  unsigned int myUnsignedIntMedian = Median.getMedian(myUnsignedIntArray, sizeof(myUnsignedIntArray)/sizeof(unsigned int));
  int myIntMedian = Median.getMedian(myIntArray, sizeof(myIntArray)/sizeof(int));
  unsigned long myUnsignedLongMedian = Median.getMedian(myUnsignedLongArray, sizeof(myUnsignedLongArray)/sizeof(unsigned long));
  long myLongMedian = Median.getMedian(myLongArray, sizeof(myLongArray)/sizeof(long));
  float myFloatMedian = Median.getMedian(myFloatArray, sizeof(myFloatArray)/sizeof(float));
  
  //print the results
  Serial.println(myByteMedian);
  Serial.println(myCharMedian);
  Serial.println(myUnsignedIntMedian);
  Serial.println(myIntMedian);
  Serial.println(myUnsignedLongMedian);
  Serial.println(myLongMedian);
  Serial.println(myFloatMedian,5); //5 decimal digits of precision 
}

void loop()
{

}


