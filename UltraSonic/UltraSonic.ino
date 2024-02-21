// pin 설정
const int triggerPin = 11;
const int echoPin = 12;
const int ledPin = 9;


// const int redPin = 3;
// const int greenPin = 5;
// const int bluePin = 6;

const int rgb_arr[3] = { 3, 5, 6 };
const int rgb_step[3] = {20,30,40};

int cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);

  long received_time = pulseIn(echoPin, HIGH);  // 수신한 시간

  return received_time;  // 수신한 시간
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // 핀 초기화
  for (int i = 0; i < 3; i++) {
    pinMode(rgb_arr[i], OUTPUT);
  }


  // pinMode(redPin, OUTPUT);
  // pinMode(greenPin, OUTPUT);
  // pinMode(bluePin, OUTPUT);
}

void loop() {
  // measure the ping time in cm
  int raw = 0.01712 * readUltrasonicDistance(triggerPin, echoPin);

  if (10 < raw && raw < 300) {
    cm = raw;
    int reverse = 255 - map(cm, 10, 300, 0, 255);

    analogWrite(ledPin, reverse);
  }

  // 구간별로 색상 다르게 하여 출력 10,40,100
  // RGB
  for (int i = 0; i < 3; i++) {
    if (cm < rgb_step[i]) {
      digitalWrite(rgb_arr[i], HIGH);
    } else {
      digitalWrite(rgb_arr[i], LOW);
    }
  }
  // Serial.print("raw");
  // Serial.print(", ");
  // Serial.println(cm);
  delay(100);  // Wait for 100 millisecond(s)
}
