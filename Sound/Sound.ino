#define RGBLEGNTH 3

const int sound_pin = 2;  //마이크 연결핀
const int led_pin = 9;    // LED 출력핀

const int rgb_arr_pin[3] = { 3, 5, 6 };

void rgb_write(int pin, int value);  // 선언 interface

void setup() {
  // put your setup code here, to run once:
  pinMode(sound_pin, INPUT);
  pinMode(led_pin, OUTPUT);

  // 한방에 초기화
  for (int i = 0; i < 3; i++) {
    pinMode(rgb_arr_pin[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int read_mic = digitalRead(sound_pin);

  if (read_mic) {
    Serial.println("sonund detect!");

    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);

    int rgb_value_arr[RGBLEGNTH] = { r, g, b };

    for (int i = 0; i < 3; i++) 
    {
      rgb_write(i, rgb_value_arr[i]);
    }
  }
}

// 디자인 패턴 - 퍼사드
void rgb_write(int pin, int value) {
  analogWrite(rgb_arr_pin[pin], value);
}