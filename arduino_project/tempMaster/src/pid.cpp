#include "pid.h"

double SimplePID::compute(double measurement, double setpoint) {
    double error = setpoint - measurement;
    //积分项
    _integral += error;
    //微分项
    _derivative = error - _lastError;
    //输出
    double output = _Kp * error + _Ki * _integral + _Kd * _derivative;
    // 限幅
    if (output > _maxOutput) {
        output = _maxOutput;
        _integral = _integral > 0 ? _integral - error : _integral;
    } else if (output < _minOutput) {
        output = _minOutput;
        _integral = _integral < 0 ? _integral + error : _integral;
    }
    // 更新状态
    _lastError = error;
    _lastMeasurement = measurement;
    return output;
}

void  SimplePID::resetIntegral() {
    _integral = 0;
}