#include "weather.h"

WiFiClientSecure httpsClient;
const char* key = "f715d659038b9afa0ba3a9a1d3d0eaa4";
char cityName[32] = "深圳市";

bool getRequestHttp(const char* url, String* body) {
    httpsClient.setInsecure();
    for (int retry = 0; retry < 3; retry++) {
        if (httpsClient.connect("restapi.amap.com", 443)) {
            httpsClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                             "Host: restapi.amap.com\r\n" +
                             "Connection: close\r\n\r\n");
            
            unsigned long start = millis();
            while (!httpsClient.available() && millis() - start < 5000) {
                delay(100);
            }
            
            if (httpsClient.available()) {
                *body = httpsClient.readString();
                httpsClient.stop();
                
                int bodyStart = body->indexOf("\r\n\r\n");
                if (bodyStart != -1) {
                    *body = body->substring(bodyStart + 4);
                    return true;
                }
            }
        }
        httpsClient.stop();
        delay(1000);
    }
    return false;
}

void getWeather(weather_t* forecasts) {
    char url[128];
    snprintf(url, sizeof(url), 
             "https://restapi.amap.com/v3/weather/weatherInfo?key=%s&city=%s&extensions=all&output=json", 
             key, cityName);
    
    String body;
    if (!getRequestHttp(url, &body)) {
        Serial.println("HTTP request failed");
        return;
    }

    StaticJsonDocument<2048> jsonDoc;  // 增加缓冲区大小
    DeserializationError error = deserializeJson(jsonDoc, body);
    
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    // 检查forecasts数组是否存在
    if (!jsonDoc["forecasts"].is<JsonArray>() || jsonDoc["forecasts"].size() == 0) {
        Serial.println("Invalid forecasts data");
        return;
    }

    // 获取第一个城市预报
    JsonObject cityForecast = jsonDoc["forecasts"][0];
    const char* city = cityForecast["city"] | "N/A";
    
    // 获取casts数组
    JsonArray castsArray = cityForecast["casts"];
    for (int i = 0; i < 3 && i < castsArray.size(); i++) {
        JsonObject cast = castsArray[i];
        
        strlcpy(forecasts[i].city, city, sizeof(forecasts[i].city));
        strlcpy(forecasts[i].date, cast["date"] | "N/A", sizeof(forecasts[i].date));
        strlcpy(forecasts[i].dayweather, cast["dayweather"] | "N/A", sizeof(forecasts[i].dayweather));
        strlcpy(forecasts[i].nightweather, cast["nightweather"] | "N/A", sizeof(forecasts[i].nightweather));
    }
}
