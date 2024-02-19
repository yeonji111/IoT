
// pins for the LEDs:
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

int readSerialValue = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    
    readSerialValue = Serial.read();

    // look for the next valid integer in the incoming serial stream:
    int red = readSerialValue;
    // do it again:
    int green = readSerialValue;
    // do it again:
    int blue = readSerialValue;

    // look for the newline. That's the end of your sentence:
   
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);

      // fade the red, green, and blue legs of the LED:
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    
  }
}
