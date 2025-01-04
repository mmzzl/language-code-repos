#include "network.h"

void Network::init()
{
    loadConfig();
    connectSta(config.stdssid, config.stapsw);
    if (!flag)
    {
        Serial.print("***上次没有配网成功，执行一键配网功能");
        Serial.print("/n");
        smartConnect();
    }
    
}

void Network::saveConfig()
{
    Serial.println("save config!");
    Serial.print("stassid: ");
    Serial.println(config.stdssid);
    Serial.print("stapsw: ");
    Serial.println(config.stapsw);
    EEPROM.begin(1024);
    uint8_t *p =  (uint8_t*)(&config);
    for (uint8_t i=0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *(p+i));
    }
    EEPROM.commit();
}

void Network::loadConfig()
{
    EEPROM.begin(1024);
    uint8_t *p = (uint8_t*)(&config);
    for (uint8_t i=0; i < sizeof(config); i++)
    {
        *(p+i) = EEPROM.read(i);
    }
    EEPROM.commit();
    Serial.println("------Read Config----------");
    Serial.print("stassid: ");
    Serial.println(config.stdssid);
    Serial.print("stapsw: ");
    Serial.println(config.stapsw);
}

void Network::connectSta(const char *stdssid, const char *psw)
{
    WiFi.begin(stdssid, psw);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        timeVal ++;
        Serial.print(".");
        Serial.print(WiFi.status());
        if (timeVal > 20)
        {
            timeVal = 0;
            flag = 0;
            return;
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        flag = 1;
        //输出mac地址
        Serial.println(String("Connected, mac address: ") + WiFi.macAddress().c_str());
        //输出station IP地址，这里的IP地址由DHCP分配
        Serial.println(String("Connected, IP address: ") + WiFi.localIP().toString());

    }
}

void Network::smartConnect()
{
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    delay(2000);
    WiFi.beginSmartConfig();
    while(1)
    {
        Serial.print(".");
        delay(500);
        timeVal ++;
        if (timeVal > 200)
        {
            timeVal = 0;
            flag = 0;
            break;
        }
        if (WiFi.smartConfigDone())
        {
            Serial.println("SmartConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            WiFi.setAutoConnect(true);
            strcpy(config.stdssid, WiFi.SSID().c_str());
            strcpy(config.stapsw, WiFi.psk().c_str());
            saveConfig();
            break;
        }
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("Ip address: ");
    Serial.println(WiFi.localIP().toString());
}