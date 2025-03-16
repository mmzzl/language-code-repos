#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "IRremoteESP8266.h"
#include <IRrecv.h>
#include <IRutils.h>
#include "network.h"



#define DECODE_NEC          // Includes Apple and Onkyo
#define DECODE_DISTANCE_WIDTH // In case NEC is not received correctly. Universal decoder for pulse distance width protocols
#define DELAY_AFTER_SEND 2000
#define DELAY_AFTER_LOOP 5000

// 定义红外接收引脚
const uint16_t IR_RECEIVE_PIN = 4;
IRrecv irrecv(IR_RECEIVE_PIN);   // 定义红外遥控接收器对象并传入使用的引脚，库会根据传入引脚编号自动初始化接收功能
 
decode_results result; 

const int ledPin = 12;
const int ledPin2 = 0;
const int buttonPin = 2;
const char* mqtt_server = "bemfa.com";
const int mqtt_port = 9501;
const char* client_id = "ead3b3d5ed5242a68fe762c87929f11b";
int ledBrightness = 0;  // 统一使用 ledBrightness 表示亮度，避免混淆
const int MAX_RECEIVED_DATA = 10;
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
void callback(char* topic, byte* payload, unsigned int length);
void splitString(const String& input, char* parts[], int maxParts);
void led();

PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

uint16_t sAddress = 0x0102;
uint8_t sCommand = 0x34;
uint8_t sRepeats = 1;

unsigned long receivedData[MAX_RECEIVED_DATA];
int dataIndex = 0;
int state = 0;

void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    // 初始化LEDC
    analogWriteFreq(23000); // 频率设置为23khz
    analogWriteRange(255);
    // 配置外部中断
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, CHANGE);
    String topic = WiFi.macAddress().substring(8); // 取mac地址做主题用
    topic.replace(":", ""); // 去掉:号
    // 这里假设 loadConfig 和 apConfig 函数存在且正确实现
    loadConfig();//加载存储的数据
    apConfig(topic);//加载ap

    irrecv.enableIRIn();  // Start the receiver

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);

    WiFi.disconnect(); // 断开连接
    WiFi.mode(WIFI_STA); // STA模式
    Serial.println("Connecting to WiFi...");
    // 这里假设 config 结构体存在且正确初始化
    Serial.println(config.stassid);
    Serial.println(config.stapsw);
    WiFi.begin(config.stassid, config.stapsw); // 连接路由器
    while (WiFi.status() != WL_CONNECTED) { // 检查是否连接成功
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    led();
    if (client.connect(client_id)) {
        Serial.println("MQTT connected");
        client.subscribe(mqtt_topic);
    } else {
        Serial.println("MQTT connection failed");
    }
}

void loop() {
    client.loop();
    // 检查是否接收到红外信号
    if (irrecv.decode(&result)) {
        // print() & println() can't handle printing long longs. (uint64_t)
        serialPrintUint64(result.value, HEX);
        switch (result.value)
        {
        case 0x1FE50AF:
            /* code */
            ledBrightness = 50;
            break;
        case 0x1FEF807:
            ledBrightness = 0;
            break;
        case 0x1FEE01F:
            ledBrightness += 10;
            if (ledBrightness >= 100) {
                ledBrightness = 100;
            }
            break;
        case 0x1FE906F:
            ledBrightness -= 10;
            if (ledBrightness <= 0) {
                ledBrightness = 0;
            }
            break;
        default:
            break;
        }
        Serial.println("");
        irrecv.resume();  // Receive the next value
    }
    switch (state) {
        case 0:
            analogWrite(ledPin2, map(ledBrightness, 0, 100, 0, 255));
            analogWrite(ledPin, map(0, 0, 100, 0, 255));
            break;
        case 1:
            analogWrite(ledPin, map(ledBrightness, 0, 100, 0, 255));
            analogWrite(ledPin2, map(0, 0, 100, 0, 255));
            break;
        case 2:
            analogWrite(ledPin, map(ledBrightness, 0, 100, 0, 255));
            analogWrite(ledPin2, map(ledBrightness, 0, 100, 0, 255));
            break;
    }
    
    delay(10);
}

void led() {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
}

//中断服务程序（ISR）
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
                handleLongPress();
                longPressStartTime = 0;  // 处理完长按后重置长按开始时间
            }
        } else {
            if (longPressStartTime != 0) {
                if (currentTime - longPressStartTime < longPressThreshold) {
                    Serial.println("Button Short Pressed");
                    handleShortPress();
                }
            } else {
                Serial.println("Button Released");
            }
            longPressStartTime = 0;  // 按键松开时重置长按开始时间
        }
        lastButtonState = buttonState;
    }
}

// 处理短按的函数
void handleShortPress() {
    Serial.println("Handling short press.");
    num++;
    switch (num) {
        case 1:
            ledBrightness = 100 / 4;
            break;
        case 2:
            ledBrightness = 100 / 3;
            break;
        case 3:
            ledBrightness = 100 / 2;
            break;
        case 4:
            ledBrightness = 100;
            break;
        case 5:
            ledBrightness = 0;
            num = 0;
            state = (state+1) % 3;
            break;
        default:
            break;
    }
}

// 处理长按的函数
void handleLongPress() {
    Serial.println("Handling long press.");
    // 在此处添加按键长按的处理逻辑
}

// MQTT 回调函数
void callback(char* topic, byte* payload, unsigned int length) {
    char* parts[2];
    if (strcmp(topic, mqtt_topic) == 0) {
        String message = "";
        for (unsigned int i = 0; i < length; i++) {
            message += (char)payload[i];
        }
        Serial.println("Received message: " + message);
        splitString(message, parts, 2);
        if (parts[0] != nullptr) {
            if (strcmp(parts[0], "on") == 0) {
                Serial.println("Received wake up signal via mqtt!");
                if (parts[1] != nullptr) {
                    ledBrightness = atoi(parts[1]);
                } else {
                    ledBrightness = 50;
                }
            } else if (strcmp(parts[0], "off") == 0) {
                ledBrightness = 0;
                state = (state+1) % 3;
            }
        }
        Serial.println("LED brightness set to: " + String(ledBrightness));
        // 释放动态分配的内存
        for (int i = 0; i < 2; i++) {
            if (parts[i] != nullptr) {
                delete[] parts[i];
            }
        }
    }
}

// 分割字符串函数
void splitString(const String& input, char* parts[], int maxParts) {
    int index = 0;
    int startIndex = 0;
    int partCount = 0;
    index = input.indexOf('#');
    if (index == -1) {
        // 如果没有分隔符，将整个字符串作为一个部分
        parts[0] = new char[input.length() + 1];
        strcpy(parts[0], input.c_str());
        partCount = 1;
    } else {
        while (true) {
            if (partCount >= maxParts) {
                break;
            }
            String part = input.substring(startIndex, index);
            parts[partCount] = new char[part.length() + 1];
            strcpy(parts[partCount], part.c_str());
            partCount++;
            startIndex = index + 1;
            index = input.indexOf('#', startIndex);
            if (index == -1) {
                // 存储最后一部分
                part = input.substring(startIndex);
                if (partCount < maxParts) {
                    parts[partCount] = new char[part.length() + 1];
                    strcpy(parts[partCount], part.c_str());
                    partCount++;
                }
                break;
            }
        }
    }
    // 填充剩余的部分为 nullptr
    for (int i = partCount; i < maxParts; i++) {
        parts[i] = nullptr;
    }
}