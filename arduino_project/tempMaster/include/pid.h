class SimplePID {
    public:
        SimplePID(double Kp, double Ki, double Kd, double minOutput, double maxOutput):
            _Kp(Kp), _Ki(Ki), _Kd(Kd), _lastError(0), _integral(0), _derivative(0),
             _lastMeasurement(0), _minOutput(minOutput), _maxOutput(maxOutput) {}
        
        double compute(double measurement, double setpoint);
        //重置积分项
        void resetIntegral();
    private:
        double _Kp, _Ki, _Kd;
        double _lastError;
        double _integral;
        double _derivative;
        double _lastMeasurement;
        double _minOutput, _maxOutput;
};  