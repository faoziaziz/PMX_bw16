#include <WiFi.h>      // Gunakan WiFi.h untuk ESP32 atau ESP8266WiFi.h untuk ESP8266
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Konfigurasi WiFi
const char* ssid = "WirelessNet";            // Ganti dengan SSID WiFi Anda
const char* password = "eeeeeeee";    // Ganti dengan password WiFi Anda

// URL Zabbix API
const String zabbixApiUrl = "http://192.168.100.79/zabbix/api_jsonrpc.php";  // Ganti dengan URL Zabbix Anda

// Login data
const String username = "Admin";            // Ganti dengan username Zabbix Anda
const String zabbixPassword = "zabbix";    // Ganti dengan password Zabbix Anda

// Fungsi untuk mendapatkan auth token
String getAuthToken() {
  HTTPClient http;
  String response = "";

  // Membuat JSON request untuk Zabbix API login
  String requestData = "{\"jsonrpc\": \"2.0\", "
                       "\"method\": \"user.login\", "
                       "\"params\": {\"user\": \"" + username + "\", "
                       "\"password\": \"" + zabbixPassword + "\"}, "
                       "\"id\": 1}";

  // Kirim request HTTP POST untuk login
  http.begin(zabbixApiUrl);
  http.addHeader("Content-Type", "application/json");

  // Mengirimkan JSON request ke Zabbix
  int httpResponseCode = http.POST(requestData);

  // Mengecek response dari Zabbix
  if (httpResponseCode > 0) {
    response = http.getString();
    Serial.println("Login successful, response:");
    Serial.println(response);
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();

  // Parsing JSON untuk mengambil token
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    Serial.println("Failed to parse JSON");
    return "";
  }

  return doc["result"].as<String>();  // Mengambil token auth
}

// Fungsi untuk mengambil data item dari Zabbix menggunakan auth token
String getZabbixItemData(const String& authToken) {
  HTTPClient http;
  String response = "";

  // Membuat JSON request untuk mengambil data item
  String requestData = "{\"jsonrpc\": \"2.0\", "
                       "\"method\": \"item.get\", "
                       "\"params\": {\"output\": [\"itemid\", \"name\", \"key_\", \"lastvalue\"], "
                       "\"filter\": {\"key_\": \"test.item\"}}, "  // Ganti key_ sesuai dengan item Anda
                       "\"auth\": \"" + authToken + "\", "
                       "\"id\": 2}";

  // Kirim request HTTP POST untuk mengambil data item
  http.begin(zabbixApiUrl);
  http.addHeader("Content-Type", "application/json");

  // Mengirimkan JSON request ke Zabbix
  int httpResponseCode = http.POST(requestData);

  // Mengecek response dari Zabbix
  if (httpResponseCode > 0) {
    response = http.getString();
    Serial.println("Data received from Zabbix:");
    Serial.println(response);
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();
  return response;
}

// Fungsi untuk mengurai dan menampilkan data JSON
void parseJsonData(String jsonResponse) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, jsonResponse);

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  // Mengambil nilai terakhir dari item
  const char* lastValue = doc["result"][0]["lastvalue"];
  Serial.print("Last Value: ");
  Serial.println(lastValue);
}

void setup() {
  Serial.begin(115200);
  
  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Mendapatkan auth token dari Zabbix
  String authToken = getAuthToken();
  if (authToken != "") {
    Serial.println("Auth Token: " + authToken);

    // Mendapatkan data item dari Zabbix
    String zabbixData = getZabbixItemData(authToken);

    // Mengurai data JSON dan menampilkan nilai terakhir
    parseJsonData(zabbixData);
  } else {
    Serial.println("Failed to get auth token");
  }
}

void loop() {
  // Program utama berjalan di sini (misalnya membaca sensor atau lainnya)
  delay(60000);  // Mengambil data setiap 1 menit
}
