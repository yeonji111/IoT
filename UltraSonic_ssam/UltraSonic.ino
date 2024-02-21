
#define RGBLENGTH 3

//pin 설정
const int triggerPin = 6;
const int echoPin = 7;
const int ledPin = 9;

int cm = 0;

const int rgb_arr[RGBLENGTH] = {9, 10, 11};
const int rgb_step[RGBLENGTH] = {20, 50, 100}; 
const String rgb_str[RGBLENGTH] = {"위험", "적당", "넉넉"}; 

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);

  long received_time = pulseIn(echoPin, HIGH); //수신한 시간

  return received_time; //수신한 시간
}

void setup()
{
  Serial.begin(9600);
  //핀 초기화
  for(int i = 0; i < RGBLENGTH; i++)
  {
    pinMode(rgb_arr[i], OUTPUT);
  }

  Serial.println("real,filter");
}

void loop()
{
  // measure the ping time in cm
  int raw = 0.01723 * readUltrasonicDistance(triggerPin, echoPin);
 
  if(10 < raw && raw < 300)
  {
    cm = raw; //안전해진 것만 넣기
  }

  int reverse = 255 - map(cm, 10, 300, 0, 255);
	analogWrite(ledPin, reverse);

  //구간별로 색상 다르게하여 출력 100 200 300
  for(int i = 0; i < RGBLENGTH; i++)
  {
    if(cm < rgb_step[i])
    {
      digitalWrite(rgb_arr[i], HIGH);
      Serial.println(rgb_str[i]);
      break;
    }
    else
    {
      digitalWrite(rgb_arr[i], LOW);
    }
  }
  //Serial.print(raw);
  //Serial.print(",");
  //Serial.println(cm);
  delay(100); // Wait for 100 millisecond(s)
}