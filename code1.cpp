#include <WiFi.h> 
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>
 
#define DHTTYPE DHT11
#define DHTPIN 18
#define DS18B20 5
#define REPORTING_PERIOD_MS     1000
 

const char* ssid = "filias";
const char* password = "123456789";
 
DHT dht(DHTPIN, DHTTYPE);
PulseOximeter pox;
uint32_t tsLastReport = 0;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
 
float temperature, humidity, BPM, SpO2, bodytemperature;
 
WebServer server(80);             
 
void onBeatDetected()
{
  Serial.println("Tep detekovan");
}
 
void setup() {
  Serial.begin(115200);
  pinMode(19, OUTPUT);
  delay(100);   
 
  Serial.println("Pripojuji se ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi pripojena");
  Serial.print("IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
 
  server.begin();
  Serial.println("HTTP server start");
 
  Serial.print("Inicialnizace detektoru pulzu/kysliky");
 
  if (!pox.begin()) {
    Serial.println("ERROR");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
 
   pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
 
}
void loop() {
  server.handleClient();
  pox.update();
  sensors.requestTemperatures();
  int chk = DHT.read11(DHT11_PIN);
  
  temperature = DHT.temperature;
  humidity = DHT.humidity;
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  bodytemperature = sensors.getTempCByIndex(0);
 
  
 if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
    Serial.print("Teplota v mistnosti: ");
    Serial.print(DHT.temperature);
    Serial.println("°C");
    
    Serial.print("Vlhkost v mistnosti: ");
    Serial.print(DHT.humidity);
    Serial.println("%");
    
    Serial.print("BPM: ");
    Serial.println(BPM);
    
    Serial.print("O2: ");
    Serial.print(SpO2);
    Serial.println("%");
 
    Serial.print("Teplota tela: ");
    Serial.print(bodytemperature);
    Serial.println("°C");
    
    Serial.println("*********************************");
    Serial.println();
 
    tsLastReport = millis();
  }
  
}