#include "WIFIManager.h" // wifi 담당
#include "TH11.h" // 온습도 센서 담당


//모드전환하는 버튼
int operatingMode = 1; // 1 - AP, 2 - STA
int operatingPin = D2;
int buttonState;
unsigned long currentMillis = 0;
unsigned long preMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(operatingPin, INPUT);

  EEPROM.begin(255);
  Serial.begin(115200); //최소 115200이 요구됨
  Serial.println("Start WIFI");

  Serial.println("Start AP");

  //보드 자체적으로 WIFI를 구축하겠다는거임
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(ap_ssid, ap_password);

  delay(1000);

  //인터넷은 안되지만, 자체적으로 IP 주소를 가지고 있음
  IPAddress myIP = WiFi.softAPIP();
  //자체적으로 가지고 있는 IP 주소 확인
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //주소 GepMapping
  //해당 주소에 대한 요청을 처리해줌
  server.on("/", handleRoot);
  server.on("/action_page", handleForm);
  delay(1000);
 
  Serial.println("start server");
  server.begin();
  delay(1000);

  char SSID_temp[50];
  char Password_temp[50];

  for(int i = 0; i < 50; i++)
  {
    SSID_temp[i] = EEPROM.read(connectedSSIDAddress + i);
    Password_temp[i] = EEPROM.read(connectedPasswordAddress + i);
  }

  Serial.println("ROM Load");
  Serial.println(SSID_temp);
  Serial.println(Password_temp);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  
  int buttonState = digitalRead(operatingPin);

  if(operatingMode == 2 && buttonState)
  {
    operatingMode = 1;
    delay(1000);

    //보드 자체적으로 WIFI를 구축하겠다는거임
    //보드 자체적으로 WIFI를 구축하겠다는거임
    WiFi.mode(WIFI_AP); 
    WiFi.softAP(ap_ssid, ap_password);
    server.begin();
    delay(1000);
    Serial.println("Switching STA >>>> AP");
  }
  else if(operatingMode == 1 && buttonState)
  {
    delay(1000);

    //ROM에 저장된 값 가져오기
    char SSID_temp[50];
    char Password_temp[50];

    for(int i = 0; i < 50; i++)
    {
      SSID_temp[i] = EEPROM.read(connectedSSIDAddress + i);
      Password_temp[i] = EEPROM.read(connectedPasswordAddress + i);
    }
    Serial.println("ROM Load");
    Serial.println("SSID_temp");
    Serial.println("Password_temp");
    

    if(isValidate(SSID_temp) && isValidate(Password_temp))
    {
      //connectedSSID = SSID_temp; // EEPROM 이슈로 걍 학원 WIFI 고정
      //connectedPassword = Password_temp; // EEPROM 이슈로 걍 학원 WIFI 고정
      operatingMode = 2;
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(connectedSSID);

      WiFi.mode(WIFI_STA); //자체 WIFI를 끄고, 숙주한테 들러붙는 모드
      WiFi.begin(connectedSSID, connectedPassword); //숙주한테 붙기
      delay(1000);
      //연결될 때까지 기다리기
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      //숙주한테 연결하도록 허가 받음
      Serial.println("wifi connected OK");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("Switching AP >>>> STA");
    }
    else
    {
      Serial.println("wifi setting not yet");
    }

    
  }

  switch(operatingMode)
  {
    //숙주 찾는 모드
    case 1:
      //server.handleClient();
      networking(); //숙주 찾는 함수
      break;

    case 2:
      sensing(); //숙주에 들러붙어 사는 함수
      break;
  }

}

void networking()
{
  server.handleClient();
}

void sensing()
{
  if(currentMillis - preMillis > DHTSensingDelay)
  {
    float temp = readTemp(); // 온도 읽기
    float hum = readHum(); // 습도 읽기

    String sensingLog = "temp: " + String(temp) + "C hum: " + String(hum) + "% ";
    Serial.println(sensingLog);

    preMillis = currentMillis;


    
  }

}
