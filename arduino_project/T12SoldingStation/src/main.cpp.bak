#include <Arduino.h>
#include <PID_v1.h>
#include <avr/sleep.h>
#include "SevSeg.h"

#define P_MOSFET
#define ROTARY_TYPE 1
#define SENSOR_PIN A0 // 温度传感器
#define VIN_PIN A1 // 可调电阻输入电压
#define TEMP_MIN 150 // 最低温度
#define TEMP_MAX 400 // 最高温度
#define TEMP_DEFAULT 320  // 默认温度
#define TEMP_SLEEP 150 // 休眠温度
#define TEMP_BOOST 50 // 启动温度
#define TEMP_STEP 10 // 温度步进
#define TEMP200 216 // 200度
#define TEMP280 308 // 280度
#define TEMP360 390 // 300度
#define TIPMAX        8         // max number of tips
#define TEMPCHP 30 // 芯片温度
#define TIPNAMELENGTH 6 // 提示名称长度
#define TIPNAME "BC1.5"  // 提示名称
#define TIME2SLEEP 5  // 休眠时间
#define TIME2OFF 15 // 关闭时间
#define TIME2OFBOOST 40 // 启动时间
#define PID_ENABLE    false
#define CONTROL_PIN    3  // 控制引脚输出
#define SWITCH_PIN A4 // 手柄开关

 // sevseg 变量
 byte numDigits = 3;
 bool resistorsOnSegments = false;
 byte hardwareConfig = COMMON_CATHODE;
 bool updateWithDelays = false;
 bool leadingZeros = true;
 bool disableDecPoint = true;
 byte digitPins[] = {11, 12, 13};
 byte segmentPins[] = {2, 4, 6, 7, 8, 9, 10};

#if  defined (P_MOSFET) 
  #define HEATER_ON 255
  #define HEATER_OFF 0
  #define HEATER_PWM 255 - Output 
#elif defined (N_MOSFET)
  #define HEATER_ON 0
  #define HEATER_OFF 255
  #define HEATER_PWM Output
#else
  #error Wrong MOSFET type!
#endif
// pid 参数
double  aggKp=11, aggKi=0.5, aggKd=1;
double consKp=11, consKi=3, consKd=5;

uint16_t DefaultTemp = TEMP_DEFAULT;
uint16_t SleepTemp = TEMP_SLEEP;
uint16_t BoostTemp = TEMP_BOOST;
uint16_t time2sleep = TIME2SLEEP;
uint8_t time2off = TIME2OFF;
uint8_t timeOfBoost = TIME2OFBOOST;
bool PIDenable = PID_ENABLE;
// Default values for tips
uint16_t  CalTemp[TIPMAX][4] = {TEMP200, TEMP280, TEMP360, TEMPCHP};
char      TipName[TIPMAX][TIPNAMELENGTH] = {TIPNAME};
uint8_t   CurrentTip   = 0;
uint8_t   NumberOfTips = 1;


// State variables
bool      inSleepMode = false;
bool      inOffMode   = false;
bool      inBoostMode = false;
bool      inCalibMode = false;
bool      isWorky     = true;
bool      beepIfWorky = true;
bool      TipIsPresent= true;

// Timing variables
uint32_t  sleepmillis;
uint32_t  boostmillis;
uint32_t  buttonmillis;
uint8_t   goneMinutes;
uint8_t   goneSeconds;
uint8_t   SensorCounter = 255;

// Variables for pin change interrupt
volatile uint8_t  a0, b0, c0, d0;
volatile bool     ab0;
volatile int      count, countMin, countMax, countStep;
volatile bool     handleMoved;

// Variables for temperature control
uint16_t  SetTemp, ShowTemp, gap, Step;
double    Input, Output, Setpoint, RawTemp, CurrentTemp, ChipTemp;

// Variables for voltage readings
uint16_t  Vcc, Vin, LastVin;

SevSeg sevseg; // 数码管对象
PID ctrl(&Input, &Output, &Setpoint, aggKp, aggKi, aggKd, REVERSE);



void SevSegInit(); // 数码管初始化
void SLEEPCheck(); // 休眠检测

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(VIN_PIN, INPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  analogWrite(CONTROL_PIN, HEATER_OFF);
  sevseg_init();
  // 设置SWITCH_PIN 中断
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), SLEEPCheck, FALLING);
   // read supply voltages in mV
   Vcc = getVCC(); 
   Vin = getVIN();
   Serial.println("Vcc->" + String(Vcc) + "mV");
   Serial.println("Vin->" + String(Vin) + "mV");
  // 读取并设置当前温度
  SetTemp = DefaultTemp;
  RawTemp = denoiseAnalog(SENSOR_PIN);
  ChipTemp = getChipTemp();
  calculateTemp();
  if ((CurrentTemp + 20) < DefaultTemp) analogWrite(CONTROL_PIN, HEATER_ON);
   // set PID output range and start the PID
  ctrl.SetOutputLimits(0, 255);
  ctrl.SetMode(AUTOMATIC);
  // reset sleep timer
  sleepmillis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  ROTARYCheck();
  SLEEPCheck();
  SENSORCheck();
  Thermostat();
  sevseg.refreshDisplay();
}

void ROTARYCheck() {
  // 设置温度，切换增压模式
  int SetTemp = map(analogRead(VIN_PIN), 0, 1023, TEMP_MIN, TEMP_MAX);
  // 检测vin_pin 电压是否改变，如果改变，进行延时
  if (LastVin != SetTemp && LastVin + 1  != SetTemp && LastVin - 1 != SetTemp) {
    LastVin = SetTemp;
    // 延时显示
    buttonmillis = millis();
    while (millis() - buttonmillis < 2000) {
      sevseg.SetNumber(SetTemp, 1);
      sevseg.refreshDisplay();
    }
  }

}

uint16_t denoiseAnalog(byte port) {
  uint16_t result = 0;
  ADCSRA |= bit(ADEN) | bit(ADIF); // 启用ADC, 关闭任何挂起的中断
  if (port >=A0) port -= A0; // 设置端口
  ADMUX = (0x0F & port) | bit(REFS0); // 参考avcc
  set_sleep_mode(SLEEP_MODE_ADC);  // 采样时睡眠以降低噪声
  for (uint8_t i=0; i<32; i++) { // 采样32次
    sleep_mode();  // 在采集adc样本时入睡
    while (bitRead(ADCSRA, ADSC)); // 确保采样完成
    result += ADC;  // 把它们加起来
  }
  bitClear(ADCSRA, ADEN); // 禁用ADC
  return (result >> 5);  // 除以32并返回值
}

double getChipTemp() {
  // 通过读取ADC通道8的1.1v参考值来获取内部温度
  uint16_t result = 0;
  ADCSRA |= bit(ADEN) | bit(ADIF); // 启用ADC, 关闭任何挂起的中断
  ADMUX = bit(REFS1) | bit(REFS0) | bit(MUX3); // BIT设置参考和多路复用器
  delay(20); // 等待电压稳定
  set_sleep_mode(SLEEP_MODE_ADC); // 采样时睡眠以降低噪声
  for(uint8_t i=0; i<32; i++) { //获取32个读数
    sleep_mode(); //在采集adc样本时入睡
    while (bitRead(ADCSRA, ADSC)); //确保采样完成
    result += ADC;  //把它们加起来
  }
  bitClear(ADCSRA, ADEN); //禁用ADC
  result >= 2; // 除以4
  return ((result - 2594) / 9.76); // 计算内部温度，单位为摄氏度
}

uint16_t getVCC() {
  // 通过读取ADC通道8的1.1v参考值以 mv为单位来获取输入电压
  uint16_t result = 0;
  ADCSRA |= bit(ADEN) | bit(ADIF); // 启用ADC, 关闭任何挂起的中断
  ADMUX = bit(REFS0) | bit(MUX3) | bit(MUX2) | bit(MUX1);
  delay(1); // 等待电压稳定
  set_sleep_mode(SLEEP_MODE_ADC); // 采样时睡眠以降低噪声
  for(uint8_t i=0; i<16; i++) { // 获取16个读数
    sleep_mode(); // 在采集adc样本时入睡
    while (bitRead(ADCSRA, ADSC));  // 确保采样完成
    result += ADC; // 把它们加起来
  }
  bitClear(ADCSRA, ADEN); // 禁用ADC
  result >>= 4; // 除以16
  return (1125300L / result); // 1125300 = 1.1 * 1023 * 1000
}

uint16_t getVIN() {
  // 获取电源电压
  long result;
  result = denoiseAnalog(VIN_PIN); // 通过分压器读取电源电压
  return (result * Vcc / 179.474)  // 179.474 = 1023 * R13 / (R12 + R13)
}

void SENSORCheck() {
  // 检测传感器，设置温度
  analogWrite(CONTROL_PIN, HEATER_OFF);
  delayMicroseconds(TIME2SETTLE);
  double temp = denoiseAnalog(SENSOR_PIN);
  uint8_t d = digitalRead(SWITCH_PIN);
  if (d != d0) {handleMoved = true; d0 = d;}
  if (! SensorCounter--) Vin = getVIN();
  analogWrite(CONTROL_PIN, HEATER_PWM);
  RawTemp += (temp - RawTemp) * SMOOTHING;
  calculateTemp();
  if ((ShowTemp != Setpoint) || (abs(ShowTemp - CurrentTemp) > 5)) ShowTemp = CurrentTemp;
  if (abs(ShowTemp - Setpoint) <= 1) ShowTemp = Setpoint;
  gap = abs(SetTemp - CurrentTemp);
  if (gap < 5) {
    if (!isWoky && beepIfWorky) beep();
    isWorky = true;
    beepIfWorky = false;
  } 
  else isWork = false;
  if (ShowTemp > 500) TipIsPresent = false;
  if (!TipIsPresent && (ShowTemp < 500)) {
    analogWrite(CONTROL_PIN, HEATER_OFF);
    beep();
    TipIsPresent = true;
    handleMoved = true;
    RawTemp = denoiseAnalog(SENSOR_PIN);
    c0 = LOW;
    setRotary(0);
  }
}

void calculateTemp() {
  if (RawTemp < 200) CurrentTemp = map(RawTemp, 0, 200, 21, CalTemp[CurrentTip][0]);
  else if (RawTemp < 280) CurrentTemp = map(RawTemp, 200, 280, CalTemp[CurrentTip][0], CalTemp[CurrentTip][1]);
  else CurrentTemp = map(RawTemp, 280, 360, CalTemp[CurrentTip][1], CalTemp[CurrentTip][2]);
}

void Thermostat() {
  if (inOffMode) Setpoint = 0;
  else if (inSleepMode) Setpoint = SleepTemp;
  else if (inBoostMode) Setpoint = SetTemp + BoostTemp;
  else Setpoint = SetTemp;
  gap = abs(Setpoint - CurrentTemp);
  if (PIDenable) {
    Input = CurrentTemp;
    if (gap < 30) ctrl.SetTunings(consKp, consKi, consKd);
    else ctrl.SetTunings(aggKp, aggKi, aggKd);
    ctrl.Compute();
  } else {
    if ((CurrentTemp + 0.5) < Setpoint) Output = 0; else Output = 255;
  }
  analogWrite(CONTROL_PIN, HEATER_PWM);
}

void SLEEPCheck() {
  if (handleMoved) {
    if (inSleepMode) {
      if ((CurrentTemp + 20) < SetTemp)
        analogWrite(CONTROL_PIN, HEATER_ON);
      beepIfWorky = true;
    }
    handleMoved = false;
    inSleepMode = false;
    inOffMode = false;
    sleepmillis = millis();
  }
  goneMinutes = (millis() - sleepmillis) / 60000;
  if ( (!inSleepMode) && (time2sleep > 0) && (goneMinutes >= time2sleep) ) {inSleepMode = true; beep();}
  if ( (!inOffMode)   && (time2off   > 0) && (goneMinutes >= time2off  ) ) {inOffMode   = true; beep();}
}

void SevSegInit() {
  // 数码管初始化
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void ISR() {
  // 如果触发了中断，说明手柄在移动
  handleMoved = true;
}