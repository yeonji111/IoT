  //자체 WEB Server 구축용
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//HTML 소스 코드
#include "HTMLPage.h"

// HTML 소스 모음 클래스
HTMLPage html; //객체 생성

//최대 20개까지 스캔
//신호 강도 기준
#define WIFI_MAX_SIZE 15

//각자의 보드 WIFI 설정
const char* ap_ssid = "yeonji11111"; //표출되는 이름
const char* ap_password = "yeonji11111"; //비번

//WEB 서버 클래스
ESP8266WebServer server(80); //server라는 객체 생성

//스캔한 wifi를 받아둘 배열 만들기
String wifiList[WIFI_MAX_SIZE];
//스캔한 wifi의 강도를 저장할 배열 만들기
String rssiList[WIFI_MAX_SIZE];

//최종 연결된 기존에 있던 wifi
//인터넷이 가능한 wifi에 들러붙어 기생
String connectedSSID = "next1";
String connectedPassword = "next18850";
int connectedSSIDAddress = 64; // 0~255까지 주소지 설정 가능
int connectedPasswordAddress = 128;


// 모드 전환하는 버튼
int operatingMode = 1;  // 1 - AP(Access Point = 스캔하고 있는 상태 , 자체(=기생) 와이파이), 2 - STA(Station Access Point = 스캔한 것들 중에 숙주용으로 찍은 것, 숙주 와이파이)
int operatingPin = D2;
int buttonState;
unsigned long currentMillis = 0; // unsigned = 양수 타입(+), 시간은 -가 없으므로 확실히 하기 위해



//wifi 스캔함수
//찾은 갯수를 반환
int scanWiFiList()
{
  int n = WiFi.scanNetworks(); //근처에 있는 와이파이들 갯수

  //최대 갯수만 넣기
  int k = 0; //멀쩡한 WIFI 갯수
  for(int i = 0; i < WIFI_MAX_SIZE && i < n; i++)
  {
    //wifiList[i] = WiFi.SSID(i); //스캔된 WIFI이름 문자열
    //rssiList[i] = WiFi.RSSI(i); //스캔된 wifi의 강도

    String s = WiFi.SSID(i);
    String r = String(WiFi.RSSI(i));

    //둘다 멀쩡해야 됨
    if(isValidate(s) && isValidate(r))
    {
      wifiList[k] = WiFi.SSID(i);
      rssiList[k] = WiFi.RSSI(i);
      k++; //검증될 때만 k값 증가
    }
  }

  //오름차순으로 정렬하기
  //정렬 알고리즘은 자율선택
  //String을 int로 변환필요 toInt();
  //int를 String           String(x)
  //-40 -50 -55 -60 -70
  //일단 전체를 돌고 돌아야함
  for(int i = 0; i < WIFI_MAX_SIZE && i < n; i++)
  {
    bool isSwap = false;
    for(int j = 0; j < n - 1; j++)
    {
      //비교했더니 다음꺼가 더 크다면 전치
      if(rssiList[j].toInt() < rssiList[j + 1].toInt())
      {
        swap(&rssiList[j], &rssiList[j + 1]);
        swap(&wifiList[j], &wifiList[j + 1]);
        isSwap =  true;
      }
    }
    if(!isSwap)
    {
      break;
    }
  }

  return k; //검증된 wifi 갯수만 반환
}

//빈문자열인지 이상한 문자열인지 null인지 empty인지 검사
//멀쩡하면 true, 쬐끔이라도 이상하면 false 반환
bool isValidate(String data)
{
  return (data != NULL) //null은 절대안됨
  && (!data.isEmpty()) //제공되는 함수
  && (data != " ") //공백문자 안됨
  && (data != "")  //비어있어도 안됨
  && ( data != "0") // 0이여도 안됨
  ;
}

//전치 전도 함수
void swap(String* a, String* b)
{
  String temp = *a;
  *a = *b;
  *b = temp;
}

//맨처음 접속시 보여줄 문자열(=HTML코드) 출력
void handleRoot()
{
  scanWiFiList(); //하드웨어적으로 스캔결과 보관

  //맨처음 접속시 보여줄 문자열(=HTML코드) 출력
  server.send(200, "text/html", prepareSelectWifiList());
}

//Form 데이터 받아서 처리
//지정한 ssid, pw 받아서 저장
void handleForm()
{
  String apName = server.arg("apName");
  String apPw = server.arg("apPw");

  //잘 받았나 시리얼로 확인하기
  Serial.print("apName = ");
  Serial.println(apName);
  Serial.print("apPw = ");
  Serial.println(apPw);

  //받은거 저장하기
  //이후 계속 이거로 사용
  connectedSSID = apName;
  connectedPassword = apPw;

  EEPROM.put(connectedSSIDAddress, apName);
  EEPROM.put(connectedPasswordAddress, apPw);

  server.send(200, "text/html", html.getReturn);
}

//HTML에 스캔 결과를 띄워주는 함수
String prepareSelectWifiList()
{
  String strList ="<ul>";
  for(int i =0; i< WIFI_MAX_SIZE; i++){
      if(wifiList[i] != "")
      {
        strList += "<li><a name='" + wifiList[i] + "' onclick='select(this.name)'>" + wifiList[i] + "</a> </li>";
      }
  }
  strList += "</ul>";

  //위에서 만든 <li> 코드를 이제 완성된 HTML 코드로 만들어줌
  String htmlPage = html.getWIFIList(strList);
  
  return htmlPage;

}
//여기까지가 본 기기가 동작되기 위해 필요한 잡다한 함수들

void setup() {
  // put your setup code here, to run once:
  pinMode(operatingPin, INPUT);

  Serial.begin(115200); //최소 115200이 요구됨
  Serial.println("Start WIFI");

  Serial.println("Start AP");

  //보드 자체적으로 WIFI를 구축하겠다는거임
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

  Serial.println("start server");
  server.begin();

  delay(1000);
  // int n = scanWiFiList();

  //스캔 결과를 시리얼로 확인
  // for(int i = 0; i < n; i++)
  // {
  //   String name = wifiList[i];
  //   String rssi = rssiList[i];

  //   String scanningResult = name + " : " + rssi;
  //   Serial.println(scanningResult);
  // }
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
    WiFi.softAP(ap_ssid, ap_password);
    server.begin();
    delay(1000);
    Serial.println("Switching STA >>>> AP");
  }
  else if(operatingMode == 1 && buttonState)
  {
    operatingMode = 2;
    delay(1000);

    //ROM에 저장된 값 가져오기
    String SSID_temp;
    EEPROM.get(connectedSSIDAddress,SSID_temp);

    String password_temp;
    EEPROM.get(connectedPasswordAddress, password_temp);

    if(isValidate(SSID_temp) && isValidate(password_temp))
    {
    connectedSSID = SSID_temp;
    connectedPassword = password_temp;

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(connectedSSID);

    WiFi.mode(WIFI_STA); // 자체 WIFI를 끄고, 숙주한테 들러붙는 모드
    WiFi.begin(connectedSSID, connectedPassword); // 숙주한테 붙기
    delay(1000);
    // 연결될 때까지 기다리기
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    // 숙주한테 연결하도록 허가 받음
    Serial.println("wifi connected OK");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Switching AP >>>> STA");

    }
    else
    {
    Serial.println("wifi setting not yet !!");  
    }
  }

  switch(operatingMode)
  {
    // 숙주 찾는 모드
    case 1:
      // server.handleClient();
      networking(); // 숙주 찾는 함수
      break;

    case 2:
      sensing(); // 숙주에 들러붙어 사는 함수
      break;
  }
}

void networking()
{
 //보드 자체적으로 WIFI를 구축하겠다는거임
  // WiFi.softAP(ap_ssid, ap_password);
  server.handleClient();
}

void sensing()
{

}