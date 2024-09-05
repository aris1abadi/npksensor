#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
/* INCLUDE ESP2SOTA LIBRARY */
#include <ESP2SOTA.h>
#include <sensorHandle.h>
#include <loggerHandle.h>

const char *ssid = "npksensor V1";
const char *password = "npk123";

WebServer server(80);

void ota_init()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("npk sensor", "npksensor123");
  delay(1000);
  IPAddress IP = IPAddress(10, 10, 10, 1);
  IPAddress NMask = IPAddress(255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, NMask);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* SETUP YOR WEB OWN ENTRY POINTS */
  server.on("/npksensor", HTTP_GET, []()
            {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", getSensor()); });

  server.on("/npklogger", HTTP_GET, []()
            {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", getDataLog()); });

  /* INITIALIZE ESP2SOTA LIBRARY */
  ESP2SOTA.begin(&server);
  server.begin();
}

void ota_loop()
{
  static long delayOta = millis();
  if ((millis() - delayOta) > 5)
  {
    delayOta = millis();
    server.handleClient();
  }
}