#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// LED matrix object
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

// variables associated with to ptm switch
const int switchPin = 2;
int switchState = 0;

// variables associated with potentiometer + smoothing
const int numReadings = 10;
double potReadings[numReadings];
int readIndex = 0;
int potTotal = 0;
int potAverage = 0;

//variables associated with time keeping
int state = 0;
unsigned long previousTime = 0;
int interval;

// variables determining display pattern 
// 4/4
const int x0[] = {0,0,4,4,0,0,4,4,0,0,4,4,0,0,4,4};
const int y0[] = {0,4,4,0,0,4,4,0,0,4,4,0,0,4,4,0};
const int z0[] = {2,2,2,2,3,3,3,3,1,1,1,1,0,0,0,0};
const int w0[] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
const int h0[] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

// 3/4
const int x1[] = {2,0,4,2,0,4,2,0,4,2,0,4};
const int y1[] = {0,4,4,0,4,4,0,4,4,0,4,4};
const int z1[] = {2,2,2,3,3,3,1,1,1,0,0,0};
const int w1[] = {4,4,4,4,4,4,4,4,4,4,4,4};
const int h1[] = {4,4,4,4,4,4,4,4,4,4,4,4};

// 6/8
const int x2[] = {0,0,4,4,0,0,0,0,4,4,0,0};
const int y2[] = {4,0,4,0,4,0,4,0,4,0,4,0};
const int z2[] = {1,1,1,3,3,3,2,2,2,0,0,0};
const int w2[] = {4,8,4,4,8,4,4,8,4,4,8,4};
const int h2[] = {4,4,4,4,4,4,4,4,4,4,4,4};

// pointers to current display pattern 
const int *xp = x0;
const int *yp = y0;
const int *zp = z0;
const int *wp = w0;
const int *hp = h0;

// number of iterations of display pattern
int stateLimit = 16;

// default and number of display patterns 
int TimeSig = 0;
int TimeSigLimit = 3;

//variables associated with piezo 
const int piezoPin = 8;
int modulo = 4; // accent the first beat of the bar 


void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");
  
  matrix.begin(0x70);  // pass in the address
  matrix.setRotation(3); // rotate so pins are at 0th row 
  
  pinMode(switchPin, INPUT);
  
  // initialise all pot readings to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    potReadings[thisReading] = 0;
  } 
}


void loop() {
  
  //potentiometer smoothing 
  potTotal = potTotal - potReadings[readIndex]; //subtract the last reading 
  potReadings[readIndex] = analogRead(A0); 
  potTotal = potTotal + potReadings[readIndex]; //add current reading to total 
  readIndex = readIndex + 1; //advance to next position in array 
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  potAverage = sqrt(potTotal/numReadings) * 100; //take the square root to get a non linear tempo response, multiply by 100 to increase the sensitivity of map (which can only handle integers). 
  delay(1); // delay in between reads for stability 

  // map the pot average to an interval (bpm = 1000/interval * 60). 
  interval = map(potAverage,0,3200,1500,200); //bpm can be set between 45bpm (1500 ms interval) and 300bpm (200ms interval). Must be mapped in reverse to encode non linear tempo mapping. 
  Serial.println(interval);

  //check whether the button has been pressed to change time sig 
  switchState = digitalRead(switchPin);
  if(switchState == HIGH){
    // move to next time signature
    TimeSig += 1; 
    // if the time signature limit is reached go back to start 
    if (TimeSig == TimeSigLimit){
      TimeSig = 0;
    }
    // update the display pattern based on the time signature 
    switch(TimeSig){
    // 4/4
    case 0:
    xp = x0;
    yp = y0;
    zp = z0;
    wp = w0;
    hp = h0;
    stateLimit = 16;
    modulo = 4;
    break;
    // 3/4
    case 1:
    xp = x1;
    yp = y1;
    zp = z1;
    wp = w1;
    hp = h1;
    stateLimit = 12;
    modulo = 3;
    break;
    // 6/8
    case 2:
    xp = x2;
    yp = y2;
    zp = z2;
    wp = w2;
    hp = h2;
    stateLimit = 12;
    modulo = 6;
    break;
    }  
    // reset the state counter 
    state = 0;
    // clear the display
    matrix.clear();
    matrix.writeDisplay();
    // delay to allow button time to unpress
    delay(250);
  }

  // update matrix with new pattern 
  matrix.fillRect(xp[state],yp[state],wp[state],hp[state],zp[state]);

  //how much time has passed since the last beat?
  unsigned long currentTime = millis();
  
  // if is time for the next beat
  if (currentTime - previousTime > interval){
    // reset time keeping
    previousTime = currentTime;
    // update matrix display
    matrix.writeDisplay();
    // if is the first beat of the bar play tone an octave higher (A at 880hz)
    if(state % modulo == 0){
      tone(piezoPin,880,20);
    }
    // else play an A at 440 hz
    else{
      tone(piezoPin,440,20);
    }
    // increase state
    state += 1; 
    // reset state to 0 once it has reached state limit
    if(state == stateLimit){
      state = 0;
    }
  }
}
