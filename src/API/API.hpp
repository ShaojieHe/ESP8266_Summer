#ifndef _API_
#define _API_

#include <Arduino.h>
#include <ESP8266_Heweather.h>
#include <U8g2lib.h>

const PROGMEM char Key[]="";
const PROGMEM char Location[]="106.09,38.49";

bool API_GetAll(void);
void API_Configure(void);

uint16_t API_GetLastUpdateTime(uint16_t API_Type);
const unsigned char * API_ReturnAddress(unsigned ICON);
String API_NextEvent(void);
void API_TestIcon(U8G2 u8g2);
void API_Progressbar(U8G2 u8g2, int x, int y, int w, int h, float value);
void API_Boot(U8G2 u8g2);
void API_FirstPage(U8G2 u8g2);
void API_WeatherNow(U8G2 u8g2);
void API_WeatherTomorrow(U8G2 u8g2);

void API_CheckUpdate(U8G2 u8g2);


#endif