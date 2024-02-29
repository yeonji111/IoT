// 중복 import 방지용 코드
#ifndef TH11
#define TH11

#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int DHTSensingDelay = 2100; // 스펙상 2초에 한 번


float readTemp(); //온도 읽기
float readHum(); //습도 읽기

float readTemp()
{
  return dht.readTemperature();
}

float readHum()
{
  return dht.readHumidity();
}










#endif