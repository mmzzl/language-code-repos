#include "weather.h"

GeoIP geoip; 
location_t loc;
WiFiClientSecure httpsClient;
String cityName;
static  const String key = "f715d659038b9afa0ba3a9a1d3d0eaa4";

void getGeo() { 
    //获取地理位置信息
    String url = "https://restapi.amap.com/v3/ip?key=" + key + "&ip=120.237.102.250";
    // String url = "https://whois.pconline.com.cn/ipJson.jsp?json=true";
    String body;
    Serial.println(url);
    getRequestHttp(url, &body);
    JsonDocument jsonDoc; 
    DeserializationError error = deserializeJson(jsonDoc, body);
    if (!error) {
        JsonObject rootObj = jsonDoc.as<JsonObject>();
        cityName = rootObj["city"].as<String>();
    } else {
            Serial.println("JSON parse failed");
            Serial.println(error.f_str());
    }
}

void getRequestHttp(String url, String *body) {
    httpsClient.setInsecure();
    if (httpsClient.connect("restapi.amap.com", 443)) {
        Serial.println("Connected to server");
        httpsClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                         "Host: " + "restapi.amap.com" + "\r\n" +
                         "Connection: close\r\n\r\n");
        while (!httpsClient.available()) {
                delay(100);
        }
        String response = httpsClient.readString();
        httpsClient.stop();
        int bodyStart = response.indexOf("\r\n\r\n");
        if (bodyStart != -1) {
            // 从空行之后的位置开始，提取响应正文
            *body = response.substring(bodyStart + 4);
            // Serial.println(*body); // 现在 body 只包含响应正文
        } else {
            Serial.println("No body found");
        }
        Serial.println(ESP.getFreeHeap());
    } else {
        Serial.println("Failed to connect to server");
    }
}
void getWeather(weather_t *forecasts) {
   
    String region;
    // 获取地理位置信息
        // delay(5000);
    getGeo();
    Serial.println(cityName);
    cityName = "深圳市";
    String url = "https://restapi.amap.com/v3/weather/weatherInfo?key=" + key + "&city=" + cityName + "&extensions=all&output=json";
    String body;
    getRequestHttp(url, &body);
    JsonDocument jsonDoc; 
    DeserializationError error = deserializeJson(jsonDoc, body);
    if (!error) {
            JsonObject rootObj = jsonDoc.as<JsonObject>();
            // 检查forecasts数组是否存在且长度足够
            if (rootObj.containsKey("forecasts") && rootObj["forecasts"].is<JsonArray>() && rootObj["forecasts"].size() > 0) {
                JsonArray forecastsArray = rootObj["forecasts"].as<JsonArray>();
                if (forecastsArray[0].containsKey("city") && forecastsArray[0]["city"].is<String>()) {
                    // 使用String的赋值操作来复制城市名称，而不是错误的strcpy（strcpy用于char数组）
                    for (int i = 0; i < 3; i++) {
                        forecasts[i].city = forecastsArray[0]["city"].as<String>();
                        JsonArray casts = forecastsArray[0]["casts"];
                        for (auto cast : casts) {
                            forecasts[i].date = cast["date"].as<String>();
                            forecasts[i].dayweather = cast["dayweather"].as<String>();
                            forecasts[i].nightweather = cast["nightweather"].as<String>();
                        }
                    }
                }
            }
    } 
    else {
        Serial.println("JSON parse failed");
        Serial.println(error.f_str());
    }
}
