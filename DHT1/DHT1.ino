
// <> 라이브러리, "" 사용자
#include <DHT.h> 

// 1-wire 시리얼 통신
//-- global static
#define DHTPIN 2     // Digital pin connected to the DHT sensor 

// 멤버 고정 변수
// const int pin = 2; 

// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT11

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
// 클래스로 객체 생성
DHT dht(DHTPIN, DHTTYPE);

const int led = 9;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 Start!"));

  dht.begin();

  pinMode(led,OUTPUT);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2200);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float degree = (1.8 * t) - 0.55*(1 - h/100)*(1.8*t - 26) + 32;



  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("%  불쾌지수: "));
  Serial.println(degree);

  int out = map(degree, 0, 100, 0,255);
  analogWrite(led, out);
}
