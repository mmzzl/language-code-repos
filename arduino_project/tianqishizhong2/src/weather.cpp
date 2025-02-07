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
      // 设置连接超时时间为 5 秒
    httpsClient.setTimeout(5000); 

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

// 获取实时天气信息
void realtimeWeather(weather_t *forecasts) {
    cityName = "武胜县";
    String url = "https://restapi.amap.com/v3/weather/weatherInfo?key=" + key + "&city=" + cityName + "&extensions=base&output=json";
    String body;
    getRequestHttp(url, &body);
    JsonDocument jsonDoc; 
    DeserializationError error = deserializeJson(jsonDoc, body);
    if (!error) {
        JsonObject rootObj = jsonDoc.as<JsonObject>();
        if (rootObj.containsKey("lives") && rootObj["lives"].is<JsonArray>() && rootObj["lives"].size() > 0) {
            JsonArray livesArray = rootObj["lives"].as<JsonArray>();
            if (livesArray[0].containsKey("city") && livesArray[0]["city"]) {
                // 使用String的赋值操作来复制城市名称，而不是错误的strcpy（strcpy用于char数组）
                JsonObject lives = livesArray[0];
                strcpy(forecasts[0].realtimeWeather,lives["weather"]);
            }
        }
    }
    // 释放 JsonDocument 占用的内存
    jsonDoc.clear(); 
}

void getWeather(weather_t *forecasts) {
   
    String region;
    // 获取地理位置信息
        // delay(5000);
    // getGeo();
    // Serial.println(cityName);
    cityName = "武胜县";
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
                    JsonArray casts = forecastsArray[0]["casts"];
                    // 确定要处理的元素数量，取 3 和 casts 数组长度的较小值
                    int numForecasts = 3;
                    for (int i = 0; i < numForecasts; i++) {
                        // 赋值城市信息
                        strcpy(forecasts[i].city, forecastsArray[0]["city"]);
                        // 获取 casts 数组中对应索引的元素
                        JsonObject cast = casts[i];
                        // 赋值日期信息
                        strcpy(forecasts[i].date, cast["date"]);
                        // 赋值白天天气信息
                        strcpy(forecasts[i].dayweather, cast["dayweather"]);
                        // 赋值夜间天气信息
                        strcpy(forecasts[i].nightweather, cast["nightweather"]);
                    }
                }
            }
    } 
    else {
        Serial.println("JSON parse failed");
        Serial.println(error.f_str());
    }
    // 释放 JsonDocument 占用的内存
    jsonDoc.clear(); 
}
