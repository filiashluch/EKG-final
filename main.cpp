#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>  
float teplota, vlhkost, tep, kyslik, teplota_tela;
 
dht DHT;
PulseOximeter pox;

void setup() {
}

void loop() {
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
  ptr +=".text{font-weight: 600;font-size: 19px;width: 200px;}";
  ptr +=".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr +=".teplota .reading{color: #F29C1F;}";
  ptr +=".vlhkost .reading{color: #3B97D3;}";
  ptr +=".tep .reading{color: #FF0000;}";
  ptr +=".kyslik .reading{color: #955BA5;}";
  ptr +=".teplota_tela .reading{color: #F29C1F;}";
  ptr +="</style>";
  ptr +="</head>";
  ptr +="<body>";
