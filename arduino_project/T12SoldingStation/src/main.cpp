// Libraries
#include <PID_v1.h>             // PID library
#include <avr/sleep.h>          // for sleeping during ADC sampling
#include <SevSeg.h>             // SevSeg library for 7-segment display

// Firmware version
#define VERSION       "v1.9"

// Type of MOSFET
#define P_MOSFET                // P_MOSFET or N_MOSFET

// Pins
#define SENSOR_PIN    A0        // tip temperature sense
#define VIN_PIN       A1        // input voltage sense
#define BUZZER_PIN     5        // buzzer
#define CONTROL_PIN    9        // heater MOSFET PWM control
#define SWITCH_PIN    10        // handle vibration switch
#define POTENTIOMETER_PIN A2    // potentiometer for temperature setting

// Default temperature control values (recommended soldering temperature: 300-380°C)
#define TEMP_MIN      150       // min selectable temperature
#define TEMP_MAX      400       // max selectable temperature
#define TEMP_DEFAULT  320       // default start setpoint
#define TEMP_SLEEP    150       // temperature in sleep mode
#define TEMP_BOOST     50       // temperature increase in boost mode

// Control values
#define TIME2SETTLE   950       // time in microseconds to allow OpAmp output to settle
#define SMOOTHIE      0.05      // OpAmp output smooth factor (1=no smoothing; 0.05 default)
#define PID_ENABLE    true     // enable PID control
#define BEEP_ENABLE   true      // enable/disable buzzer
#define SLEEP_TIME    300000    // 5 minutes in milliseconds
#define TIPMAX        8         // max number of tips
#define TEMPCHP 30 // 芯片温度
#define TIPNAMELENGTH 6 // 提示名称长度
#define TIPNAME "BC1.5"  // 提示名称

#define TEMP200 216 // 200度
#define TEMP280 308 // 280度
#define TEMP360 390 // 300度

// MOSFET control definitions
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

// Define the aggressive and conservative PID tuning parameters
double aggKp=11, aggKi=0.5, aggKd=1;
double consKp=11, consKi=3, consKd=5;

// Default values
uint16_t  DefaultTemp = TEMP_DEFAULT;
uint16_t  SetTemp, ShowTemp;
double    Input, Output, Setpoint, RawTemp, CurrentTemp;

// Default values for tips
uint16_t  CalTemp[TIPMAX][4] = {TEMP200, TEMP280, TEMP360, TEMPCHP};
char      TipName[TIPMAX][TIPNAMELENGTH] = {TIPNAME};
uint8_t   CurrentTip   = 0;
uint8_t   NumberOfTips = 1;

// State variables
bool      inSleepMode = false;
bool      inOffMode   = false;
bool      isWorky     = true;
bool      TipIsPresent = true; // Initialize to true

// Timing variables
uint32_t  sleepmillis;
uint32_t  lastActivityTime; // 记录最后活动时间
uint32_t  buttonmillis;

// Specify variable pointers and initial PID tuning parameters
PID ctrl(&Input, &Output, &Setpoint, aggKp, aggKi, aggKd, REVERSE);

// SevSeg variables
SevSeg sevseg; // Create a SevSeg object
byte numDigits = 3;
bool resistorsOnSegments = false;
byte hardwareConfig = COMMON_CATHODE;
bool updateWithDelays = false;
bool leadingZeros = true;
bool disableDecPoint = true;
byte digitPins[] = {11, 12, 13}; // Digit pins
byte segmentPins[] = {2, 4, 6, 7, 8, 9, 10}; // Segment pins
uint16_t gap;
bool PIDenable = PID_ENABLE;

void SevSegInit() {
  // 数码管初始化
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void enterSleepMode();
void SENSORCheck();
void calculateTemp();
void Thermostat();
void beep();
uint16_t denoiseAnalog(byte port);
void handleSwitchChange();

void setup() {
  // set the pin modes
  pinMode(SENSOR_PIN,   INPUT);
  pinMode(VIN_PIN,      INPUT);
  pinMode(BUZZER_PIN,   OUTPUT);
  pinMode(CONTROL_PIN,  OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(SWITCH_PIN,   INPUT_PULLUP);

  analogWrite(CONTROL_PIN, HEATER_OFF); // this shuts off the heater
  digitalWrite(BUZZER_PIN, LOW);        // must be LOW when buzzer not in use

  // setup ADC
  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);  // set ADC prescaler to 128
  ADCSRA |= bit (ADIE);                 // enable ADC interrupt
  interrupts ();                        // enable global interrupts

  // Initialize SevSeg
  SevSegInit();

  // read and set current iron temperature
  SetTemp  = DefaultTemp;
  RawTemp  = denoiseAnalog(SENSOR_PIN);
  calculateTemp();

  // turn on heater if iron temperature is well below setpoint
  if ((CurrentTemp + 20) < DefaultTemp) analogWrite(CONTROL_PIN, HEATER_ON);

  // set PID output range and start the PID
  ctrl.SetOutputLimits(0, 255);
  ctrl.SetMode(AUTOMATIC);

  // reset sleep timer
  sleepmillis = millis();
  lastActivityTime = millis(); // 初始化最后活动时间
   // Attach interrupt to handle switch state change
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleSwitchChange, CHANGE);
}

void loop() {
  // 检查手柄状态
  uint8_t d = digitalRead(SWITCH_PIN);
  if (d == LOW) { // 如果手柄被移除
    TipIsPresent = false; // 设置为未插入状态
  } else {
    TipIsPresent = true; // 手柄插入
    lastActivityTime = millis(); // 更新最后活动时间
  }

  // 检查是否需要进入休眠模式
  if (!inSleepMode && (millis() - lastActivityTime >= SLEEP_TIME)) {
    enterSleepMode(); // 进入休眠模式
  }

  // 读取可调电阻的值并映射到温度
  int potValue = analogRead(POTENTIOMETER_PIN);
  SetTemp = map(potValue, 0, 1023, TEMP_MIN, TEMP_MAX);

  // 检查温度并控制加热器
  SENSORCheck();      // 读取温度和手柄状态
  Thermostat();       // 控制加热器

  // 更新显示
  if (!TipIsPresent) {
    sevseg.setNumber(999); // 显示错误代码，例如999表示错误
  } else {
    sevseg.setNumber(ShowTemp); // 显示当前温度
  }
  sevseg.refreshDisplay(); // 刷新显示
}

// 进入休眠模式
void enterSleepMode() {
  inSleepMode = true;
  analogWrite(CONTROL_PIN, HEATER_OFF); // 关闭加热器
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // 设置休眠模式
  sleep_mode(); // 进入休眠
  // 休眠结束后，恢复工作状态
  inSleepMode = false;
  lastActivityTime = millis(); // 重置最后活动时间
}

// 读取温度、手柄状态和电压
void SENSORCheck() {
  analogWrite(CONTROL_PIN, HEATER_OFF);       // shut off heater in order to measure temperature
  delayMicroseconds(TIME2SETTLE);             // wait for voltage to settle

  double temp = denoiseAnalog(SENSOR_PIN);    // read ADC value for temperature

  analogWrite(CONTROL_PIN, HEATER_ON);        // turn on heater again

  RawTemp += (temp - RawTemp) * SMOOTHIE;     // stabilize ADC temperature reading
  calculateTemp();                            // calculate real temperature value

  // stabilize displayed temperature when around setpoint
  if ((ShowTemp != Setpoint) || (abs(ShowTemp - CurrentTemp) > 5)) ShowTemp = CurrentTemp;
  if (abs(ShowTemp - Setpoint) <= 1) ShowTemp = Setpoint;

  // set state variable if temperature is in working range; beep if working temperature was just reached
  if (abs(SetTemp - CurrentTemp) < 5) {
    if (!isWorky) beep();
    isWorky = true;
  } else {
    isWorky = false;
  }
}

// 计算实际温度值
void calculateTemp() {
  if      (RawTemp < 200) CurrentTemp = map (RawTemp,   0, 200,                     21, CalTemp[CurrentTip][0]);
  else if (RawTemp < 280) CurrentTemp = map (RawTemp, 200, 280, CalTemp[CurrentTip][0], CalTemp[CurrentTip][1]);
  else                    CurrentTemp = map (RawTemp, 280, 360, CalTemp[CurrentTip][1], CalTemp[CurrentTip][2]);
}

// 控制加热器
void Thermostat() {
  // define Setpoint according to current working mode
  Setpoint = SetTemp;
  gap = abs(Setpoint - CurrentTemp);
  if (PIDenable) {
    Input = CurrentTemp;
    if (gap < 30) ctrl.SetTunings(consKp, consKi, consKd);
    else ctrl.SetTunings(aggKp, aggKi, aggKd);
    ctrl.Compute();
  } else {
    if ((CurrentTemp + 0.5) < Setpoint) Output = 0; else Output = 255;
  }
  
  analogWrite(CONTROL_PIN, HEATER_PWM); // set heater PWM
}

// 创建短蜂鸣声
void beep() {
  if (BEEP_ENABLE) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// 平均多个ADC读取以去噪
uint16_t denoiseAnalog (byte port) {
  uint16_t result = 0;
  ADCSRA |= bit (ADEN) | bit (ADIF);    // enable ADC, turn off any pending interrupt
  if (port >= A0) port -= A0;           // set port and
  ADMUX = (0x0F & port) | bit(REFS0);   // reference to AVcc
  set_sleep_mode (SLEEP_MODE_ADC);      // sleep during sample for noise reduction
  for (uint8_t i=0; i<32; i++) {        // get 32 readings
    sleep_mode();                       // go to sleep while taking ADC sample
    while (bitRead(ADCSRA, ADSC));      // make sure sampling is completed
    result += ADC;                      // add them up
  }
  bitClear (ADCSRA, ADEN);              // disable ADC
  return (result >> 5);                 // divide by 32 and return value
}

// 处理手柄状态变化的中断服务例程
void handleSwitchChange() {
  lastActivityTime = millis(); // 更新最后活动时间
  if (inSleepMode) {
    // 如果当前在休眠模式，退出休眠
    sleep_disable(); // 退出休眠
    inSleepMode = false; // 更新状态
  }
}