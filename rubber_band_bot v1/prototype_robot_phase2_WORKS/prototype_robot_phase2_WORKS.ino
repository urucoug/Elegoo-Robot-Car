//
//  This is prototype robot functionality--meant to simulate the robot actions using minimal 
//    components as a portable way to advance my robot when I don't actually have my robot with me
//
//  Phase 1 of this effort sets servo positions for the rubber bands, introduces a reloading routine at 
//    the press of a button, and "fires" the rubber bands.
//
//  Phase 2 integrates an ultrasonic rangefinder and makes decisions on whether to fire and retreat or to
//    find a different path.
//
//  Components needed: Arduino, ultrasonic rangefinder, and push button
//
//  Schematic: Pin D5 is connected to a button; other side of button connected to ground
//    Ultrasonic rangefinder: VCC, Ground, Trig, and Echo connect to VIN, Ground, A5, and A4, respectively
//
//  The robot's main objective is to find targets worthy of shooting and shoot them with a rubber band. After 
//    each shot, the robot will wait 5 seconds, re-evaluate whether or not to shoot, then continue hunting for targets.
//    When the ammunition is expended, the robot will do a dance, then stand still and do nothing. The robot can be 
//    re-loaded by pressing a button (and physically putting the rubber bands on while the servo goes through the 
//    required motions. The criteria for shooting something is if the target, which is within a specified range, 
//    makes discernible movement during the time period of evaluation.
//
//  Kevin Nufer
//  Creation date: 13 Dec 2017
//  Last updated: 30 Dec 2017
//

#include <eRCaGuy_EventReader.h>
#include <Servo.h>

// -----------------------------
// Rubber band shooter variables
// -----------------------------
Servo rubberBandServo;
// all the way right is 0; all the way left is 120; needs to start towards the middle and go right; calibrate with actual servo later
int shooterServo[4] = {90, 82, 70, 53};
int shootIndex = 0; // 0 is ready to shoot three rubber bands; 3 is unloaded

// ----------------
// Button variables
// ----------------
const byte BUTTON_PIN = 5; //the number of the pushbutton pin
// Define Button States:
const boolean BUTTON_PRESSED = LOW; // if using a pull-up resistor (ex: INPUT_PULLUP) on your button, the BUTTON_PIN will be LOW when the button is pressed; 
const boolean BUTTON_NOT_PRESSED = HIGH; // if using a pull-up resistor (ex: INPUT_PULLUP) on your button, the BUTTON_PIN will be HIGH when the button is NOT pressed

// instantiate an object of eRCaGuy_EventReader library class; call it "reloadButton"
// if not explicitly stated, debounceDelay defaults to 50ms, and the _BUTTON_PRESSED value in the library defaults to LOW; see .h file for details
eRCaGuy_EventReader reloadButton(50, BUTTON_PRESSED); // object instantiation

// -------------------------------------------
// Rangefinder and hunting algorithm variables
// -------------------------------------------
const byte numSamples = 5;                                // Used in averaging function for ultrasonic rangefinder
const int HUNTING_RANGE_MM = 600;                // Threshold distance (mm) used to detect targets (1219 mm ~= 4 ft); if object is detected within this threshold, investigate and possibly shoot it
const int THRESHOLD_DETECTABLE_MOVE_MM = 150;    // Threshold distance (mm) target has to move to trigger shot fired (150 mm ~= 6 inches)

int sampleCount = 0;     // counts how many samples have been collected within Target State 1
const int TOTAL_SAMPLES = 12;
int sampleCheck1 = 0;    // 
int sampleCheck2 = 0;    // sampleChecks are used by targetState==0 and targetState==1 to determine validity of data

// For evaluation of targets by the ultrasonic rangefinder; used in huntTargets function
int targetState = 0;     // used in huntTargets(); sets robot's mode of operation
int targetPings[TOTAL_SAMPLES];     // ultrasonic rangefinder samples used by robot in huntTargets() to determine if object detected is an obstacle, target, or false alarm

void setup() {
  Serial.begin(115200);
  
  // Need the ultrasonic rangefinder .ino in same folder
  setUpUltrasonicRangeFinder();
  
  // default to assuming robot has rubber bands loaded
  rubberBandServo.write(shooterServo[0]);
  
  // Set up button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("Begin");
}

void loop() {
  // Check if any targets exist; if so, shoot them
  huntTargets();
  
  // Button functionality
  checkButton();
}

void fire() {
  if (shootIndex < 3) {
    shootIndex++;
    // Serial.print("Shot number "); Serial.println(shootIndex);
    rubberBandServo.write(shooterServo[shootIndex]);
    delay(1000);
    if (shootIndex < 3) {
      // if there is ammo left, put the robot on the hunt (see huntTargets)
      targetState = 0;
    } else {
      // if there is no ammo left, stop the robot and allow user to reload rubber bands
      targetState = 4;
    }
  } else {
    // shouldn't ever get here; shootIndex > 3 for this robot would be out of ammo; targetState should be assigned to 4 if out of ammo, which doesn't initiate fire()
  }
}

void reload() { 
  Serial.println("Giving 1.5 seconds between reload depress and shooting all rubber bands");
  delay(1500);
  rubberBandServo.write(shooterServo[3]);
  Serial.println("Waiting 5 seconds before starting to insert pin; giving chance to situate rubber bands first");
  delay(5000);
  insertSlow(shooterServo[3],shooterServo[2]);
  insertSlow(shooterServo[2],shooterServo[1]);
  insertSlow(shooterServo[1],shooterServo[0]);
  shootIndex = 0;
  targetState = 0;
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
      rubberBandServo.write(pos1);
      Serial.print(rubberBandServo.read()); Serial.print("; incrementing to "); Serial.println(pos2);
      delay(incTime);
    }
  } else if (pos1 < pos2) {
    while (pos1 < pos2) {
      if (pos1 + inc < pos2) {
        pos1 = pos1 + inc;
      } else {
        pos1 = pos1 + 1;
      }
      rubberBandServo.write(pos1);
      Serial.print(rubberBandServo.read()); Serial.print("; incrementing to "); Serial.println(pos2);
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
  byte buttonState = digitalRead(BUTTON_PIN);
  reloadButton.readEvent(buttonState);
  
  int8_t button_action = reloadButton.getDebouncedAction();
  boolean button_state = reloadButton.getDebouncedState();
  
  //Serial.print(F("Button state read = ")); Serial.print(buttonState); Serial.print(", ");Serial.print(F("Debounced state = ")); Serial.print(button_state);Serial.print(", ");Serial.print(F("Button Action = ")); Serial.println(button_action);
  
  //output data if a button press or release was detected, & reload if button is pressed
  if (button_action != 0) //if an action has occurred on the button
  {
    // Serial.print(F("button_action = ")); Serial.print(button_action); Serial.print(", "); 
    if (button_action==1)
    {
      // Serial.print(F("Button PRESS detected"));
    }
    else //button_action==-1
    {
      // Serial.print(F("Button RELEASE detected"));
    }
    // Serial.print(F("; button_state = ")); Serial.print(button_state); 
    // Serial.print(F(", ")); 
    if (button_state==BUTTON_PRESSED)
    {
      // Serial.println(F("BUTTON_PRESSED"));
    }
    else //button_state==BUTTON_NOT_PRESSED
    {
      // Serial.println(F("BUTTON_NOT_PRESSED"));
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
        // Serial.println(F("button is being held down"));
      }
      else //button_state==BUTTON_NOT_PRESSED
      {
        // Serial.println(F("button is just sitting there, NOT pressed"));
      }
    }
  }
} //end of loop()

//-----------------------------------------------------------------------------------------
//newDelay
//-this delay function is still blocking, but it allows background work, such as sampling 
// from the ultrasonic range-finder, to still occur *while* delaying!
//-----------------------------------------------------------------------------------------
void newDelay(unsigned long delay_ms)
{
  unsigned long tStart = millis(); //ms
  unsigned long tNow = tStart; //ms
  while (tNow - tStart < delay_ms)
  {
    getUltrasonicDistance(); //keep the Ultrasonic rangefinder sampling in the background
    tNow = millis(); //ms; update
  }
}

void huntTargets() {
  // Get distance in front of robot with ultrasonic rangefinder
  unsigned int distance = getUltrasonicDistance(numSamples); //mm 
  
  // Print "millis() distance targetState" to the Serial Plotter
  Serial.print(millis());Serial.print(" "); Serial.print(distance); Serial.print(" "); Serial.println(targetState);  
  
  // Possible targetStates:
  //    0: no target detected 
  //    1: potential target identified; undergoing evaluation period
  //    2: initiate firing/running away sequence
  //    3: cautious navigation around obstacle
  //    4: out of ammo
  if (targetState == 0) {
    // 
    //  Target State 0: 
    //    This is the state where no targets have been detected
    //    Actively searching for targets; if two consecutive samples are less than HUNTING_RANGE_MM, move 
    //    the robot state to Target State 1
    //
    // Serial.print("targetState = "); Serial.print(targetState); Serial.println("; No targets detected yet");
        
    // No target has been detected yet; check for targets
    if (distance <= HUNTING_RANGE_MM) {
      // If two consecutive samples are below HUNTING_RANGE_MM, send to Target State 1
      if (sampleCheck1 == 0) {
        sampleCheck1 = distance;
      } else if (sampleCheck2 == 0) {
        sampleCheck2 = distance;
        
        // Since both samples are below HUNTING_RANGE_MM, send robot to Target State 1
        targetState = 1;
        // Serial.println("Going to targetState==1 to decide if target is shootable");
        
        // These two samples that were used to to determine if the robot state should be set to Target State 1
        //  will be used in Target State 1 (Target State 1 collects 12 samples; these will be the first two)   
        sampleCount = 2;      // start counting how many samples have been collected
        targetPings[0] = sampleCheck1;
        targetPings[1] = sampleCheck2;
        
        // In Target State 1, sampleCheck1 and sampleCheck2 will be used for other purposes
        sampleCheck1 = 0;
        sampleCheck2 = 0;
        
        for (int i=2; i < TOTAL_SAMPLES; i++) {
          targetPings[i] = 0;
        }
      } else {
        // Serial.println("Error. If I'm in targetState==0, one of these samples should be blank");
      }
      
    } else {
      // Reset the samples to 0; don't have two consecutive samples below HUNTING_RANGE_MM
      sampleCheck1 = 0;
      sampleCheck2 = 0;
    }
    
  } else if (targetState == 1) {
    //
    //  Target State 1:
    //    This is the state where the ultrasonic rangefinder has apparently detected something. This could be
    //    an unmoving obstacle (which must continue to be approached, then navigated around--exit to Target State 3),
    //    a target (which must be shot by a rubber band--use fire() command), or a false alarm (where nothing 
    //    must happen--return to Target State 0)
    //
    //    To make the determination, twelve samples will be collected and analyzed, using the algorithm below
    //
    //  Algorithm: 
    //    **** Eliminating this criteria #1 for now; not sure how much value added it will be (30 Dec 2017) **** 1) 2 samples of the first 5 must be within +/- of the original value collected
    //    2) If 3 consecutive samples are above HUNTING_RANGE_MM, send back to Target State 0; assume instrumentation error
    //    3) Compare median of first 5 samples of targetPings[] to median last 5 samples; if the difference is greater than THRESHOLD_DETECTABLE_MOVE_MM threshold, then fire()
    //
    
    // If there are more samples to collect to get a complete dataset, collect the samples. If all samples have been collected, evaluate
    if (sampleCount < TOTAL_SAMPLES) {
      // Run algorithm #2
      if (distance > HUNTING_RANGE_MM) {
        if (sampleCheck1 == 0) {
          sampleCheck1 = distance;
        } else if (sampleCheck2 == 0) {
          sampleCheck2 = distance;
        } else {
          // There have been 3 consecutive samples greater than the HUNTING_RANGE_MM; kick the robot back to targetState=0
          targetState = 0;
          sampleCheck1 = 0;
          sampleCheck2 = 0;
        }
      } else {
        // If the sample is not greater than the HUNTING_RANGE_MM, then it fails the criteria of 3 consecutive samples above the range; reset the checks and continue the test
        sampleCheck1 = 0;
        sampleCheck2 = 0;
      }
      
      targetPings[sampleCount] = distance;
      sampleCount++;
    } else {
      // All samples have been collected. Evaluate using Algorithm #3
      
      // // Print targetPings array to the Serial Monitor for visual verification
      // Serial.print("targetPings: ");
      // for (int i=0; i < TOTAL_SAMPLES; i++) {
        // Serial.print(targetPings[i]); Serial.print(" ");
      // }
      // Serial.println();
      
      // Get the median of the first 5 samples and the last 5 samples
      int firstFive[] = {targetPings[0],targetPings[1],targetPings[2],targetPings[3],targetPings[4]};
      int lastFive[] = {targetPings[7],targetPings[8],targetPings[9],targetPings[10],targetPings[11]};
      int n = 5;
      int medianFirstSet = getMedian(firstFive,n);
      int medianLastSet = getMedian(lastFive,n);
      
      // Serial.print("From getMedian = "); Serial.println(medianFirstSet);
      // Serial.print("From getMedian = "); Serial.println(medianLastSet);
      // Serial.print("THRESHOLD_DETECTABLE_MOVE_MM = "); Serial.println(THRESHOLD_DETECTABLE_MOVE_MM);
      // Serial.print("Median difference = "); Serial.println(medianFirstSet-medianLastSet);
      
      if (abs(medianFirstSet - medianLastSet) > THRESHOLD_DETECTABLE_MOVE_MM) {
        targetState = 2;
        Serial.println("Fire in the hole!");
        sampleCheck1 = 0;
        sampleCheck2 = 0;
      } else {
        // Median values too close. Assumption is that the "target" is not moving, and may be an obstacle. targetState = 3
        targetState = 3;
        sampleCheck1 = 0;
        sampleCheck2 = 0;
        Serial.println("Re-assigning to targetState = 3. Assuming obstacle");
      }
    }
    
    
        
  } else if (targetState == 2) {
    //
    //  Target State 2:
    //    Determination has been made in Target State 1 that the target must be shot with a rubber band. 
    //    Execute fire() command. This exists as an entire state for troubleshooting and expansion of the program
    //    down the line
    //
    
    fire();
    
    // Serial.print("targetState = "); Serial.print(targetState); Serial.println("; Firing and running away");
  } else if (targetState == 3) {
    //
    //  Target State 3:
    //    Target State 1 made determination that data signature seen was not instrumentation error and it was not
    //    a target. It is assumed to be a stationary object that must be navigated around.
    //
    //    For this release, assume that if 3 consecutive samples are taken outside HUNTING_RANGE_MM, that the
    //    robot has successfully navigated around the obstacle, and it should be re-assigned to targetState 0 to
    //    continue searching for targets
    //
    //    Future releases may include the assumption that the target is waiting to spring out and get
    //    the robot (so be ready to fire), but that will not be included in this release
    //
    
    // If three consecutive samples are above HUNTING_RANGE_MM, assign targetState = 0; otherwise, continue 
    //  navigating under the assumption that the object that is within HUNTING_RANGE_MM is an obstacle, not a target
    if (distance > HUNTING_RANGE_MM) {
      if (sampleCheck1 == 0) {
        sampleCheck1 = distance;
      } else if (sampleCheck2 == 0) {
        sampleCheck2 = distance;
      } else {
        // There have been 3 consecutive samples greater than the HUNTING_RANGE_MM; assume the robot has cleared the obstacle, and return to targetState=0
        targetState = 0;
        sampleCheck1 = 0;
        sampleCheck2 = 0;
        Serial.println("Three consecutive samples above HUNTING_RANGE_MM. Assuming obstacle is cleared. Re-directing to Target State 0");
      }
    } else {
      // If the sample is not greater than the HUNTING_RANGE_MM, then continue navigating under Target State 3; reset, and continue searching for three consecutive samples above HUNTING_RANGE_MM
      sampleCheck1 = 0;
      sampleCheck2 = 0;
    }
    
  } else if (targetState == 4) {
    //
    //  Target State 4:
    //    All ammo has been expended. Do a victory dance and then stop, patiently waiting for human to bring some
    //    new rubber bands over and reload
    //
    
    // Serial.print("targetState = "); Serial.print(targetState); Serial.println("; Doing a dance and stopping");
    delay(1000);
    
    // do a dance
    // [placeholder for dance code]
    
    // For now, automatically reload; after everything seems to be working, wait for the button push
    reload();    
  }
}

// Function is utilized getMedian(). Swaps values stored at integer addresses
void swap(int *p,int *q) {
  int t;

  t=*p; 
  *p=*q; 
  *q=t;
}

int getMedian(int array[], int arraySize) {
  int i,j,temp;
  int medianLocation = (arraySize+1) / 2 - 1;  // -1 as array indexing in C starts from 0
  
  for(i=0;i<arraySize-1;i++) {
    for(j=0;j<arraySize-i-1;j++) {
      if(array[j]>array[j+1])
        swap(&array[j],&array[j+1]);
    }
  }
  
  return array[medianLocation];
}