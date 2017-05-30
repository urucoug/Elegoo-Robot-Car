/*
eRCaGuy_GetMedian

AUTHOR:
By Gabriel Staples
Website: http://www.ElectricRCAircraftGuy.com
My contact info is available by clicking the "Contact Me" tab at the top of my website.

FOR PERTINENT INFORMATION ABOUT THIS LIBRARY SEE THE TOP OF THE HEADER (.h) FILE, AND THE COMMENTS IN BOTH THIS FILE AND THE HEADER FILE.
*/

/*
===================================================================================================
  LICENSE & DISCLAIMER
  Copyright (C) 2016 Gabriel Staples.  All right reserved.
  
  This file is part of eRCaGuy_GetMedian.
  
  I AM WILLING TO DUAL-LICENSE THIS SOFTWARE--EX: BY SELLING YOU A SEPARATE COMMERICAL LICENSE FOR
  PRORPRIETARY USE. HOWEVER, UNLESS YOU HAVE PAID FOR AND RECEIVED A RECEIPT FOR AN ALTERNATE 
  LICENSE AGREEMENT, FROM ME, THE COPYRIGHT OWNER, THIS SOFTWARE IS LICENSED UNDER THE GNU GPLV3
  OR LATER, A COPY-LEFT LICENSE, AS FOLLOWS.
  
  NB: THE GNU GPLV3 LICENSE IS AN OPEN SOURCE LICENSE WHICH REQUIRES THAT ALL DERIVATIVE WORKS 
  YOU CREATE (IE: *ANY AND ALL* CODE YOU HAVE LINKING TO, BORROWING FROM, OR OTHERWISE USING THIS CODE) 
  ALSO BE RELEASED UNDER THE SAME LICENSE, AND BE OPEN-SOURCE FOR YOUR USERS AND/OR CUSTOMERS.
  FOR ALL STIPULATIONS AND LEGAL DETAILS, REFER TO THE FULL LICENSE AGREEMENT.
  
  ------------------------------------------------------------------------------------------------
  License: GNU General Public License Version 3 (GPLv3) - https://www.gnu.org/licenses/gpl.html
  ------------------------------------------------------------------------------------------------
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/
===================================================================================================
*/

/*
References:
qsort - http://www.cplusplus.com/reference/cstdlib/qsort/
pointers - http://www.cplusplus.com/doc/tutorial/pointers/
-this link also explains the meaning of void* pointers, as well as how to use pointers to functions!

For additional information on various sorting algorithms, see the following:
http://en.wikipedia.org/wiki/Quicksort
http://en.wikipedia.org/wiki/Introsort
http://en.wikipedia.org/wiki/Heapsort
*/

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "eRCaGuy_GetMedian.h"

//macros & defines:
#define M_LN2 0.69314718055994530942 //log_e 2; see: http://www.nongnu.org/avr-libc/user-manual/group__avr__math.html
#define EVEN true 
#define ODD false 
#define BEFORE (-1)
#define AFTER  (1)
#define EQUIVALENT (0)

eRCaGuy_GetMedian Median; //preinstantiation of object

/* //FOR EXTENSIVE COMMENTS & REFERENCE, SEE THIS EXAMPLE:
//-------------------------------------------------------------------------------------------------------------
//getMedian; works on an array of longs
//-You will have to modify these functions if using a data_array of another data type
//-IMPORTANT: the data_array passed in will be sorted in place, so be aware that once you call getMedian on a given array, the array will be modified in place and you will see that it is sorted next time you access it. Therefore, it is recommended to pass in a *copy* of your array to be sorted, so that your copy gets sorted, *not* your original. 
//-------------------------------------------------------------------------------------------------------------
long getMedian(long* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort
  //Format of qsort: qsort(pointer to array, number of elements in the array, number of bytes per element, compare function to compare two values);
  //-As written in the documentation: http://www.cplusplus.com/reference/cstdlib/qsort/
  //--void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*));
  //--qsort(data_array, sizeof(data_array)/sizeof(unsigned int), sizeof(unsigned int), compare); //this method does NOT work, since "sizeof(data_array)/sizeof(unsigned int)" always produces a 1 in this case, since data_array is a pointer, which takes up 2 bytes, or one unsigned int element, always.
  qsort(data_array, array_length, sizeof(long), compare); //this is the proper way to do it
  
  //Get median value
  long median;
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }
  
//  //Estimate & print out the # of times the compare function is called during sorting
//  //COMMENT OUT IF YOU DON'T WANT TO SEE THIS DATA
//  //Note (from qsort documentation): "Unspecified, but quicksorts are generally linearithmic in num, on average, calling compar approximately num*log2(num) times."
//  //this can be written as: num*log(num)/M_LN2, where M_LN2 is log(2)
//  unsigned int compare_count_estimated = array_length*log(array_length)/M_LN2;
//  Serial.print("compare_count_estimated = "); Serial.println(compare_count_estimated); Serial.println();
  
  return median;
} */

//--------------------------------------------------------------------------------------------------------
//define class constructor method
//--------------------------------------------------------------------------------------------------------
eRCaGuy_GetMedian::eRCaGuy_GetMedian()
{
  //empty constructor 
}

//-------------------------------------------------------------------------------------------------------------
//getMedian functions 
//-NB there are 5 places to change the data type in each of the overloaded functions; they are numbered below 
//-------------------------------------------------------------------------------------------------------------

//1 & 2 on next line 
byte eRCaGuy_GetMedian::getMedian(byte* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(byte), compareBytes); //3 & 4 
  
  //Get median value
  byte median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
int8_t eRCaGuy_GetMedian::getMedian(int8_t* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(int8_t), compareChars); //3 & 4 
  
  //Get median value
  int8_t median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
unsigned int eRCaGuy_GetMedian::getMedian(unsigned int* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(unsigned int), compareUnsignedInts); //3 & 4 
  
  //Get median value
  unsigned int median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
int eRCaGuy_GetMedian::getMedian(int* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(int), compareInts); //3 & 4 
  
  //Get median value
  int median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
unsigned long eRCaGuy_GetMedian::getMedian(unsigned long* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(unsigned long), compareUnsignedLongs); //3 & 4 
  
  //Get median value
  unsigned long median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
long eRCaGuy_GetMedian::getMedian(long* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(long), compareLongs); //3 & 4 
  
  //Get median value
  long median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = (data_array[array_length/2 - 1] + data_array[array_length/2])/2;
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }  
  return median;
}

//1 & 2 on next line  
float eRCaGuy_GetMedian::getMedian(float* data_array, array_length_t array_length)
{
  //Sort the data_array in place, using qsort; see here: http://www.cplusplus.com/reference/cstdlib/qsort/
  qsort(data_array, array_length, sizeof(float), compareFloats); //3 & 4 
  
  //Get median value
  float median; //5
  if (isEven(array_length)) //for arrays with an even number of elements, the median is the average of the two middle values
  {
    //Serial.print("even: median index = "); Serial.println(array_length/2 - 1); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = ((float)data_array[array_length/2 - 1] + (float)data_array[array_length/2])/2.0; //<--USE FLOAT NUMBERS IN AVERAGING  
  }
  else //for arrays with an odd number of elements, the median is the middle value
  {
    //Serial.print("odd: median index = "); Serial.println(array_length/2); //FOR DEBUGGING; COMMENT OUT WHEN DONE
    median = data_array[array_length/2];
  }
  return median;
}

//-------------------------------------------------------------------------------------------------------------
//compare functions for qsort
//-see here for help: http://www.cplusplus.com/reference/cstdlib/qsort/ 
//-------------------------------------------------------------------------------------------------------------

int eRCaGuy_GetMedian::compareBytes(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(uint8_t*)a < *(uint8_t*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(uint8_t*)a > *(uint8_t*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareChars(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(int8_t*)a < *(int8_t*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(int8_t*)a > *(int8_t*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareUnsignedInts(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(unsigned int*)a < *(unsigned int*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(unsigned int*)a > *(unsigned int*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareInts(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(int*)a < *(int*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(int*)a > *(int*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareUnsignedLongs(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(unsigned long*)a < *(unsigned long*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(unsigned long*)a > *(unsigned long*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareLongs(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(long*)a < *(long*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(long*)a > *(long*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

int eRCaGuy_GetMedian::compareFloats(const void* a, const void* b)
{
  //do the comparison of a to b
  if (*(float*)a < *(float*)b) //contents_of_a < contents_of_b
    return BEFORE; //The element pointed by a goes *before* the element pointed by b
  else if (*(float*)a > *(float*)b) //contents_of_a > contents_of_b
    return AFTER; //The element pointed by a goes *after* the element pointed by b
  else // (*(unsigned int*)a == *(unsigned int*)b) //contents_of_a == contents_of_b
    return EQUIVALENT; //The element pointed by a is *equivalent to* the element pointed by b
}

//-------------------------------------------------------------------------------------------------------------
//isEven
//-find out if num_in is even or odd
//-for even return true, odd return false 
//-------------------------------------------------------------------------------------------------------------
inline boolean eRCaGuy_GetMedian::isEven(array_length_t num_in)
{
  if (num_in % 2 == 0) //num_in is even
    return EVEN;
  else //if (num_in % 2 == 1) //num_in is odd
    return ODD;
}

