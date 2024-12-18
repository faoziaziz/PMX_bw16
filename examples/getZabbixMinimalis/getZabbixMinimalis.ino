#include <WiFi.h>

char* ssid = "WirelessNet";  // Ganti dengan SSID Wi-Fi Anda
char* password = "eeeeeeeee";  // Ganti dengan password Wi-Fi Anda
char* host = "192.168.100.79";  // Ganti dengan IP Zabbix Server
const int port = 80;  // Port untuk HTTP (biasanya 80 untuk HTTP biasa)

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Menghubungkan ke Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi!");

  // Membuka koneksi ke server Zabbix
  if (client.connect(host, port)) {
    Serial.println("Terkoneksi ke server Zabbix!");
  } else {
    Serial.println("Gagal menghubungkan ke server Zabbix");
    return;
  }

  // Membuat JSON-RPC request
  String jsonRequest = "{\"jsonrpc\": \"2.0\", \"method\": \"apiinfo.version\", \"params\": [], \"id\": 1}";

  // Mengirim request JSON-RPC
  client.print("POST /zabbix/api_jsonrpc.php HTTP/1.1\r\n");
  client.print("Host: 192.168.100.79\r\n");
  client.print("Content-Type: application/json-rpc\r\n");
  client.print("User-Agent: arduino/1.0\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Length: ");
  client.print(jsonRequest.length());
  client.print("\r\n\r\n");
  client.print(jsonRequest);

  Serial.println("Request JSON-RPC dikirim!");

  // Membaca respons dari server
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  client.stop();  // Menutup koneksi setelah menerima respons
}

void loop() {
  // Tidak ada kode di loop untuk saat ini
}
