#include "weather.h"

// GeoIP geoip; 
// location_t loc;
WiFiClientSecure httpsClient;
char cityName[32] = "深圳市";
const char* key= "f715d659038b9afa0ba3a9a1d3d0eaa4";

const int URL_MAX_LEN = 256;
const int RESPONSE_MAX_LEN = 1024;
const int BODY_MAX_LEN = 1024;

// const std::map<std::string, const uint16_t*> weatherIcons = {
//     {"晴", qing},
//     {"雷阵雨", leizhenyu},
//     {"阴", yin},
//     {"阵雨", zhenyu},
//     {"晴间多云", qingjianduoyun},
//     {"大暴雨", dabaoyu},
//     {"暴雨", baoyu},
//     {"大雨", dayu},
//     {"中雨", zhongyu},
//     {"小雨", xiaoyu},
//     {"多云", duoyun}
// };

// 显示天气图标的函数
// void showWeatherIcon(const char* weather, lv_obj_t *obj) {
//     std::string weatherStr(weather);
//     auto it = weatherIcons.find(weatherStr);
//     if (it != weatherIcons.end()) {
//         // 创建图像对象
//         lv_obj_t * img = lv_img_create(obj);

//         // 设置图像源为对应的图标数据
//         lv_img_set_src(img, it->second);

//         // 居中显示图像
//         lv_obj_center(img);
//     } else {
//         lv_obj_t * img = lv_img_create(obj);
//         // 设置图像源为对应的图标数据
//         lv_img_set_src(img, &weizhitianqi);

//         // 居中显示图像
//         lv_obj_center(img);
//     }
// }


void getRequestHttp(char * url, String *body) {
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
    char url[256];
    // 构建 URL
    snprintf(url, sizeof(url), "https://restapi.amap.com/v3/weather/weatherInfo?key=%s&city=%s&extensions=all&output=json", key, cityName);
    Serial.print("构建的 URL 为: ");
    Serial.println(url);

    String body;
    getRequestHttp(url, &body);
    // Serial.print("HTTP 请求响应内容: ");
    // Serial.println(body);

    StaticJsonDocument<2048> jsonDoc; 
    DeserializationError error = deserializeJson(jsonDoc, body);
    if (!error) {
        Serial.println("JSON 解析成功");
        JsonObject rootObj = jsonDoc.as<JsonObject>();
        // 检查 forecasts 数组是否存在且长度足够
        if (rootObj.containsKey("forecasts") && rootObj["forecasts"].is<JsonArray>() && rootObj["forecasts"].size() > 0) {
            Serial.println("JSON 数据中包含 forecasts 数组");
            JsonArray forecastsArray = rootObj["forecasts"].as<JsonArray>();
            if (forecastsArray[0].containsKey("city") && forecastsArray[0]["city"].is<const char*>()) {
                Serial.println("forecasts 数组的第一个元素包含城市信息");
                const char* city = forecastsArray[0]["city"].as<const char*>();
                JsonArray casts = forecastsArray[0]["casts"];
                int castIndex = 0;
                for (int i = 0; i < 3 && castIndex < casts.size(); i++) {
                    strncpy(forecasts[i].city, city, sizeof(forecasts[i].city) - 1);
                    forecasts[i].city[sizeof(forecasts[i].city) - 1] = '\0';

                    const char* date = casts[castIndex]["date"].as<const char*>();
                    strncpy(forecasts[i].date, date, sizeof(forecasts[i].date) - 1);
                    forecasts[i].date[sizeof(forecasts[i].date) - 1] = '\0';

                    const char* dayweather = casts[castIndex]["dayweather"].as<const char*>();
                    strncpy(forecasts[i].dayweather, dayweather, sizeof(forecasts[i].dayweather) - 1);
                    forecasts[i].dayweather[sizeof(forecasts[i].dayweather) - 1] = '\0';

                    const char* nightweather = casts[castIndex]["nightweather"].as<const char*>();
                    strncpy(forecasts[i].nightweather, nightweather, sizeof(forecasts[i].nightweather) - 1);
                    forecasts[i].nightweather[sizeof(forecasts[i].nightweather) - 1] = '\0';

                    Serial.printf("第 %d 天的天气信息：城市: %s, 日期: %s, 白天天气: %s, 夜间天气: %s\n", 
                                  i + 1, forecasts[i].city, forecasts[i].date, forecasts[i].dayweather, forecasts[i].nightweather);

                    castIndex++;
                }
            } else {
                Serial.println("forecasts 数组的第一个元素不包含城市信息");
            }
        } else {
            Serial.println("JSON 数据中不包含 forecasts 数组");
        }
    } 
    else {
        Serial.println("JSON parse failed");
        Serial.println(error.f_str());
    }
}
