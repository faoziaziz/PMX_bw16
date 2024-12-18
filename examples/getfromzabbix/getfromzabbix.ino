#include <WiFi.h>
#include <WiFiClient.h>
#include "parson.h" // Pastikan untuk menambahkan parson.h di folder proyek Anda

// Konfigurasi WiFi
char* ssid = "WirelessNet";        // Ganti dengan SSID WiFi Anda
char* password = "eeeeeeee"; // Ganti dengan password WiFi Anda

// URL dan parameter request
char* host = "192.168.100.79";  // Ganti dengan alamat IP server
const int httpPort = 8000;            // Ganti dengan port server yang sesuai
char* url = "/itemszab?item_id=udah"; // Endpoint yang dituju

void setup() {
  // Memulai serial monitor
  Serial.begin(115200);

  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Membuat koneksi WiFiClient
  WiFiClient client;

  // Memulai koneksi ke server
  if (client.connect(host, httpPort)) {
    Serial.println("Connected to server");

    // Menyiapkan HTTP POST request
    String httpRequest = "POST " + String(url) + " HTTP/1.1\r\n";
    httpRequest += "Host: " + String(host) + "\r\n";
    httpRequest += "Content-Type: application/json\r\n";
    httpRequest += "Accept: application/json\r\n";
    httpRequest += "Content-Length: 20\r\n";  // Panjang data JSON yang dikirim
    httpRequest += "\r\n";
    httpRequest += "{\"name\":\"test.key\"}\r\n"; // Data JSON yang akan dikirimkan

    // Mengirimkan request
    client.print(httpRequest);

    // Menunggu respons dari server dan membaca data JSON
    String response = "";
    while (client.available()) {
      response += (char)client.read();
    }

    // Menampilkan respons di Serial Monitor
    Serial.println("Response from server:");
    Serial.println(response);

    // Parsing JSON untuk mendapatkan "lastdata"
    JSON_Value *rootValue = json_parse_string(response.c_str()); // Mengurai JSON
    if (rootValue != NULL) {
      JSON_Object *rootObject = json_value_get_object(rootValue); // Mengambil objek JSON
      const char *lastdata = json_object_get_string(rootObject, "lastdata"); // Mengambil nilai "lastdata"
      
      if (lastdata != NULL) {
        Serial.print("Value of 'lastdata': ");
        Serial.println(lastdata);
      } else {
        Serial.println("Field 'lastdata' not found.");
      }

      // Membersihkan memori
      json_value_free(rootValue);
    } else {
      Serial.println("Failed to parse JSON.");
    }

  } else {
    Serial.println("Connection failed");
  }

  // Menutup koneksi
  client.stop();
}

void loop() {
  // Tidak ada yang perlu dilakukan dalam loop
}
