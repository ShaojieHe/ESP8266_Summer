#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <TimerInterrupt_Generic.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wifi_Func/Wifi_Func.hpp>

extern String SSID, Passwd;

void WiFi_Connect(void)
{
  //delay(1000);
  
  EEPROM.begin(128);

  if (EEPROM.read(0) == 0xAE && EEPROM.read(1) == 0xAC)
  {
    Serial.println("Wifi is Configured -- Trying To Connect");
    if (!WiFi_ReadConfig(SSID, Passwd))
    {
      //First Check The Old Wifi SSID is Present
      //If not, We will fallback to SmartConfig Mode And Write New Config Into Storage
      int Result = WiFi.scanNetworks(false, true, 0, (uint8 *)SSID.c_str());

      Serial.printf("Detected Perivous WiFi? %s \n", Result ? "Yes" : "No");

      WiFi.begin(SSID, Passwd);
      if (Result)
      {
        while (1)
        {
          Serial.print(">");
          WiFi_FlashLED(100);
          if (WiFi.isConnected())
            break;
        }
      }
      else
      {
        //handle When Perivous Wifi is not Found
        //now We just Simply Reconnect -- We will find A way to Handle This
        int i = 60;
        while (i--)
        {
          delay(100);
          Result = WiFi.scanNetworks(false, true, 0, (uint8 *)SSID.c_str());
          Serial.printf("Now Waiting %d seconds remaining\n",i);
          if (Result)
          {
            delay(100);
            WiFi.begin(SSID, Passwd);
            Serial.println("Reconnecting!");
            while (1)
            {
              Serial.print(">");
              WiFi_FlashLED(100);
              if (WiFi.isConnected())
              {
                WiFi_AfterConnect();
                return;
              }  
            }
          }
        }
        //Reconfigure WiFi
        WiFi_Reconfig();
      }
    }
  }
  else
  {
    Serial.println("Wifi *NOT* Configured");
    Serial.println("Using Smartconfig");
    WiFi_SmartConfig(SSID, Passwd);

    WiFi_WipeConfig();
    WiFi_WriteConfig(SSID, Passwd);
  }

  WiFi_AfterConnect();
  digitalWrite(2, 0);
  EEPROM.end();
}

void WiFi_SmartConfig(String &SSID, String &Passwd)
{
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.printf(">");
    WiFi_FlashLED(700);

    if (WiFi.isConnected())
    {
      SSID = WiFi.SSID();
      Passwd = WiFi.psk();
      break;
    }
  }
  digitalWrite(2, 1);
}

void WiFi_WriteConfig(String &SSID, String &Passwd)
{
  int i, length, count;

  EEPROM.write(1, 0xAC);

  length = SSID.length() + 2; //Skip Flag Bytes
  for (i = 2; i < length + 1; i++)
  {
    EEPROM.write(i, SSID.c_str()[i - 2]);
    //Serial.print(SSID.c_str()[i - 2]);
  }
  length = Passwd.length() + i;

  for (count = 0; count < length; count++, i++)
  {
    EEPROM.write(i, Passwd.c_str()[count]);
    //Serial.print(Passwd.c_str()[count]);
  }
  EEPROM.commit();
}

void WiFi_WipeConfig(void)
{
  int i;
  //Wipe EEPROM Section
  for (i = 0; i < 128; i++)
    EEPROM.write(i, 0x00);
  EEPROM.write(0, 0xAE);

  EEPROM.commit();
}

bool WiFi_ReadConfig(String &SSID, String &Passwd)
{
  int i = 2;
  char buf;

  SSID.remove(0, SSID.length());
  Passwd.remove(0, Passwd.length());

  if (EEPROM.read(0) != 0xAE || EEPROM.read(1) != 0xAC)
    return 1;
  else
  {
    buf = EEPROM.read(i);
    while (buf != '\0')
    {
      SSID += buf;
      i++;
      buf = EEPROM.read(i);
    }
    //Serial.println("Now We get the ssid");
    //Serial.println(SSID);

    buf = EEPROM.read(++i);
    while (buf != '\0')
    {
      Passwd += buf;
      i++;
      buf = EEPROM.read(i);
    }
    //Serial.println("Now We get the Passwd");
    //Serial.println(Passwd);
    return 0;
  }
}

String WiFi_GetIPAddress(void)
{
  WiFiClient wifi_http;
  HTTPClient Public_IP;
  Public_IP.begin(wifi_http, IP_URL);

  int returnCode = Public_IP.GET();

  if (returnCode == HTTP_CODE_OK)
  {
    return Public_IP.getString();
  }
  else
  {
    return "NaN";
  }

  Public_IP.end();
}

void WiFi_AfterConnect(void)
{
  Serial.printf("\nWifi Connected!\nSSID is %s \n", SSID.c_str());
  Serial.print("Local ip address is ");
  Serial.println(WiFi.localIP());
}

void WiFi_FlashLED(u16_t ms)
{
  digitalWrite(2, 1);
  delay(ms);
  digitalWrite(2, 0);
  delay(ms);
}

void IRAM_ATTR WiFi_Check(void)
{
  volatile wl_status_t Result = WiFi.status();
  static bool LastStatus = true;

  Serial.printf("WiFi Status: %s\n", (Result == WL_CONNECTED) ? "Connected" : "Disconnected");

  if (Result != WL_CONNECTED)
  {
    LastStatus = false;

    Serial.println("Wifi disconnected, Trying Reconnect!");
    WiFi.disconnect(true);

    WiFi.begin(SSID, Passwd);
  }
  else
  {
    if (!LastStatus)
    {
      WiFi_AfterConnect();
      LastStatus = true;
    }
  }
}

void WiFi_Reconfig(void)
{
  Serial.println("Now Attempting Reconfigure Wifi");
  WiFi_SmartConfig(SSID, Passwd);
  Serial.println("WiFi Reconfigured -- Now Writing New Config");
  EEPROM.begin(128);
  WiFi_WipeConfig();
  WiFi_WriteConfig(SSID, Passwd);
  EEPROM.end();
}