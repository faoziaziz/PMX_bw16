#ifndef __PMX_SNMP_H
#define __PMX_SNMP_H
#include <Arduino.h>
#include <WiFiUdp.h>// Library UDP untuk BW16

class SNMP {
public:
    SNMP();
    ~SNMP();

    void begin(const char* community, uint16_t port = 161);
    void listen(); // Fungsi untuk memantau permintaan SNMP
    void processPacket(uint8_t* buffer, int len); // Fungsi untuk memproses paket SNMP masuk
    void sendResponse(); // Fungsi untuk mengirim respon
    void sendSimpleResponse(IPAddress remoteIP, uint16_t remotePort);
private:
    WiFiUDP udp;
    char community[64];
    uint16_t port;

    void parsePacket();
    void buildResponse();
};

#endif
