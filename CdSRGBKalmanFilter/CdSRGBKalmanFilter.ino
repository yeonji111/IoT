// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9;
const int r_pin = 3;
const int g_pin = 4;
const int b_pin = 5;

const int lowTreshole = 600; //끄는 기준치
const int highTreshole = 750; //켜는 기준치

int sensorValue = 0;  // value read from the pot

int status = LOW; //최초 실행 상태 보장


// 칼만필터
const float A = 1;  // 시스템 행렬 과거값 반영비율
const float H = 1;  // 출력 행렬
const float Q = 2;  // 과거값 반영비율
const float R = 100;  // 측정 오차
float x = 675;  // 초기 추정값
float P = 3;  // 초기 추정값에 대한 오차 공분산




void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(analogInPin, INPUT);
  pinMode(analogOutPin, OUTPUT);
  pinMode(r_pin, OUTPUT);
  pinMode(g_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);

    Serial.println("원본, Kalman");
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);


 // 추정값과 오차 공분산 예측
  float xp = A * x;
  float Pp = (A * P / A) + Q;

  //칼만 이득 계산 
  float K = (Pp / H) / (H * Pp / H + R);

  //추정값 계산
  x = xp + K * (sensorValue - H * xp);

  //오차 공분산 계산
  P = Pp - (K * H * Pp);




  if(x > highTreshole)
  {
    status = HIGH;
    digitalWrite(r_pin, HIGH);
    digitalWrite(g_pin, LOW);
    digitalWrite(b_pin, LOW);
  }
  else if(x < lowTreshole)
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
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(x);
}
