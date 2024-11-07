#include "adjustable_soldering_iron.h"
#include "math.h"


void SolderingIron::init() {
    pinMode(TEMP_SENSOR_PIN, INPUT);
    pinMode(TEMP_SET_PIN, INPUT);
    pinMode(OUT_CTRL, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(LEFT_PUSHBUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_PUSHBUTTON_PIN, INPUT_PULLUP);
    pinMode(BALL_SW_PIN, INPUT_PULLUP);
    digitalWrite(OUT_CTRL, HIGH);
    byte digitPins[] = { 11, 12, 13 };
    byte segmentPins[] = { 2, 4, 6, 7, 8, 9, 10 };
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
    sevseg.setBrightness(90);
}


int SolderingIron::denoiseADC(int port) {
    static float  T12outadc[5] = {0}; // 差值绝对值
    int timesofdenoise = 5;
    int i = 0;
    int sum = 0;
    digitalWrite(OUT_CTRL, LOW);
    while (i < timesofdenoise) {
        int outadc = analogRead(port);
        if (outadc < 420 && outadc > 0) {
            T12outadc[i] = outadc;
            i++;
        }
    }
    int size = sizeof(T12outadc) / sizeof(T12outadc[0]);
    for (i = 0; i < size ; i++) {
        sum += T12outadc[i];
    }
    digitalWrite(OUT_CTRL, HIGH);
    return sum / size;
}


float SolderingIron::voltage(int adc) {
    float ans = adc * 5 / 1023.0;
    return ans;
}

void SolderingIron::setDisplaytargetTime(const unsigned long timer) {
    last_displaytarget_time = timer;
}