// 자체 WEB Server 구축용 
#include <ESP8266WebServer.h>

// HTML 소스 코드
#include "HTMLPage.h"

// HTML 소스 모음 클래스
HTMLPage html; // 객체 생성

// 최대 20개까지 스캔
// 신호 강도 기준(강한것부터 -> 약한것 순으로 20개)
#define WIFI_MAX_SIZE 20


// 각자의 보드 WIFI 설정
const char* ap_ssid = "std115";
const char* ap_password = "std115";


// WEB 서버 클래스
ESP8266WebServer server(80); // server라는 객체 생성


// 스캔한 wifi를 받아둘 배열 만들기
String wifiList[WIFI_MAX_SIZE];


// 최종 연결된 기존에 있던 wifi
// 인터넷이 가능한 wifi에 들러붙어 기생
String connectedSSID = "";
String connectedPassword = "";

// wifi 스캔함수
void scanWiFiList()
{
  int n = WiFi.scanNetworks(); // 근처에 있는 와이파이들 갯수를 리턴해 n에 담김

// 최대 갯수만 넣기
  for(int i = 0; i < WIFI_MAX_SIZE && i < n; i++)
  {
    wifiList[i] = WiFi.SSID(i); // 스캔된 WIFI 이름 (문자열)
  }
}

// 맨 처음 접속 시 보여줄 몬자열 ( = HTML 코드 ) 출력
void handleRoot()
{
  // 맨 처음 접속 시 보여줄 몬자열 ( = HTML 코드 ) 출력
  server.send(200, "text/html", prepareSelectWifiList());
}


// Form 데이터 받아서 처리
// 지정한 ssid, pw 받아서 저장
void handleForm()
{
  String apName = server.arg("apName");
  String apPw = server.arg("apPw");


  // 잘 받았나 시리얼로 확인하기
  Serial.println("apName = ");
  Serial.println(apName);
  Serial.println("apPw = ");
  Serial.println(apPw);


  // 받은 와이파이 저장하기
  // 이후 계속 connectedSSID 와 connectedPassword 사용
  String connectedSSID = apName;
  String connectedPassword = apPw;

  server.send(200, "text/html", html.getReturn); 

}



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
