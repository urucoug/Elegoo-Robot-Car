/*
All code in this file is part of the "core functions," which you shouldn't have to touch
*/

/*
Electromagnet/H-bridge electrical setup (uses a 12V electromagnet; utilizes L298N dual h-bridge as the on-off switch that supplies more power):
 - Supply the 12V input on the H-bridge with the wire coming off the positive output of the boost converter
 - The negative terminal output wire from the boost converter should be connected to the negative terminal of the H-bridge
 - One wire from the electromagnet should go to Output1 on the H-bridge
 - The other wire from the electromagnet should go to ground on the H-bridge
 -- if the code below turns the electromagnet off when it should be on and on when it should be off, switch the wires on the electromagnet (aka, output 1 goes to ground, ground goes to output1)
 - IN1 on the H-bridge should be connected to Analog Pin A0 on the Arduino Nano (or whichever Arduino Nano pin is called out for HbridgeElectromagnetPin)
 
Ultrasonic Rangefinder ("ping" sensor) References & Learning:
 - https://tronixlabs.com.au/news/tutorial-l298n-dual-motor-controller-module-2a-and-arduino/
 - http://www.instructables.com/id/Arduino-Modules-L298N-Dual-H-Bridge-Motor-Controll/

*/

// Choose a pin on your Arduino to supply the 5V logic test to the H-bridge, which will turn on the electromagnet
static const int HbridgeElectromagnetPin = A0;	// Analog pin 0, which you could also call pin 14

void electromagnetOn() 
{
  //turn the elecromagnet on
  digitalWrite(HbridgeElectromagnetPin, HIGH);
}

void electromagnetOff() 
{
  //turn the elecromagnet off
  digitalWrite(HbridgeElectromagnetPin, LOW);
}
