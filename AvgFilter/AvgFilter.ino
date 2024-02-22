
#include <Servo.h>
#define RGBLENGTH 3
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int servo_pin = 10;
const int triggerPin = 11;
const int echoPin = 12;


int cm = 0;


const int numReadings = 10;
int readings[numReadings] = {0};  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int movingAverage = 0;    




long readUltrasonicDistance(int triggerPin, int servo_pin);


// 바로 cm (거리)가 나옴
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);

  long received_time = pulseIn(echoPin, HIGH);  //수신한 시간

  long cm = 0.01723 * received_time;

  return cm;  //수신한 시간
}


void setup() {
  myservo.attach(servo_pin);
  Serial.begin(9600);
  Serial.println("원본, MAF");
}

// 180도 내에서 회전 반복
void loop() {
  int raw = readUltrasonicDistance(triggerPin, echoPin);

  if (10 < raw && raw < 300) {
    cm = raw;  //안전해진 것만 넣기
  }
  // 이동평균 필터
  // 기존 데이터는 합계에서 삭제
  total = total - readings[readIndex];

  // 신규 데이터를 버퍼에 저장
  readings[readIndex] = cm;

  //신규 데이터를 현재 위치까지만 합계에 반영
  total = total + readings[readIndex];

  //버퍼 이동
  readIndex = readIndex + 1;  // 다음칸으로 지시

  //꽉 차면 버퍼 비우기
  //합계가 남아 있기 때문에 괜찮
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // 실시간으로 평균
  movingAverage = total / numReadings;



  // 최소 10cm ~ 130cm
  int pos = map(movingAverage, 10, 100, 0, 180);

  myservo.write(pos);


  Serial.print(cm);
  Serial.print(",");
  Serial.println(movingAverage);

  delay(100);
}
