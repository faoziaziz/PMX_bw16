#include <WiFi.h>
#include <WiFiClient.h>

// Konfigurasi WiFi
char* ssid = "WirelessNet";     // Ganti dengan nama WiFi Anda
char* password = "eeeeeeee"; // Ganti dengan password WiFi Anda

// Konfigurasi Zabbix
const char* zabbix_server = "192.168.100.79"; // IP atau hostname Zabbix Server
const int zabbix_port = 10051;               // Port Zabbix Server

// Data yang akan dikirim
char* host = "TestHost";           // Host di Zabbix
char* key = "test.key";              // Key item di Zabbix
char* value = "69";                    // Nilai data yang akan dikirim

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Hubungkan ke WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Membuat payload JSON
  String jsonPayload = String("{\"request\":\"sender data\",\"data\":[{\"host\":\"") +
                       host + "\",\"key\":\"" + key + "\",\"value\":\"" + value + "\"}]}";

  // Membuat header sesuai protokol Zabbix
  uint32_t jsonLength = jsonPayload.length();
  char header[5] = {'Z', 'B', 'X', 'D', 1};
  uint8_t lengthBytes[8] = {
      (uint8_t)(jsonLength & 0xFF),
      (uint8_t)((jsonLength >> 8) & 0xFF),
      (uint8_t)((jsonLength >> 16) & 0xFF),
      (uint8_t)((jsonLength >> 24) & 0xFF),
      0, 0, 0, 0};

  // Kirim data ke Zabbix
  if (client.connect(zabbix_server, zabbix_port)) {
    Serial.println("Connected to Zabbix server");

    // Kirim header
    client.write(header, 5);
    client.write(lengthBytes, 8);

    // Kirim payload JSON
    client.print(jsonPayload);

    // Membaca respons
    while (client.available() == 0) {
      delay(100);
    }

    while (client.available()) {
      String response = client.readString();
      Serial.println("Response from Zabbix:");
      Serial.println(response);
    }

    client.stop();
  } else {
    Serial.println("Failed to connect to Zabbix server");
  }

  delay(60000); // Kirim data setiap 60 detik
}
