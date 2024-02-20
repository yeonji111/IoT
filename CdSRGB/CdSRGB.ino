// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9;
const int r_pin = 3;
const int g_pin = 4;
const int b_pin = 5;

const int lowTreshole = 600; //끄는 기준치
const int highTreshole = 800; //켜는 기준치

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value output to the PWM (analog out)


int status = LOW; //최초 실행 상태 보장

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(analogInPin, INPUT);
  pinMode(analogOutPin, OUTPUT);
  pinMode(r_pin, OUTPUT);
  pinMode(g_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  if(sensorValue > highTreshole)
  {
    status = HIGH;
    digitalWrite(r_pin, HIGH);
    digitalWrite(g_pin, LOW);
    digitalWrite(b_pin, LOW);
  }
  else if(sensorValue < lowTreshole)
  {
    status = LOW;
    digitalWrite(r_pin, LOW);
    digitalWrite(g_pin, LOW);
    digitalWrite(b_pin, HIGH);
  }
  else
  {
    digitalWrite(r_pin, LOW);
    digitalWrite(g_pin, HIGH);
    digitalWrite(b_pin, LOW);
  }


  digitalWrite(analogOutPin, status);

  // print the results to the Serial Monitor:
  Serial.println(sensorValue);
}
