 
  const int redPin = 3;
  const int greenPin = 5;
  const int bluePin = 6;

  const int button = 2;

  int readSerialValue = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // 포트 초기화
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  int readButton = digitalRead(button); // 누르면 1이 들어오고, 아니면 0

  // 버튼 누를 때만 값 변경

  // red = constrain(red, 0, 255); constrain을 사용하면 최소,최대값을 지정해줄 수 있다.
  if(readButton == HIGH){
  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  // 해당 값을 진짜로 쏘기
  analogWrite(redPin,r);
  analogWrite(greenPin,g);
  analogWrite(bluePin,b);

  String r_str = String("red: ") + String(r);
  String g_str = String("green: ") + String(g);
  String b_str = String("blue: ") + String(b);


  // Serial.print(red, HEX); -> 16진수 값으로 출력해준다(RGB값 하면 떠오르는 FFFFFF 같은 결과 출력)
  Serial.println(r_str);
  Serial.println(g_str);
  Serial.println(b_str);

  }

 

  
} 