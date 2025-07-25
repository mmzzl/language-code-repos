//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
#include <WiFi.h>
#include <WiFiUDP.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <network.h>


//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装

//根据需要修改的信息
String type = "002"; //设备类型，001插座设备，002灯类设备，003风扇设备,005空调，006开关，009窗帘
String Name= "台灯"; //设备昵称，可随意修改
String proto= "3"; //3是tcp设备端口8344,1是MQTT设备
char packetBuffer[255];



char config_flag = 0;//判断是否配网
#define MAGIC_NUMBER 0xAA //判断是否配网
WiFiUDP Udp;



void NetworkManager::loadConfig() {
    uint8_t mac[6];
    Serial.println("LoadConfig.......");
    WiFi.macAddress(mac);
    
    // 使用成员变量 config
    uint8_t *p = (uint8_t*)(&this->config);
    
    // 动态计算 EEPROM 大小
    size_t configSize = sizeof(config_type);
    EEPROM.begin(configSize);
    
    for (int i = 0; i < configSize; i++) {
        *(p + i) = EEPROM.read(i);
    }
    
    this->config.reboot = this->config.reboot + 1;
    
    if(this->config.reboot >= 4) {
        restoreFactory();
    }
    
    // 验证魔术字
    if(this->config.magic != MAGIC_NUMBER) {
        config_flag = 1;
    }
    
    // 更新 EEPROM
    EEPROM.begin(configSize);
    for (int i = 0; i < configSize; i++) {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
    
    Serial.println("loadConfig Over");
    
    // 重置重启计数
    this->config.reboot = 0;
    EEPROM.begin(configSize);
    for (int i = 0; i < configSize; i++) {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
}



/* 
 * 恢复出厂设置
*/
void NetworkManager::restoreFactory()
{
  Serial.println("\r\n Restore Factory....... ");
  config.magic = 0x00;
  strcpy(config.stassid, "");
  strcpy(config.stapsw, "");
  strcpy(config.cuid, "");
  strcpy(config.ctopic, "");
  config.magic = 0x00;
  saveConfig();
  ESP.restart();
  // delayRestart(1);
  while (1) {
    delay(100);
  }
}

/*
保存WIFI信息
*/
void NetworkManager::saveConfig()
{
  config.reboot = 0;
  size_t configSize = sizeof(config_type); // 动态计算大小
  EEPROM.begin(configSize);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}

void NetworkManager::apConfig(String mac) {
  if (config_flag == 1) {
      WiFi.softAP("bemfa_" + mac);
      Udp.begin(8266);
      Serial.println("Started Ap Config...");
  }
  String topic = mac + type;
  while (config_flag) { // 如果未配网，开启 AP 配网，并接收配网信息
      int packetSize = Udp.parsePacket();
      if (packetSize) {
          Serial.print("Received packet of size ");
          Serial.println(packetSize);
          Serial.print("From ");
          IPAddress remoteIp = Udp.remoteIP();
          Serial.print(remoteIp);
          Serial.print(", port ");
          Serial.println(Udp.remotePort());

          int len = Udp.read(packetBuffer, 255);
          if (len > 0) {
              packetBuffer[len] = 0;
          }
          Serial.println("Contents:");
          Serial.println(packetBuffer);

          // 修改：使用 DynamicJsonDocument
          DynamicJsonDocument doc(200);

          DeserializationError error = deserializeJson(doc, packetBuffer);
          if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              return;
          }
          int cmdType = doc["cmdType"].as<int>();

          if (cmdType == 1) {
              const char* ssid = doc["ssid"];
              const char* password = doc["password"];
              const char* token = doc["token"];
              //const char* topic = doc["topic"];
              Serial.println(cmdType);
              Serial.println(ssid);
              Serial.println(password);
              Serial.println(token);
              strcpy(config.stassid, ssid);
              strcpy(config.stapsw, password);
              strcpy(config.cuid, token);
              config.reboot = 0;
              config.magic = 0xAA;
              saveConfig();
              // 收到信息，并回复
              String  ReplyBuffer = "{\"cmdType\":2,\"productId\":\"" + topic + "\",\"deviceName\":\"" + Name + "\",\"protoVersion\":\"" + proto + "\"}";
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
              Udp.write((const uint8_t *)ReplyBuffer.c_str(), ReplyBuffer.length());
              Udp.endPacket();

          } else if (cmdType == 3) {
              config_flag = 0;
              WiFi.softAPdisconnect(true);
          }
      }
  }
}

void NetworkManager::reconnectWiFi() {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(config.stassid, config.stapsw);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}