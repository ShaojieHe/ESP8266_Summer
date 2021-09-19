#ifndef __Wifi_FUNC__
#define __Wifi_FUNC__


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define IP_URL "http://ip.sb"

String WiFi_GetIPAddress(void);
void WiFi_Connect(void);
void WiFi_SmartConfig(String &SSID, String &Passwd);
void WiFi_WriteConfig(String &SSID, String &Passwd);
void WiFi_WipeConfig(void);
bool WiFi_ReadConfig(String &SSID, String &Passwd);
void WiFi_FlashLED(u16_t ms);
void IRAM_ATTR WiFi_Check(void);
void WiFi_AfterConnect(void);
void WiFi_Reconfig(void);

#endif