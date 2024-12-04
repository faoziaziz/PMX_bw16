#include <WiFi.h>
#include "pmx_snmp.h"

// Konfigurasi WiFi
char* ssid = "WirelessNet";
char* password = "eeeeeeee";

// Objek SNMP
SNMP snmpAgent;
uint16_t port = 161;
void setup() {
    Serial.begin(115200);

    // Hubungkan ke WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");

    // Tampilkan IP Address
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Inisialisasi SNMP
    snmpAgent.begin("public"); // "public" adalah community SNMP
    Serial.println("SNMP Agent started...");
}

void loop() {
    // Pantau dan proses permintaan SNMP
    snmpAgent.listen();
}
