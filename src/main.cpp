#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <TimerInterrupt_Generic.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <U8g2lib.h>

#include <icon/icon.h>
#include <Wifi_Func/Wifi_Func.hpp>
#include <API/API.hpp>
ESP8266TimerInterrupt CheckConn;
WiFiUDP UDPClient;
NTPClient TimeClient(UDPClient, "ntp.aliyun.com", +28800, 600);
String SSID, Passwd;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 0, 5, 4);

extern WeatherNow API_Now;
extern WeatherForecast API_Forcast;
extern AirQuality API_Quality;

void setup()
{
  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearDisplay();
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(13, INPUT_PULLUP);

  API_Boot(u8g2);
  

  CheckConn.attachInterruptInterval(5000000L, WiFi_Check);
}

void loop()
{
  u8g2.clear();
  // put your main code here, to run repeatedly:
  //delay(10000);
  //Serial.printf("\nPin8 Status: %s\n", !digitalRead(14) ? "Pressed" : "Not Pressed");
  //Serial.println(TimeClient.getFormattedTime());
  API_FirstPage(u8g2);

  API_WeatherNow(u8g2);
  API_WeatherTomorrow(u8g2);

  API_CheckUpdate(u8g2);
}
