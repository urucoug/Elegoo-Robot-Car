/*
eRCaGuy_GetMedian

AUTHOR:
By Gabriel Staples
Website: http://www.ElectricRCAircraftGuy.com
My contact info is available by clicking the "Contact Me" tab at the top of my website.
Library Written: 12 April 2016 
Library Last Updated: 12 April 2016

VERSIONING SYSTEM: 
-Using Semantic Versioning 2.0.0 (http://semver.org/)

Current Library Version: see History section below.

HISTORY (newest on top):
20160412 - v0.1.0 - first version 
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

#ifndef eRCaGuy_GetMedian_h
#define eRCaGuy_GetMedian_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

typedef unsigned int array_length_t;

class eRCaGuy_GetMedian
{  
  public:
    //class constructor
    eRCaGuy_GetMedian();
  
    //overloaded getMedian function; returns the median of an array 
    //NB: THE ARRAY IS SORTED IN PLACE, SO IF YOU DON'T WANT YOUR PRIMARY ARRAY SORTED, PASS IN A COPY OF IT INSTEAD 
    static byte getMedian(byte* data_array, array_length_t array_length);
    static int8_t getMedian(int8_t* data_array, array_length_t array_length);
    static unsigned int getMedian(unsigned int* data_array, array_length_t array_length);
    static int getMedian(int* data_array, array_length_t array_length);
    static unsigned long getMedian(unsigned long* data_array, array_length_t array_length);
    static long getMedian(long* data_array, array_length_t array_length);
    static float getMedian(float* data_array, array_length_t array_length);
  
  private:
    //compare functions for qsort 
    //NB: THE COMPARE FUNCTION PASSED TO QSORT MUST EITHER BE *NOT* IN A CLASS, *OR* STATIC. See here: http://stackoverflow.com/questions/26519318/qsort-comparison-compilation-error 
    static int compareBytes(const void* a, const void* b); 
    static int compareChars(const void* a, const void* b); 
    static int compareUnsignedInts(const void* a, const void* b); 
    static int compareInts(const void* a, const void* b); 
    static int compareUnsignedLongs(const void* a, const void* b); 
    static int compareLongs(const void* a, const void* b); 
    static int compareFloats(const void* a, const void* b); 
    
    static inline bool isEven(array_length_t num_in); //find out if an integer is even or odd; used to determine if array_length is even or odd, so the data types must match 
  
};

//external instance of this class, instantiated in .cpp file
extern eRCaGuy_GetMedian Median;

#endif