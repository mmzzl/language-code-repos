#include <Arduino.h>
#include <network.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const int ledPin = 13;
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
void callback(char* topic, byte* payload, unsigned int length);
bool readLedStateFromEEPROM();
void saveLedStateEEPROM(const bool ledState);
void splitString(String input, String parts[], int maxParts);
void handleShortPress();
void handleLongPress();
void IRAM_ATTR buttonISR();
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
//   network_init();
//   client.setServer(mqtt_server, mqtt_port);
//   client.setCallback(callback);
//   if (client.connect(client_id)) {
//     Serial.println("Connected to MQTT broker");
//     client.subscribe(mqtt_topic);
//   } else {
//     Serial.println("Failed to connect to MQTT broker");
//   }
//   // 读取 LED 状态并更新 LED
//   ledState = readLedStateFromEEPROM();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Blinker.run();
    // start_server();
    if (!ledState) {
        ledBrightness = map(analogRead(A0), 0, 1023, 0, 255);
        if (ledBrightness < 5) {
            ledBrightness = 0;
        }
        analogWrite(ledPin, ledBrightness);
    } else {
        analogWrite(ledPin, ledBrightness);
    }

    // if (client.connected()) {
    //     client.loop();
    // } 
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
      }
    } else {
      if (longPressStartTime!= 0) {
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



void callback(char* topic, byte* payload, unsigned int length) {
  String parts[2];
  if (strcmp(topic, mqtt_topic) == 0) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
      message += (char)payload[i];
    }
    Serial.println("Received message: " + message);
    splitString(message, parts, 2);
    if (parts[0] == "on") {
      ledState = true;
      saveLedStateEEPROM(ledState);
      if (parts->length() > 1) {
        light = parts[1].toInt();
      }
    } else if (parts[0] == "off") {
      ledState = false;
      saveLedStateEEPROM(ledState);
    }
    updateLed();
  }
}


bool readLedStateFromEEPROM() {
  byte state = EEPROM.read(EEPROM_LED_START_ADDR);
  return state == 1;
}

void saveLedStateEEPROM(const bool ledState) {
  byte state = ledState ? 1 : 0;  // 将 bool 转换为 byte 类型，true 为 1，false 为 0
  EEPROM.write(EEPROM_LED_START_ADDR, state);
  EEPROM.commit();
  Serial.println("Saved LED state to EEPROM");
}

void splitString(String input, String parts[], int maxParts) {
  int index = 0;
  int startIndex = 0;
  int partCount = 0;
  index = input.indexOf('#');
  if (index == -1) {
    // 如果没有分隔符，将整个字符串作为一个部分
    parts[0] = input;
    partCount = 1;
  } else {
    while (true) {
      parts[partCount++] = input.substring(startIndex, index);
      startIndex = index + 1;
      index = input.indexOf('#', startIndex);
      if (index == -1) {
        // 存储最后一部分
        parts[partCount++] = input.substring(startIndex);
        break;
      }
      if (partCount >= maxParts) {
        break;
      }
    }
  }
}