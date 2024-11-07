#include "adjustable_soldering_iron.h"
#include "pid.h"


SolderingIron solerIron;
double Kp = 2.0; // 比例系数
double Ki = 0.1; // 积分系数
double Kd = 0.01; // 微分系数
double minOutput = 0; // 最小输出值
double maxOutput = 255; // 最大输出值
float  max_temp = 425; 
SimplePID pid(Kp, Ki, Kd, minOutput, maxOutput);
double input = 0;
double output = 0;
int temp_target = 0; // 目标温度
int last_temp_target = 0; // 用于记录
int count = 0;
int temp_display = 0;
int temp_display_count = 0;
static unsigned long last_display_time;
int i = 0;
int sum = 0;
int ans[100];
int tmp[100];

void setup() {
    Serial.begin(115200);
    solerIron.init();
}

void loop ()
{
    // 读取目标温度
    
    int temp_target = map(analogRead(TEMP_SET_PIN), 0, 1023, 125, 350);
    if ((last_temp_target != temp_target) && (last_temp_target != temp_target + 1) && (last_temp_target != temp_target - 1)) {
        last_temp_target = temp_target;
        // 延时显示
        unsigned long start_time = millis();
        while (millis() - start_time < 2000) {
            solerIron.sevseg.setNumber(temp_target, 1);
            solerIron.sevseg.refreshDisplay();
        }
    }
    // int tempreture_bias = map(analogRead(SETBIAS_PIN), 0, 1024, -50, 50); // 微调
    int temp_sensor = solerIron.denoiseADC(TEMP_SENSOR_PIN);
    float setpoint = solerIron.voltage(temp_target);
    float current_voltage = solerIron.voltage(temp_sensor);
    output = pid.compute(current_voltage,  setpoint);
    analogWrite(OUT_CTRL, output);
    Serial.println(output);
    // 显示
    sum += temp_sensor;
    i += 1;
    if (i >= 20) {
        solerIron.sevseg.blank();
        solerIron.sevseg.setNumber(sum / 20, 1);
        sum = 0;
        i = 0;
    }
    solerIron.sevseg.refreshDisplay();
}