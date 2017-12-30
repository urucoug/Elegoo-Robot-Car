//
// This is prototype robot functionality--meant to simulate the robot actions using minimal 
//   components before I've received my final hardware
//
// Kevin Nufer
// Creation date: 13 Dec 2017
// Last updated: 19 Dec 2017
//

#include <eRCaGuy_EventReader.h>
#include <Servo.h>

// all the way right is 0; all the way left is 120
// needs to start in the middle and go right
int shooterServo[4] = {90, 82, 70, 53};
int shootIndex = 0;     // 0 is ready to shoot three rubber bands; 3 is unloaded
Servo myServo;

// Button
//Pins:
const byte buttonPin = 5; //the number of the pushbutton pin
//Define Button States:
const boolean BUTTON_PRESSED = LOW; //if using a pull-up resistor (ex: INPUT_PULLUP) on your button, the buttonPin will be LOW when the button is pressed; 
const boolean BUTTON_NOT_PRESSED = HIGH; //if using a pull-up resistor (ex: INPUT_PULLUP) on your button, the buttonPin will be HIGH when the button is NOT pressed

//instantiate an object of this library class; call it "reloadButton"
//if not explicitly stated, debounceDelay defaults to 50ms, and the _BUTTON_PRESSED value in the library defaults to LOW; see .h file for details
eRCaGuy_EventReader reloadButton(50, BUTTON_PRESSED); //object instantiation


void setup() {
  Serial.begin(115200);
  
  // default to starting robot with rubber bands loaded
  myServo.write(shooterServo[0]);
  
  // Set up button
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Show position of servo
  Serial.println(myServo.read());
}

void loop() {
  
  // determine if a shot should be taken
  if (shootIndex < 3) {
    fire();  
    Serial.println(myServo.read());
  } else {
    //Serial.println("Out of ammo, shoot logic loop");
  }
  
  // Button functionality
  checkButton();
}

void fire() {
  if (shootIndex < 3) {
    shootIndex++;
    Serial.print("Shot number "); Serial.println(shootIndex);
    myServo.write(shooterServo[shootIndex]);
    delay(1000);
  } else {
    Serial.println("Click...");
    //reload();
  }
}

void reload() {
  Serial.println("Giving 1.5 seconds between reload depress and shooting all rubber bands");
  delay(1500);
  myServo.write(shooterServo[3]);
  Serial.println("Waiting 5 seconds before starting to insert pin; giving chance to situate rubber bands first");
  delay(5000);
  insertSlow(shooterServo[3],shooterServo[2]);
  insertSlow(shooterServo[2],shooterServo[1]);
  insertSlow(shooterServo[1],shooterServo[0]);
  shootIndex = 0;
}

void insertSlow(int pos1, int pos2) {
  int inc = 3;            // change in servo angle per step
  int incTime = 700;      // time between servo steps (ms)

  if (pos1 > pos2) {
    while (pos1 > pos2) {
      if (pos1 - inc > pos2) {
        pos1 = pos1 - inc;
      } else {
        pos1 = pos1 - 1;
      }
      myServo.write(pos1);
      Serial.print(myServo.read()); Serial.print("; incrementing to "); Serial.println(pos2);
      delay(incTime);
    }
  } else if (pos1 < pos2) {
    while (pos1 < pos2) {
      if (pos1 + inc < pos2) {
        pos1 = pos1 + inc;
      } else {
        pos1 = pos1 + 1;
      }
      myServo.write(pos1);
      Serial.print(myServo.read()); Serial.print("; incrementing to "); Serial.println(pos2);
      delay(incTime);
    }
  } 
}


void checkButton() 
{  
  //read button; use the ampersands (&) to pass the address to (pointers to) the variables as input parameters, so that the variables can be updated by the function
  //Details:
    //read the button action, and store it into the button_action variable; and read the button state, & store it into the button_state variable
    //The button state can be 0 or 1, for LOW or HIGH, respectively
    // button action indicates what just happened to the button: 
    //	0 = no-change in true, debounced button state since last time reading the button, or debounceDelay time not yet elapsed
    //	1 = button was just pressed by a human operator (debounceDelay had elapsed)
    // -1 = button was just released by a human operator (debounceDelay had elapsed)
  byte buttonState = digitalRead(buttonPin);
  reloadButton.readEvent(buttonState);
  
  int8_t button_action = reloadButton.getDebouncedAction();
  boolean button_state = reloadButton.getDebouncedState();
  
  //Serial.print(F("Button state read = ")); Serial.print(buttonState); Serial.print(", ");Serial.print(F("Debounced state = ")); Serial.print(button_state);Serial.print(", ");Serial.print(F("Button Action = ")); Serial.println(button_action);
  
  //output data if a button press or release was detected, & reload if button is pressed
  if (button_action != 0) //if an action has occurred on the button
  {
    Serial.print(F("button_action = ")); Serial.print(button_action); Serial.print(", "); 
    if (button_action==1)
    {
      Serial.print(F("Button PRESS detected"));
    }
    else //button_action==-1
    {
      Serial.print(F("Button RELEASE detected"));
    }
    Serial.print(F("; button_state = ")); Serial.print(button_state); 
    Serial.print(F(", ")); 
    if (button_state==BUTTON_PRESSED)
    {
      Serial.println(F("BUTTON_PRESSED"));
    }
    else //button_state==BUTTON_NOT_PRESSED
    {
      Serial.println(F("BUTTON_NOT_PRESSED"));
    }
    
    //if the button is pressed, reload
    if (button_action==1) //if button just pressed
    {
      reload(); //turn the LED on if off, and off if on
    }
  }

  //THE BELOW SEGMENT OF CODE IS COMMENTED OUT FOR readButton.ino, but NOT for readButtonVerbose.ino
 else //button_action==0
 {
   //if there is no new action on the button, let's print out the button_state every 0.5 seconds, to see whether the button is currently NOT being pressed, or whether it is being held down continuously
   static unsigned long t_start = millis(); //ms; a time stamp to ensure printouts only happen every 0.5 seconds
   if (millis() - t_start >= 500) //if 500ms (0.5 sec) has elapsed
   {
     t_start = millis(); //ms; update
     if (button_state==BUTTON_PRESSED)
     {
       Serial.println(F("button is being held down"));
     }
     else //button_state==BUTTON_NOT_PRESSED
     {
       Serial.println(F("button is just sitting there, NOT pressed"));
     }
   }
 }
} //end of loop()