#include "network.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
void handleRoot() {
  String html = "<html><body>";
  html += "<form action=\"/connect\" method=\"POST\">";
  html += "SSID: <input type=\"text\" name=\"ssid\"><br>";
  html += "Password: <input type=\"password\" name=\"password\"><br>";
  html += "<input type=\"submit\" value=\"Connect\"></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


void handlerConnect() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    const char *ssid = server.arg("ssid").c_str();
    const char *password = server.arg("password").c_str();
    saveDataToEEPROM(ssid, password);
    connectToWiFi(ssid, password);
    server.sendHeader("Location", "/");
    server.send(303); // HTTP 303 See Other
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void saveDataToEEPROM(const char* ssid, const char* password) {
  // 将字符串转换为字符数组，并写入 EEPROM
  for (uint16_t i = 0; i < MAX_LENGTH; i++) {
    if (i < strlen(ssid)) EEPROM.write(EEPROM_SSID_START_ADDR + i, ssid[i]);
    else EEPROM.write(EEPROM_SSID_START_ADDR + i, '\0'); // 添加终止符
    if (i < strlen(password)) EEPROM.write(EEPROM_PASSWORD_START_ADDR + i, password[i]);
    else EEPROM.write(EEPROM_PASSWORD_START_ADDR + i, '\0'); // 添加终止符
  }
  EEPROM.commit();
  Serial.println("Saved SSID and password to EEPROM.");
}

bool readDataFromEEPROM(String& ssid, String& password) {
  char buffer[MAX_LENGTH];

  // 从 EEPROM 读取 SSID
  for (uint16_t i = 0; i < MAX_LENGTH; i++) {
    buffer[i] = EEPROM.read(EEPROM_SSID_START_ADDR + i);
    if (buffer[i] == '\0') break;
  }
  ssid = String(buffer);

  // 从 EEPROM 读取 Password
  for (int i = 0; i < MAX_LENGTH; i++) {
    buffer[i] = EEPROM.read(EEPROM_PASSWORD_START_ADDR + i);
    if (buffer[i] == '\0') break;
  }
  password = String(buffer);

  return !ssid.isEmpty() && !password.isEmpty();
}

void disconnectWiFi() {
  WiFi.disconnect();
  Serial.println("Wi-Fi disconnected.");
}

void disconnectSoftAP() {
  if (WiFi.softAPdisconnect(true)) {
    Serial.println("SoftAP disconnected successfully.");
  } else {
    Serial.println("Failed to disconnect SoftAP.");
  }
}

void network_init() {
    // 尝试读取 EEPROM 中保存的 SSID 和密码
  String ssid = "";
  String password = "";

  if (readDataFromEEPROM(ssid, password)) {
    Serial.println("Using saved SSID and password from EEPROM.");
    connectToWiFi(ssid.c_str(), password.c_str());
  } else {
    WiFi.softAP("ESP8266-Config");
    server.on("/", handleRoot);
    server.on("/connect", HTTP_POST, handlerConnect);
    server.begin();
  }
}

void start_server() {
  if (WiFi.getMode() == WIFI_AP || WiFi.softAPgetStationNum() > 0) {
        server.handleClient();  // 处理客户端请求，确保在 AP 模式下服务器正常工作
  }
}

void connectToWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int attempts = 0;

  // 等待 Wi-Fi 连接
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    // digitalWrite(LED_BUILTIN, HIGH); // 显示正在尝试连接
    delay(500);
    // digitalWrite(LED_BUILTIN, LOW);  // 关闭指示灯
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi");
    // digitalWrite(LED_BUILTIN, HIGH); // 显示已连接
    disconnectSoftAP();
  } else {
    Serial.println("Failed to connect to Wi-Fi.");
    WiFi.softAP("ESP8266-Config"); // 如果连接失败，则恢复 AP 模式
    server.on("/", handleRoot);
    server.on("/connect", HTTP_POST, handlerConnect);
    server.begin();
  }
}