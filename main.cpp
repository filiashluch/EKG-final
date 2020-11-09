 #include <WiFi.h> 
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>
 
#define DHTTYPE DHT11
#define DHT11_PIN 18
#define DS18B20 5
#define REPORTING_PERIOD_MS     1000
 
float teplota, Vlhkost, BPM, KyslikKrev, TeplotaTela;
 

const char* ssid = "filias";
const char* password = "123456789";
 
DHT dht;
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
 
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  //check wi-fi is connected to wi-fi network
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
  
  teplota = dht.Teplota;
  vlhkost = dht.Vlhkost;
  BPM = pox.getHeartRate();
  kyslikKrev = pox.getSpO2();
  teplotaTela = sensors.getTempCByIndex(0);
 
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
    Serial.print("Teplota v mistnosti: ");
    Serial.print(dht.Teplota);
    Serial.println("°C");
    
    Serial.print("Vlhkost v mistnosti: ");
    Serial.print(dht.Vlhkost);
    Serial.println("%");
    
    Serial.print("BPM: ");
    Serial.println(BPM);
    
    Serial.print("Kyslik v krvi: ");
    Serial.print(KyslikKrev);
    Serial.println("%");
 
    Serial.print("Teplota tela: ");
    Serial.print(TeplotaTela);
    Serial.println("°C");
    
    Serial.println("*********************************");
    Serial.println();
 
    tsLastReport = millis();
  }
  
}

 String SendHTML(float teplota,float vlhkost,float tep,float kyslik, float teplota_tela){
  String ptr = "<!DOCTYPE html>";
  ptr +="<html>";
  ptr +="<head>";
  ptr +="<title>Stav-Pacienta</title>";
  ptr +="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr +="<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr +="<style>";
  ptr +="html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}";
  ptr +="body{margin: 0px;} ";
  ptr +="h1 {margin: 50px auto 30px;} ";
  ptr +=".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr +=".text{font-weight: 600;font-size: 20px;width: 200px;}";
  ptr +=".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr +=".teplota .reading{color: #F29C1F;}";
  ptr +=".vlhkost .reading{color: #3B97D3;}";
  ptr +=".tep .reading{color: #FF0000;}";
  ptr +=".kyslik .reading{color: #955BA5;}";
  ptr +=".teplota_tela .reading{color: #F29C1F;}";
  ptr +="</style>";
  ptr +="</head>";
  ptr +="<body>";
