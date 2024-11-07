#include "SevSeg.h"
#include "constants.h"


class SolderingIron {
    private:
        int temp_target = 220;
        bool out_status = 1;
        bool auto_turnoff_status = 0;
        int lastPrintTime = 0;
        int tempreture = 0;
        int last_temp_target = 0;
        unsigned long last_displaytarget_time = 0;
        unsigned long last_handlemove_time = 0;
        int lastTime = -151;
        int temp_display = 0;
        int temp_display_count = 0;
        int tempreture_bias = 0;
        byte numDigits = 3;
        bool resistorsOnSegments = false;      // 'false' means resistors are on digit pins
        byte hardwareConfig = COMMON_CATHODE;  // See README.md for options
        bool updateWithDelays = false;         // Default 'false' is Recommended
        bool leadingZeros = false;             // Use 'true' if you'd like to keep the leading zeros
        bool disableDecPoint = true;           // Use 'true' if your decimal point doesn't exist or isn't connected

    public:
        SevSeg sevseg;
        // 成员函数：初始化引脚
        void init();
        int denoiseADC(int port);
        float voltage(int dac); // 目标电压
        void setDisplaytargetTime(const unsigned long timer); 
        
};
