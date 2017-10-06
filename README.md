# Elegoo Robot Car  

By Kevin Nufer  
and Gabriel Staples (www.ElectricRCAircraftGuy.com / [contact me](http://www.electricrcaircraftguy.com/2013/01/contact-me.html))

Readme last updated: 29 May 2017

Purchase link: [Elegoo Robot Car for $73.99 on Amazon](https://www.amazon.com/Elegoo-Ultrasonic-Bluetooth-Intelligent-Educational/dp/B01M2Z9ZTV/ref=as_li_ss_tl?s=aps&ie=UTF8&qid=1495592752&sr=1-1-catcorr&keywords=elegoo+robot+car&linkCode=ll1&tag=wwwel-20&linkId=e8867429e668b295e301b8e5722a127e)  

This project was originally created to help Boy Scouts all create their own unique robots. All boys start with the generic Elegoo robot, and need to add their own custom subsystem. Code was written for this project to be able to more or less plug and play generic subsystem components (ultrasonic rangefinders, stepper motors, or servos so far).  v0_5_0 contains the latest of the generic code. Please note that there are only 4 pins not being used by the generic Elegoo robot Arduino uno. Since additional pins are needed, I had the boys purchase an Arduino Nano. So, you will note that there are two folders within v0_5_0--car_main and subsystem_main. car_main contains the Arduino Uno code (the Arduino which comes with the Elegoo robot), and subsystem_main is for the additional Arduino Nano, which is needed for the additional pins. The serial code for talking between the two Arduinos has not yet been implemented.

rubber_band_bot diverges from the generic code (from v0_4_0), and is my subsystem-specific implementation of a robot that can fire rubber bands. It's in progress, and additional changes, videos, CAD files, and calculations will be posted as the project progresses.
  
# Libraries needed: 

 - Accel Stepper  
   - http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html  
   - How to hook up: http://42bots.com/tutorials/28byj-48-stepper-motor-with-uln2003-driver-and-arduino-uno/  