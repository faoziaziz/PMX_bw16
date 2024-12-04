#include "pmx_snmp.h"

SNMP::SNMP() : port(161) {}

SNMP::~SNMP() {}

void SNMP::begin(const char* communityName, uint16_t port) {
    strncpy(this->community, communityName, sizeof(this->community) - 1);
    this->port = port;
    udp.begin(this->port);
    Serial.println("SNMP Agent initialized.");
}

void SNMP::listen() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        uint8_t buffer[512];
        int len = udp.read(buffer, sizeof(buffer));
        if (len > 0) {
            Serial.print("Received packet of length: ");
            Serial.println(len);
            processPacket(buffer, len);
        }
    }
}

void SNMP::processPacket(uint8_t* buffer, int len) {
    // Cek apakah paket adalah SNMP GetRequest (berdasarkan header sederhana)
    if (buffer[0] == 0x30 && buffer[5] == 0x04 && strncmp((char*)&buffer[6], this->community, strlen(this->community)) == 0) {
        Serial.println("SNMP GetRequest detected.");
        sendSimpleResponse(udp.remoteIP(), udp.remotePort());
    } else {
        Serial.println("Unknown or invalid SNMP packet.");
        sendSimpleResponse(udp.remoteIP(), udp.remotePort());
    }
}

void SNMP::sendSimpleResponse(IPAddress remoteIP, uint16_t remotePort) {
    // Respon SNMP GetResponse yang sangat sederhana
    uint8_t response[] = {
        0x30, 0x26, // SNMP Message SEQUENCE, length
        0x02, 0x01, 0x01, // Version: SNMPv2c
        0x04, 0x06, 'p', 'u', 'b', 'l', 'i', 'c', // Community: "public"
        0xA2, 0x19, // GetResponse-PDU, length
        0x02, 0x04, 0x00, 0x00, 0x00, 0x01, // Request ID
        0x02, 0x01, 0x00, // Error Status: no error
        0x02, 0x01, 0x00, // Error Index: 0
        0x30, 0x0E, // Variable Bindings SEQUENCE, length
        0x30, 0x0C, // Variable Binding SEQUENCE, length
        0x06, 0x08, 0x2B, 0x06, 0x01, 0x02, 0x01, 0x01, 0x01, 0x00, // OID: 1.3.6.1.2.1.1.1.0 (sysDescr.0)
        0x04, 0x01, 'H' // Value: "H"
    };

    udp.beginPacket(remoteIP, remotePort);
    udp.write(response, sizeof(response));
    udp.endPacket();

    Serial.println("Sent SNMP GetResponse.");
}
