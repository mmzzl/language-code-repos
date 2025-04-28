//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <network.h>

config_type config;

//需要在arduino IDE软件中---工具-->管理库-->搜索arduinojson并安装

//根据需要修改的信息
String type = "002"; //设备类型，001插座设备，002灯类设备，003风扇设备,005空调，006开关，009窗帘
String Name= "台灯"; //设备昵称，可随意修改
String proto= "3"; //3是tcp设备端口8344,1是MQTT设备




char config_flag = 0;//判断是否配网
#define MAGIC_NUMBER 0xAA //判断是否配网
char packetBuffer[255]; //发送数据包
WiFiUDP Udp;


/*
 * 从EEPROM加载参数
*/
uint8_t *p = (uint8_t*)(&config);
void loadConfig()
{

  uint8_t mac[6];
  Serial.println(" LoadConfig.......");
  WiFi.macAddress(mac);
  EEPROM.begin(512);
  for (int i = 0; i < sizeof(config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  config.reboot = config.reboot + 1;
  if(config.reboot>=4){
    restoreFactory();
  }
  if(config.magic != 0xAA){
    config_flag = 1;
  }
  EEPROM.begin(512);
  for (int i = 0; i < sizeof(config); i++){
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
  delay(2000);
  Serial.println("loadConfig Over");
  EEPROM.begin(512);
  config.reboot = 0;
  for (int i = 0; i < sizeof(config); i++){
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}


/* 
 * 恢复出厂设置
*/
void restoreFactory()
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
    ESP.wdtFeed();
    delay(100);
  }
}

/*
保存WIFI信息
*/
void saveConfig()
{
  config.reboot = 0;
  EEPROM.begin(2018);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}

void apConfig(String mac)
{
  if(config_flag == 1){
      WiFi.softAP("bemfa_"+mac);
      Udp.begin(8266);
      Serial.println("Started Ap Config...");
  }
  String topic = mac+type;
  while(config_flag){//如果未配网，开启AP配网，并接收配网信息
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
          StaticJsonDocument<200> doc;
      
          DeserializationError error = deserializeJson(doc, packetBuffer);
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
          }
          int cmdType = doc["cmdType"].as<int>();;
  
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
              //收到信息，并回复
              String  ReplyBuffer = "{\"cmdType\":2,\"productId\":\""+topic+"\",\"deviceName\":\""+Name+"\",\"protoVersion\":\""+proto+"\"}";
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
              Udp.write(ReplyBuffer.c_str());
              Udp.endPacket();
            
          }else if(cmdType == 3){
              config_flag = 0;
              WiFi.softAPdisconnect(true);
          }
          
        }
    
  }
}

