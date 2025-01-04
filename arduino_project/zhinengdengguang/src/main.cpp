#include <Arduino.h>
#include <network.h>
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

// put function declarations here:
Network network;
const int ledPin = 13;
char auth[] = "0eb6a5209fd2";
char ssid[] = "23@A210";
char pswd[] = "WHhv6e6d";
bool wsState;
uint8_t colorW;


void miotBright(const String & bright)
{
  BLINKER_LOG("need set brightness: ", bright);
  colorW = bright.toInt();
  BLINKER_LOG("now set brightness: ", colorW);
  analogWrite(ledPin, colorW);
  BlinkerMIOT.brightness(colorW);
  BlinkerMIOT.print();
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}


void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.color(0);
            BlinkerMIOT.mode(0);
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.brightness(1);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("MIOT Query Color");
            BlinkerMIOT.color(0);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("MIOT Query Mode");
            BlinkerMIOT.mode(0);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
            BLINKER_LOG("MIOT Query ColorTemperature");
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("MIOT Query Brightness");
            BlinkerMIOT.brightness(1);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.color(0);
            BlinkerMIOT.mode(0);
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.brightness(1);
            BlinkerMIOT.print();
            break;
    }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // BLINKER_DEBUG.stream(Serial);
  // BLINKER_DEBUG.debugAll();

  pinMode(A0, INPUT);
  pinMode(ledPin, OUTPUT);

  // 初始化LEDC
  analogWriteFreq(23000);
  analogWriteRange(255);
  // Blinker.begin(auth, ssid, pswd);
  // Blinker.attachData(dataRead);

  // BlinkerMIOT.attachBrightness(miotBright);
  // network.init();
  // BlinkerMIOT.attachQuery(miotQuery);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Blinker.run();
  int output = map(analogRead(A0), 0, 1023, 0, 255);
  if (output < 5) {
    output = 0;
  }
  analogWrite(ledPin, output);
}

// put function definitions here:
