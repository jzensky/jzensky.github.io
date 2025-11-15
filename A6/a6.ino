// Code for joystick pulls from https://arduinogetstarted.com/tutorials/arduino-joystick
// Code for averaging pulls from https://brodycyphers.wordpress.com/2021/11/09/simple-game-with-p5-js/ 
// Code for LCD setup from https://docs.arduino.cc/learn/electronics/lcd-displays/ 
// LCD custom characters from https://deepbluembedded.com/lcd-custom-character-arduino/ 

// Include library for LCD screen
#include <LiquidCrystal.h>

// initialize library by associating LCD interface pins with respective arduino pin number
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// define LCD custom characters
uint8_t SmileyFaceChar[] = {0x00, 0x00, 0x0a, 0x00, 0x1f, 0x11, 0x0e, 0x00};
uint8_t HeartChar[] = {0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00};

// define constant variables (unchanging) for Arduino pins connected to joystick inputs
const int VRx = A0; 
const int VRy = A1;
const int SW = 9;

// Set number of readings to average (unchanging)
const int numReadings = 3;

// Variables to enable averaging
int Xreadings[numReadings]; // array of readings from the analog input
int XreadIndex = 0;         // the index of the current reading
int Xtotal = 0;             // the running total
int Xaverage = 0;           // the average

int Yreadings[numReadings]; // array of readings from the analog input
int YreadIndex = 0;         // the index of the current reading
int Ytotal = 0;             // the running total
int Yaverage = 0;           // the average

// Variable to recieve and store score
int score;

void setup() {
  // begin Serial communication
  Serial.begin(9600);

  // set PinModes for joystick inputs
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // set up array for average readings to clear jitter
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Xreadings[thisReading] = 0;
  }

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Yreadings[thisReading] = 0;
  }

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // send the custom characters to LCD's CGRAM
  lcd.createChar(0, HeartChar);
  lcd.createChar(1, SmileyFaceChar);
  // clear LCD
  lcd.clear();
  // prints a message to the LCD.
  lcd.print("snake game!");
  lcd.write(byte(0));
  lcd.write(byte(1));
}

void loop() {
  // read and store the state of the joystick switch
  int switchState = digitalRead(SW);

  // use averaging function, first for x-axis
  Xtotal -= Xreadings[XreadIndex]; // subtract the last reading
  Xreadings[XreadIndex] = analogRead(VRx); // read x value from joystick and add to x reading array
  Xtotal += Xreadings[XreadIndex]; // add reading to total
  XreadIndex++; // go to next reading in array

  // wrap to beginning of array when at end
  if (XreadIndex >= numReadings) {
    XreadIndex = 0;
  }

  // calculate x average
  Xaverage = Xtotal / numReadings;

  // continue to use averaging function from setup, now for y-axis
  Ytotal -= Yreadings[YreadIndex]; // subtract the last reading
  Yreadings[YreadIndex] = analogRead(VRy); // read y value from joystick and add to y reading array
  Ytotal += Yreadings[YreadIndex]; // add reading to total
  YreadIndex++; // go to next reading in array

  // wrap to beginning of array when at end
  if (YreadIndex >= numReadings) {
    YreadIndex = 0;
  }

  // calculate y average
  Yaverage = Ytotal / numReadings;

  // delay in between reads for stability
  delay(100); 

  // send joystick values
  Serial.print(Xaverage);
  Serial.print(",");
  Serial.print(Yaverage);
  Serial.print(",");
  Serial.println(switchState);

  // if serial data arrives
  if (Serial.available()) {
    // wait for the entire message to arrive
    delay(100);
    // read available data
    while (Serial.available() > 0) {
      // display data on the LCD
      score = int(Serial.read());
      lcd.setCursor(0, 1);
      lcd.print(score);
    }
  }
}

