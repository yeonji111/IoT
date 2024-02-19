/*
 시리얼 입력예제
 
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 4;    // the number of the LED pin

// Variables will change:
int ledState = HIGH;        // the current state of the output pin

int buttonState = LOW;

void setup() {
	
  Serial.begin(115200);	
	
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  
  int readButton = digitalRead(buttonPin);

  Serial.println(readButton); // 버튼의 실제값 확인하기, 2번 포트에 어떤 값이 있는지 출력
    
  if(buttonState != readButton)
  {
    digitalWrite(ledPin, readButton);
    
    buttonState = readButton;
  }
  
  //Serial.println(readButton);

  
  if (Serial.available() > 0) {

    // look for the newline. That's the end of your sentence:
	
	char readSerialData = Serial.read();
	
    if (readSerialData == 'w') 
	{
      Serial.println("LED ON");
	  ledState = HIGH;
    }
	else if(readSerialData == 's')
	{
	  Serial.println("LED OFF");
	  ledState = LOW;
	}
    

    // set the LED:
    digitalWrite(ledPin, ledState);
  }
  
}
