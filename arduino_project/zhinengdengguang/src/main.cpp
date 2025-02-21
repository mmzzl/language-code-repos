#include <Arduino.h>
#include <network.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


const int ledPin = 12;
const int buttonPin = 2;
const char *mqtt_server = "bemfa.com"; 
const int mqtt_port = 9501;
const char *client_id = "ead3b3d5ed5242a68fe762c87929f11b";
uint16_t light = 100;
// 存储 MQTT 消息控制的 LED 状态
bool ledState = false;
// 存储 ADC 控制的 LED 亮度
int ledBrightness = 0;
// 按键次数
uint8_t num = 0;
const char* mqtt_topic = "led002";
WiFiClient espClient;
// 存储按键的当前状态和之前的状态
volatile bool buttonState = HIGH;
volatile bool lastButtonState = HIGH;
// 存储上一次按键状态改变的时间
volatile unsigned long lastButtonChangeTime = 0;
// 存储长按开始时间
volatile unsigned long longPressStartTime = 0;
// 去抖动的时间间隔（毫秒）
const unsigned long debounceInterval = 100;
// 长按的时间阈值（毫秒）
const unsigned long longPressThreshold = 1000;
void handleShortPress();
void handleLongPress();
void IRAM_ATTR buttonISR();
void callback(char *topic, byte *payload, unsigned int length);
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(A0, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // 初始化LEDC
  analogWriteFreq(23000); // 频率设置为23khz
  analogWriteRange(255);
  // 配置外部中断
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, CHANGE);
  String topic = WiFi.macAddress().substring(8);//取mac地址做主题用
  topic.replace(":", "");//去掉:号
  loadConfig();//加载存储的数据
  apConfig(topic);//加载ap

  WiFi.disconnect();//断开连接
  WiFi.mode(WIFI_STA);//STA模式
  Serial.println("Connecting to WiFi...");
  Serial.println(config.stdssid);
  Serial.println(config.stapsw);
  WiFi.begin(config.stassid, config.stapsw); // 连接路由器
  while (WiFi.status() != WL_CONNECTED) {//检查是否连接成功
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  if (client.connect(client_id)) {
    client.publish("outTopic","hello world");
    client.subscribe(mqtt_topic);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  
}

void loop() {
  client.loop();  
  analogWrite(ledPin, ledBrightness);
}

void updateLed() {
  if (!ledState) {
    // 将 ADC 值映射到 PWM 范围（0-1023）
    ledBrightness = map(analogRead(A0), 0, 100, 0, 255);
    analogWrite(ledPin, ledBrightness);
  } else {
    ledBrightness = map(light, 0, 1023, 0, 255);
    analogWrite(ledPin, ledBrightness);
  }
}
// 中断服务程序（ISR）
void IRAM_ATTR buttonISR() {
  unsigned long currentTime = millis();
  // 去抖动处理
  if (currentTime - lastButtonChangeTime >= debounceInterval) {
    lastButtonChangeTime = currentTime;
    buttonState = digitalRead(buttonPin);


    if (buttonState == LOW) {
      if (longPressStartTime == 0) {
        longPressStartTime = currentTime;
      } else if (currentTime - longPressStartTime >= longPressThreshold) {
        Serial.println("Button Long Pressed");
        // 在此处添加按键长按的处理逻辑
        handleLongPress();
      } else if (currentTime - longPressStartTime < longPressThreshold) {
        Serial.println("Button Short Pressed");
        // 在此处添加按键短按的处理逻辑
        handleShortPress();
      }
    } else {
      if (longPressStartTime != 0) {
        if (currentTime - longPressStartTime < longPressThreshold) {
          Serial.println("Button Short Pressed");
          // 在此处添加按键短按的处理逻辑
          handleShortPress();
        }
        longPressStartTime = 0;
      }
    }
    lastButtonState = buttonState;
  }
}

// 处理短按的函数
void handleShortPress() {
  Serial.println("Handling short press.");
  num ++;
  if (num == 1) {
    ledState = true;
    ledBrightness = 255 / 4;
  } else if (num == 2) {
    ledState = true;
    ledBrightness = 255 / 3;
  } else if (num == 3) {
    ledState = true;
    ledBrightness = 255 / 2;
  } else if (num == 4) {
    ledState = true;
    ledBrightness = 255;
  } else if (num == 5) {
    ledState = false;
    ledBrightness = 0;
    num = 0;
  }
}


// 处理长按的函数
void handleLongPress() {
  Serial.println("Handling long press.");
}







