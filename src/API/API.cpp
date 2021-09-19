#include <Arduino.h>
#include <ESP8266_Heweather.h>
#include "API/API.hpp"
#include <NTPClient.h>
#include <icon/icon.h>
#include <U8g2lib.h>
#include <Wifi_Func/Wifi_Func.hpp>

WeatherNow API_Now;
WeatherForecast API_Forcast;
AirQuality API_Quality;
extern NTPClient TimeClient;

bool API_UpdateFlag = false;

void API_Configure(void)
{
    API_Now.config(Key, Location, "m", "zh");
    API_Forcast.config(Key, Location, "m", "zh");
    API_Quality.config(Key, Location, "m", "zh");
}

bool API_GetAll(void)
{
    bool Result = true;
    Result &= API_Now.get();
    Result &= API_Forcast.get();
    Result &= API_Quality.get();

    API_UpdateFlag = true;
    return Result;
}

uint16_t API_GetLastUpdateTime(uint16_t API_Type)
{
    if (!API_UpdateFlag)
        return 0;
    String LastUpdate;
    uint16_t Time, Hour, Minute;

    switch (API_Type)
    {
    case 0:
        LastUpdate = API_Now.getLastUpdate();
        break;
    case 1:
        LastUpdate = API_Forcast.getLastUpdate();
        break;
    case 2:
        LastUpdate = API_Quality.getLastUpdate();
        break;
    default:
        return 0;
        break;
    }
    //Serial.println(LastUpdate);

    Hour = (LastUpdate.substring(LastUpdate.indexOf('T') + 1, LastUpdate.indexOf('T') + 3)).toInt();
    Minute = (LastUpdate.substring(LastUpdate.indexOf(':') + 1, LastUpdate.indexOf(':') + 3)).toInt();

    Time = (TimeClient.getHours() - Hour) * 60 + (TimeClient.getMinutes() - Minute);

    return Time;
}

const unsigned char *API_ReturnAddress(unsigned ICON)
{
    switch (ICON)
    {
    case 100:
        return bits_100;
        break;
    case 101:
        return bits_101;
        break;
    case 102:
        return bits_102;
        break;
    case 103:
        return bits_103;
        break;
    case 104:
        return bits_104;
        break;
    case 150:
        return bits_150;
        break;
    case 153:
        return bits_153;
        break;
    case 154:
        return bits_154;
        break;
    case 300:
        return bits_300;
        break;
    case 301:
        return bits_301;
        break;
    case 302:
        return bits_302;
        break;
    case 303:
        return bits_303;
        break;
    case 304:
        return bits_304;
        break;
    case 305:
        return bits_305;
        break;
    case 306:
        return bits_306;
        break;
    case 307:
        return bits_307;
        break;
    case 308:
        return bits_308;
        break;
    case 309:
        return bits_309;
        break;
    case 310:
        return bits_310;
        break;
    case 311:
        return bits_311;
        break;
    case 312:
        return bits_312;
        break;
    case 313:
        return bits_313;
        break;
    case 314:
        return bits_314;
        break;
    case 315:
        return bits_315;
        break;
    case 316:
        return bits_316;
        break;
    case 317:
        return bits_317;
        break;
    case 318:
        return bits_318;
        break;
    case 350:
        return bits_350;
        break;
    case 351:
        return bits_351;
        break;
    case 399:
        return bits_399;
        break;
    case 400:
        return bits_400;
        break;
    case 401:
        return bits_401;
        break;
    case 402:
        return bits_402;
        break;
    case 403:
        return bits_403;
        break;
    case 404:
        return bits_404;
        break;
    case 405:
        return bits_405;
        break;
    case 406:
        return bits_406;
        break;
    case 407:
        return bits_407;
        break;
    case 408:
        return bits_408;
        break;
    case 409:
        return bits_409;
        break;
    case 410:
        return bits_410;
        break;
    case 456:
        return bits_456;
        break;
    case 457:
        return bits_457;
        break;
    case 499:
        return bits_499;
        break;
    case 500:
        return bits_500;
        break;
    case 501:
        return bits_501;
        break;
    case 502:
        return bits_502;
        break;
    case 503:
        return bits_503;
        break;
    case 504:
        return bits_504;
        break;
    case 507:
        return bits_507;
        break;
    case 508:
        return bits_508;
        break;
    case 509:
        return bits_509;
        break;
    case 510:
        return bits_510;
        break;
    case 511:
        return bits_511;
        break;
    case 512:
        return bits_512;
        break;
    case 513:
        return bits_513;
        break;
    case 514:
        return bits_514;
        break;
    case 515:
        return bits_515;
        break;
    case 900:
        return bits_900;
        break;
    case 901:
        return bits_901;
        break;

    default:
        return bits_999;
        break;
    }
    return bits_999;
}

String API_NextEvent(void)
{
    int Hour = TimeClient.getHours(), Minute = TimeClient.getMinutes();
    int TestHour, TestMinute;
    int i;
    bool EqualHour = false;

    String ReturnValue = "NaN";
    String Array[] = {API_Forcast.getSunRise(0), API_Forcast.getSunSet(0), API_Forcast.getSunRise(1)};
    //今天还没日升 今天还没日落  今天日落了但是明天的日还没升

    //Array[0].remove(2);
    for (i = 0; i < 2; i++)
    {
        TestHour = Array[i].substring(0, 2).toInt();
        TestMinute = Array[i].substring(3, 5).toInt();
        if ((Hour < TestHour) || (TestHour == Hour && Minute < TestMinute))
        {
            EqualHour = (TestHour == Hour);
            break;
        }
    }
    switch (i)
    {
    case 0:
        //return "还没日升";
        if (EqualHour)
            ReturnValue = "还有" + String((int)(TestMinute - Minute)) + "分钟日出";
        else
            ReturnValue = "还有约" + String((int)(TestHour - Hour)) + "小时日出";
        break;
    case 1:
        //return "还没日落";
        if (EqualHour)
            ReturnValue = "还有" + String((int)(TestMinute - Minute)) + "分钟日落";
        else
            ReturnValue = "还有约" + String((int)(TestHour - Hour)) + "小时日落";
        break;

    case 2:
        //return "今天日落了但是明天还没日出";
        ReturnValue = "还有约" + String((int)((24 - Hour) + Array[2].substring(0, 2).toInt())) + "小时日出";
        break;
    }
    return ReturnValue;
}

void API_TestIcon(U8G2 u8g2)
{
    String Test1;
    int i = 0;
    unsigned int a[] = {
        100,
        101,
        102,
        103,
        104,
        150,
        153,
        154,
        300,
        301,
        302,
        303,
        304,
        305,
        306,
        307,
        308,
        309,
        310,
        311,
        312,
        313,
        314,
        315,
        316,
        317,
        318,
        350,
        351,
        399,
        400,
        401,
        402,
        403,
        404,
        405,
        406,
        407,
        408,
        409,
        410,
        456,
        457,
        499,
        500,
        501,
        502,
        503,
        504,
        507,
        508,
        509,
        510,
        511,
        512,
        513,
        514,
        515,
        900,
        901,
        999,
    };
    while (1)
    {
        Test1 = "ID:";
        Test1.concat(a[i]);
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(64, 12, "图标测试");
            u8g2.drawUTF8(64, 24, Test1.c_str());
            u8g2.drawXBMP(0, 0, 64, 64, API_ReturnAddress(a[i]));
            API_Progressbar(u8g2, 64, 34, 64, 10, (float)(i / 60.0));
        } while (u8g2.nextPage());
        i = i + 1;
        if (i > (sizeof(a) / sizeof(a[0])) - 1)
        {
            //delay(5000);
            break;
        }
        delay(200);
    }
}

void API_Progressbar(U8G2 u8g2, int x, int y, int w, int h, float value)
{
    u8g2.drawFrame(x, y, w, h);
    u8g2.drawBox(x + 2, y + 2, (w - 4) * value, h - 4);
}

void API_Boot(U8G2 u8g2)
{
    float i = 0.00;
    extern String SSID;
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);

    bool Result = !digitalRead(14);
    //Serial.printf("\nPin8 Status: %s\n", Result ? "Pressed" : "Not Pressed");

    u8g2.clear();
    u8g2.drawUTF8(0, 12, (Result) ? "正在重新配置WiFi" : "正在连接WiFi");
    u8g2.drawUTF8(0, 24, (Result) ? "请使用微信公众号" : "");
    u8g2.drawUTF8(0, 36, (Result) ? "安信可科技-配网功能" : "");
    u8g2.sendBuffer();

    if (Result)
        WiFi_Reconfig();
    else
        WiFi_Connect();

    String IP = "Local IP: " + WiFi.localIP().toString();
    String PublicIP = "Pub IP: " + WiFi_GetIPAddress();

    for (i = 0; i < 0.5; i = i + 0.01)
    {
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(0, 12, "连接到: ");
            u8g2.drawUTF8(0, 24, SSID.c_str());
            u8g2.drawUTF8(0, 36, IP.c_str());
            u8g2.drawUTF8(0, 48, PublicIP.c_str());
            API_Progressbar(u8g2, 0, 54, 128, 10, i);
        } while (u8g2.nextPage());
        delay(50);
    }

    TimeClient.begin();
    TimeClient.update();
    //Serial.println(TimeClient.getFormattedTime());
    API_Configure();
    API_GetAll();
    u8g2.clear();

    for (; i < 1; i = i + 0.01)
    {
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(0, 12, "获取天气数据");
            u8g2.drawUTF8(0, 24, "数据源:和风天气");
            u8g2.drawUTF8(0, 36, "现在时间");
            u8g2.drawUTF8(0, 48, TimeClient.getFormattedTime().c_str());
            API_Progressbar(u8g2, 0, 54, 128, 10, i);
        } while (u8g2.nextPage());
        delay(50);
    }
    Serial.println(API_NextEvent());
    u8g2.clear();
}

void API_FirstPage(U8G2 u8g2)
{
    u8g2.clear();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
    float i = 0.00;
    String NextEvent = API_NextEvent();
    String Time = "现在时间 " + TimeClient.getFormattedTime();
    String LastUpdate_now = "实时数据在" + String(API_GetLastUpdateTime(0)) + "分钟前更新";
    String LastUpdate_forcast = "预报数据在" + String(API_GetLastUpdateTime(1)) + "分钟前更新";
    for (i = 0.00; i < 1; i = i + 0.02)
    {
        u8g2.firstPage();
        Time = "现在时间 " + TimeClient.getFormattedTime();
        do
        {
            u8g2.drawUTF8(0, 12, Time.c_str());
            u8g2.drawUTF8(0, 24, NextEvent.c_str());
            u8g2.drawUTF8(0, 36, LastUpdate_now.c_str());
            u8g2.drawUTF8(0, 48, LastUpdate_forcast.c_str());
            API_Progressbar(u8g2, 0, 54, 128, 10, i);
        } while (u8g2.nextPage());
        delay(100);
    }
}

void API_WeatherNow(U8G2 u8g2)
{
    String WindLevel = (String("风力: ") + String(API_Now.getWindScale()) );
    String Temp = (String("温度:") + API_Now.getTemp() + String("°C"));
    String AQI = (String("AQI: ") + String(API_Quality.getAqi()));
    String Weather = API_Now.getWeatherText();
    u8g2.clear();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);

    u8g2.drawUTF8(40, 30, "现在天气");
    u8g2.sendBuffer();
    delay(700);

    float i = 0.00;
    for (i = 0.00; i < 1; i = i + 0.015)
    {
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(64, 12, Weather.c_str());
            u8g2.drawUTF8(64, 24, Temp.c_str());
            u8g2.drawUTF8(64, 36, WindLevel.c_str());
            u8g2.drawUTF8(64, 48, AQI.c_str());
            u8g2.drawXBMP(0, 0, 64, 64, API_ReturnAddress(API_Now.getIcon()));
            API_Progressbar(u8g2, 64, 54, 64, 10, i);
        } while (u8g2.nextPage());
        delay(100);
    }
}

void API_WeatherTomorrow(U8G2 u8g2)
{
    String Temp = (String("温度") + API_Forcast.getTempMin(1) + String("/") + API_Forcast.getTempMax(1) + String("°C"));
    String Weather = API_Forcast.getTextDay(1);
    u8g2.clear();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);

    u8g2.drawUTF8(40, 30, "明日天气");
    u8g2.sendBuffer();
    delay(700);

    float i = 0.00;
    for (i = 0.00; i < 1; i = i + 0.015)
    {
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(64, 12, Weather.c_str());
            u8g2.drawUTF8(64, 24, Temp.c_str());
            u8g2.drawXBMP(0, 0, 64, 64, API_ReturnAddress(API_Forcast.getIconDay(1)));
            API_Progressbar(u8g2, 64, 54, 64, 10, i);
        } while (u8g2.nextPage());
        delay(100);
    }
}

void API_CheckUpdate(U8G2 u8g2)
{
    float i;
    if(millis() % 600000)
    {
        return;
    }
    else
    {
    API_GetAll();
    for (i = 0.00; i < 1; i = i + 0.02)
    {
        u8g2.firstPage();
        do
        {
            u8g2.drawUTF8(0, 12, "数据已更新");
            API_Progressbar(u8g2, 0, 54, 128, 10, i);
        } while (u8g2.nextPage());
        delay(100);
    }
    }
}