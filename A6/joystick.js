// Global variables!
const BAUD_RATE = 9600; // this should match the baud rate in your Arduino sketch
let port, connectBtn;   // these are used for setting up the serial connection


let xVal;            // joystick X value from Arduino
let yVal;            // joystick Y value from Arduino
let switchVal;       // joystick switch value from Arduino
let prevSwitchVal;   // previous joystick switch value from Arduino
let totalSwitchPresses = 0; // total number of times the joystick button has been pressed

function setup() {
  setupSerial(); // Run our serial setup function (below)

  // Create a canvas that is the size of our browser window.
  // windowWidth and windowHeight are p5 variables
  createCanvas(windowWidth, windowHeight);

  // p5 text settings. BOLD and CENTER are constants provided by p5.
  // See the "Typography" section in the p5 reference: https://p5js.org/reference/
  textFont("system-ui", 50);
  textStyle(BOLD);
  textAlign(CENTER, CENTER);
}


function draw() {
  background(0);  // draw a black background
  receiveData();  // read any data coming in from Arduino
  if (switchVal != prevSwitchVal){
    totalSwitchPresses++;  // increment total switch presses if switch value has changed
    prevSwitchVal = switchVal; // update previous switch value
  }
  displayHUD();   // a HUD, or 'heads up display', is for displaying important values.
                  // it's not strictly necessary, feel free to comment it out.
  drawGUI();      // display joystick values and total movements on screen
  sendData();     // send data out to the Arduino
}

function drawGUI() {
  /**
   * Draw a simple GUI that the user can interact with using the joystick.
   * Display joystick x, y, and switch state.
   * Also, add up total movements/interactions and send back to Arduino.
   */
  textSize(40);
    fill(255);
    text(`Joystick X: ${xVal}`, width / 2, height / 4);
    text(`Joystick Y: ${yVal}`, width / 2, height / 4 + 50);
    text(`Switch: ${switchVal}`, width / 2, height / 4 + 100);
    text(`Total Switch Presses: ${totalSwitchPresses}`, width / 2, height / 4 + 150);
}


function displayHUD() {
  /**
   * Make a "Heads Up Display" (HUD) that displays important values clearly to the user
   * This isn't strictly necessary, just useful for debugging
   */
  fill(255, 0, 80);
  textSize(22);
  text(`Joystick X: ${xVal}`, 200, 100);
  text(`Joystick Y: ${yVal}`, 200, 150);
  text(`Switch: ${switchVal}`, 200, 200);
  text(`Total Switch Presses: ${totalSwitchPresses}`, 200, 250);
}

function receiveData() {
  /**
   * Receive data over serial from your Arduino
   * We're terminating data with a newline character here
   * i.e., we need to Serial.println() in our Arduino code
   */
  const portIsOpen = checkPort(); // Check whether the port is open (see checkPort function below)
  if (!portIsOpen) return; // If the port is not open, exit the draw loop

  let str = port.readUntil("\n"); // Read from the port until the newline
  if (str.length == 0) return; // If we didn't read anything, return.
  str = str.trim(); // Remove any whitespace/newline characters from the string

  console.log('received:', str); // Log the incoming string to the console for debugging

  // Split the incoming string into an array using commas as separators
  let values = str.split(",");
  if (values.length >= 3) {
    // Parse the individual values and store in respective variables
    xVal = Number(values[0]);
    yVal = Number(values[1]);
    switchVal = Number(values[2]);    
  }
}

function sendData() {
  /**
   * Send the total switch presses to the Arduino
   * totalSwitchPresses is a global variable
   */
    console.log('writing:', totalSwitchPresses);
    port.write(totalSwitchPresses);
}

// Three helper functions for managing the serial connection.

function setupSerial() {
  port = createSerial();

  // Check to see if there are any ports we have used previously
  let usedPorts = usedSerialPorts();
  if (usedPorts.length > 0) {
    // If there are ports we've used, open the first one
    port.open(usedPorts[0], BAUD_RATE);
  }

  // create a connect button
  connectBtn = createButton("Connect to Arduino");
  connectBtn.position(5, 5); // Position the button in the top left of the screen.
  connectBtn.mouseClicked(onConnectButtonClicked); // When the button is clicked, run the onConnectButtonClicked function
}

function checkPort() {
  if (!port.opened()) {
    // If the port is not open, change button text
    connectBtn.html("Connect to Arduino");
    // Set background to gray
    background("gray");
    return false;
  } else {
    // Otherwise we are connected
    connectBtn.html("Disconnect");
    return true;
  }
}

function onConnectButtonClicked() {
  // When the connect button is clicked
  if (!port.opened()) {
    // If the port is not opened, we open it
    port.open(BAUD_RATE);
  } else {
    // Otherwise, we close it!
    port.close();
  }
}