#include <WiFi.h> 
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Arduino.h>
#include "OneWire.h"
#include <DallasTemperature.h>
#include "DHT.h"
 
#define DHTPIN 15
#define DHTTYPE DHT11
#define DS18B20 5
#define OPAK_REPORT     1000
 
float temperature, humidity, BPM, SpO2, bodytemperature;

const char* ssid = "filias";
const char* password = "tp159net";
 
DHT dht(DHTPIN, DHTTYPE);
PulseOximeter pox;
uint32_t tsLastReport = 0;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
 
 
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
 
  /*server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);*/
 
  server.begin();
  Serial.println("HTTP server start");
 
  Serial.print("Inicialnizace detektoru pulzu/kysliku");
 
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
  
  temperature = dht.readTemperature();;
  humidity =  dht.readHumidity();;
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  bodytemperature = sensors.getTempCByIndex(0);
 
  
 if (millis() - tsLastReport > OPAK_REPORT) 
  {
    Serial.print("Teplota v mistnosti: ");
    Serial.print(temperature);
    Serial.println("°C");
    
    Serial.print("Vlhkost v mistnosti: ");
    Serial.print(humidity);
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