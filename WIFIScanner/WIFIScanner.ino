// 자체 WEB Server 구축용
#include <ESP8266WebServer.h>

// HTML 소스 코드
#include "HTMLPage.h"

// HTML 소스 모음 클래스
HTMLPage html;  // 객체 생성

// 최대 20개까지 스캔
// 신호 강도 기준(강한것부터 -> 약한것 순으로 20개)
#define WIFI_MAX_SIZE 20


// 각자의 보드 WIFI 설정
const char* ap_ssid = "std115";      // 표출되는 이름
const char* ap_password = "std115";  // 비번


// WEB 서버 클래스
ESP8266WebServer server(80);  // server라는 객체 생성


// 스캔한 wifi를 받아둘 배열 만들기
String wifiList[WIFI_MAX_SIZE];

// 스캔한 wifi의 강도를 저장할 배열 만들기
String rssiList[WIFI_MAX_SIZE];

// 최종 연결된 기존에 있던 wifi
// 인터넷이 가능한 wifi에 들러붙어 기생
String connectedSSID = "";
String connectedPassword = "";

// wifi 스캔함수
// 찾은 와이파이의 갯수를 반환(return) 하는 scanWiFiList 함수
int scanWiFiList() {
  int n = WiFi.scanNetworks();  // 근처에 있는 와이파이들 갯수를 리턴해 n에 담김, 함수 실행 후 리턴할 와이파이의 갯수

  // 최대 갯수만 넣기
  int k = 0;  // 멀쩡한 WIFI 갯수
  for (int i = 0; i < WIFI_MAX_SIZE && i < n; i++) {
    // wifiList[i] = WiFi.SSID(i);  // 스캔된 WIFI 이름 (문자열)
    // rssiList[i] = WiFi.RSSI(i);  // 스캔된 wifi의 강도

    String s = WiFi.SSID(i);
    String r = String(WiFi.RSSI(i));
    // 둘 다 멀쩡해야 됨
    if(isValidate(s) && isValidate(r)){
      wifiList[k] = WiFi.SSID(i);
      rssiList[k] = WiFi.RSSI(i);
      k++; // 검증될 때만 k값 증가
    }
  }

  // 오름차순으로 정렬하기
  // 정렬 알고리즘은 자율선택
  bool isSwap = false;
  for (int i = 0; i < WIFI_MAX_SIZE - 1 && i < n; i++) {
    for (int j = 0; j < WIFI_MAX_SIZE - 1 - i; j++) {
      // 비교했더니 다음꺼가 더 크다면 전치
      if (rssiList[j].toInt() > rssiList[j + 1].toInt()) {
        swap(&rssiList[j], &rssiList[j + 1]);
        swap(&wifiList[j], &wifiList[j + 1]);
        isSwap = true;

        // int tmp = rssiList[j].toInt();
        // rssiList[j] = rssiList[j + 1].toInt();
        // rssiList[j + 1] = tmp;

        // String name = wifiList[j];
        // wifiList[j] = wifiList[j + 1];
        // wifiList[j + 1] = name;
      }
    }
    if (!isSwap) {
      break;
    }
  }
  return k; // 검증된 wifi 갯수만 반환
}

// 빈문자열인지 이상한 문자열인지 null인지 empty인지 검사 (유효성검사)
// 멀쩡하면 true, 이상하면 false 반환
bool isValidate(String data) {
  return (data != NULL) // null 안됨
  && (!data.isEmpty()) // .isEmpty() => 제공되는 함수
  && (data != " ") // 공백문자 안됨
  && (data != ""); // 비어있어도 안됨
}

// 전치 전도 함수
void swap(String* a, String* b) {
  String temp = *a;
  *a = *b;
  *b = temp;
}

// 맨 처음 접속 시 보여줄 문자열 ( = HTML 코드 ) 출력
void handleRoot() {
  // 이거 원래 여기 있으면 안됨
  scanWiFiList();  // 하드웨어적으로 스캔결과 보관

  // 맨 처음 접속 시 보여줄 몬자열 ( = HTML 코드 ) 출력
  server.send(200, "text/html", prepareSelectWifiList());
}


// Form 데이터 받아서 처리
// 지정한 ssid, pw 받아서 저장
void handleForm() {
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

// HTML에 스캔 결과를 띄워주는 함수
String prepareSelectWifiList() {



  String strList = "<ul>";
  for (int i = 0; i < WIFI_MAX_SIZE; i++) {
    if (wifiList[i] != "") {
      strList += "<li><a name='" + wifiList[i] + "' onclick='select(this.name)'>" + wifiList[i] + "</a> </li>";
    }
  }
  strList += "</ul>";

  // 위에서 만든 <li> 코드를 이제 완성된 HTML 코드로 만들어줌
  String htmlPage = html.getWIFIList(strList);

  return htmlPage;
}
// 여기까지가 본 기기가 동작되기 위해 필요한 잡다한 함수들




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // 최소 115200이 요구된다.
  Serial.println("Start WIFI");


  Serial.println("Start AP");

  // 보드 자체적으로 WIFI를 구축하겠다는 의미
  // ap_ssid, ap_password 는 코드 작성자가 임의로 만든 와이파이 id/pw
  WiFi.softAP(ap_ssid, ap_password);

  // 인터넷은 안되지만, 자체적으로 IP 주소를 가지고 있다.
  IPAddress myIP = WiFi.softAPIP();


  // 자체적으로 가지고 있는 IP 주소 확인


  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // = 주소 GetMapping
  // 해당 주소에 대한 요청을 처리해줌
  // server.on("/", handleRoot);
  // server.on("/action_page", handleForm);
  scanWiFiList();

  int n = scanWiFiList();

  // 스캔 결과를 시리얼로 확인
  for (int i = 0; i < n; i++) {
    String name = wifiList[i];
    String rssi = rssiList[i];

    String scanningResult = name + " : " + rssi;
    Serial.println(scanningResult);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // 아직은 센서가 없음
  // 요청 감시하는 함수만 호출
  // server.handleClient();
}
