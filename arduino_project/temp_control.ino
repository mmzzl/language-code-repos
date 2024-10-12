#define max_temp 425  // 设置温度保护阈值
#define display_period 2000 // 用于控制显示时间
#define autooff_time 5 * 60 * 1000 // 用于自动关闭的实际

#include "SevSeg.h" // 用于显示数码管
SevSeg sevseg;  //数码管对象

int red_led_pin = 5;
int temp_sensor_pin = A0; // 当前烙铁头温度
int temp_set_pin = A1; // 设置温度引脚
int setbias_pin =  A6; //  微调

// pid 参数
double kp = 2.0; // 比例系数
double ki = 0.5; // 积分系数
double kd = 1.0; // 微分系数


void void setup()
{
    //初始化引脚
    Serial.begin(38400); // 打开串口
    pinMode(temp_sensor_pin, INPUT); 
    pinMode(temp_set_pin, INPUT);
    pinMode(red_led_pin, OUTPUT);
}

void loop() {
    static unsigned long timer = millis(); //  获取微秒
    // 使用pid控制温度
    // 1. 计算误差： error = temp_target - temp_sensor 
    // 2. 比例部分： p_out = kp * error 
    // 3. 积分部分： i_out = ki * integral  其中integral 是误差的累积
    // 4. 微分部分： d_out = kd * derivative 其中derivative 是误差的变化
    // 5. 调整输出： output = p_out + i_out + d_out
    // 6. 根据pid输出调整系统的控制变量
    // 读取当前温度
    int raw_temp = 

}

int getTempreture(int rawTemp) {
    // 温度加25
    return rawTemp + 25;
}