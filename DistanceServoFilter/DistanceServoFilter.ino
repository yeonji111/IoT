
#include <Servo.h>
#define RGBLENGTH 3
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// 핀 설정
const int servo_pin = 10;
const int triggerPin = 11;
const int echoPin = 12;

int cm = 0;


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


long readUltrasonicDistance(int triggerPin, int echoPin);

// 저역통과필터 _전역변수 깔아두기
float pre_value = 0;
float pre_time = 0;
float tau = 500; // 어느 주파수를 차단할 지 결정 , tau의 수치가 낮을 수록 필터링이 적게 됨(원본과 비슷)


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
 
  // 저역통과필터

  //현재시간 측정 전원이 켜지고 경과한 시간
  float st_time = millis();

  //지난 시간과의 시간차 계산
  float dt = (st_time - pre_time);   

  //주파수 필터값 계산 (공식)
  float af_value = tau / (tau + dt)*pre_value 
  + dt/(tau + dt)*cm;

  //현재 상태 저장
  pre_value = af_value;
  pre_time = st_time; 




  // 최소 10cm ~ 130cm
  int pos = map(af_value, 10, 130, 0, 180);

  myservo.write(pos);

  Serial.print(cm);
  Serial.print(",");
  Serial.println(af_value);
}
