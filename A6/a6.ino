// Code used from ArduinoGetStarted.com https://arduinogetstarted.com/tutorials/arduino-joystick
// Code used from https://brodycyphers.wordpress.com/2021/11/09/simple-game-with-p5-js/ 

// Define constant variables (unchanging) for Arduino pins connected to joystick inputs and LED output
const int VRx = A0; 
const int VRy = A1;
const int SW = 9;
const int LEDpin = 2;

// Set number of readings to average (unchanging)
const int numReadings = 3;

// Variables to enable and tracking averaging
int Xreadings[numReadings]; // array of readings from the analog input
int XreadIndex = 0;         // the index of the current reading
int Xtotal = 0;             // the running total
int Xaverage = 0;           // the average

int Yreadings[numReadings]; // array of readings from the analog input
int YreadIndex = 0;         // the index of the current reading
int Ytotal = 0;             // the running total
int Yaverage = 0;           // the average

int totalSwitchPresses = 0;

void setup() {
  // begin Serial communication
  Serial.begin(9600);

  // set PinModes for joystick inputs
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  // set PinModes for LED output
  pinMode(LEDpin, OUTPUT);

  // set up array to average readings to clear jitter
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Xreadings[thisReading] = 0;
  }

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Yreadings[thisReading] = 0;
  }
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

  // read raw values for debugging
  /* Xaverage = analogRead(VRx);
  Yaverage = analogRead(VRy); */

  // delay in between reads for stability
  //delay(50); 


  // Additional printing of values for debugging
 /* Serial.print("x0= ");
  Serial.print(Xreadings[0]); 
  Serial.print(", x1= ");
  Serial.print(Xreadings[1]);
  Serial.print(", x2= ");
  Serial.print(Xreadings[2]);
  Serial.print(", x3= ");
  Serial.print(Xreadings[3]);
  Serial.print(", numReadings =");
  Serial.print(numReadings);
  Serial.print(", xTotal= ");
  Serial.println(Xtotal);
 */ 


  // send values
  Serial.print(Xaverage);
  Serial.print(",");
  Serial.print(Yaverage);
  Serial.print(",");
  Serial.println(switchState);

  // if there's serial data
  if (Serial.available()) {
    totalSwitchPresses = Serial.read(); // read and store the data
  }

  // once totalSwitchPresses is greater than 5, turn on LED
  if (totalSwitchPresses > 5) {
    digitalWrite(LEDpin, HIGH);
  }

}