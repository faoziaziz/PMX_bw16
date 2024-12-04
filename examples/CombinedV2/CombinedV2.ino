/*
 * This sketch demonstrates how to use TFT LCD with ILI9314 api
 * 
 * Pre-requirement:
 *     an ILI9341 TFT LCD with SPI interface
 * 
 * An ILI9341 TFT LCD with SPI interface can be used with spi to
 * send command and data. We can draw text, line, circle, and
 * other picture on it.
 *
 * Example guide:
 * https://www.amebaiot.com/en/amebad-arduino-spi-lcd/
 */


#include <WiFi.h>
#include <PubSubClient.h>
#include "SPI.h"
#include "AmebaILI9341.h"
#include "AmebaLogo.h"
#include <PMX_bw16.h>
#include <PMX_aht10.h>

#define TFT_RESET 3
#define TFT_DC 6
#define TFT_CS SPI_SS
#define SPI_BUS SPI

char ssid[] = "WirelessNet";       // your network SSID (name)
char pass[] = "eeeeeeee";           // your network password
int status = WL_IDLE_STATUS;        // Indicator of Wifi status

char mqttServer[]     = "192.168.100.175";
char clientId[]       = "Zabuntu";
char publishTopic1[]   = "temp";
char publishTopic2[]   = "hum";



AHT10 pmxAHT(AHT10_ADDRESS_0X38, AHT10_SENSOR);
// Select 2 GPIO pins connect to TFT_RESET and TFT_DC. And default SPI_SS/SPI1_SS connect to TFT_CS.


AmebaILI9341 tft = AmebaILI9341(TFT_CS, TFT_DC, TFT_RESET, SPI_BUS);

#define ILI9341_SPI_FREQUENCY 20000000
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
    }
    Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
    // Loop until we're reconnected
    while (!(client.connected())) {
        Serial.print("\r\nAttempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientId)) {
            Serial.println("connected");
            //Once connected, publish an announcement and resubscribe
       //     client.publish(publishTopic, publishPayload);
            //client.subscribe(subscribeTopic);
        } else {
            Serial.println("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            //Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
void setup() {
  Serial.begin(115200);
  Serial.println("ILI9341 Test!");
  // put your setup code here, to run once:


  while (pmxAHT.begin() != true) {
    Serial.println("Gak bisa kedetek aht10 nya ");
    delay(5000);
  }

  //Attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }

    client.setServer(mqttServer, 1883);
    client.setCallback(callback);

    //Allow Hardware to sort itself out
    delay(1500);

  Serial.println("Aht10 kedetek");
  tft.begin();
  SPI_BUS.setDefaultFrequency(ILI9341_SPI_FREQUENCY);

  Serial.println("test filling screen");
  testFillScreen();
  delay(500);

  Serial.println("test Lines");
  testLines(ILI9341_CYAN);
  delay(500);

  Serial.println("test Circles");
  testCircles(5, ILI9341_BLUE);
  delay(500);

  Serial.println("test Circles");
  testRectangle(ILI9341_LIGHTGREY);
  delay(500);

  Serial.println("test Bitmap");
  testBitmap(0, 0, logoWidth, logoHeight, logo);
  delay(500);

  Serial.println("test Text");
  testText();
  delay(500);
  tft.clr();
}

void loop() {

  Serial.println("baca 12 bytes");
  Serial.print("temperature : "); Serial.print(pmxAHT.readTemperature());
  Serial.println();
  Serial.print("humidity : "); Serial.print(pmxAHT.readHumidity());
  Serial.println();
  tft.setCursor(0, 0);

  tft.setForeground(ILI9341_GREEN);
  tft.setFontSize(3);
  tft.println("Suhu");

  tft.setForeground(ILI9341_GREEN);
  tft.setFontSize(5);
  tft.println(pmxAHT.readTemperature());


  tft.setForeground(ILI9341_GREEN);
  tft.setFontSize(3);
  tft.println("Kelembaban");

  tft.setForeground(ILI9341_GREEN);
  tft.setFontSize(5);
  tft.println(pmxAHT.readHumidity());

  
  if (client.connect(clientId)) {
            Serial.println("connected");
            //Once connected, publish an announcement and resubscribe
            client.publish("prasimax/temp", String(pmxAHT.readTemperature()).c_str());
            client.publish("prasimax/hum", String(pmxAHT.readHumidity()).c_str());
            //client.subscribe(subscribeTopic);
        } else {
            Serial.println("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            //Wait 5 seconds before retrying
            delay(5000);
      }
}

unsigned long testFillScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_BLACK);

  return 0;
}

unsigned long testText() {
 
  return 0;
}

unsigned long testLines(uint16_t color) {
  int x0, y0, x1, y1;
  int w = tft.getWidth();
  int h = tft.getHeight();

  tft.clr();

  x0 = 0;
  x1 = w;
  for (y0 = y1 = 0; y0 <= h; y0 += 5, y1 += 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }
  y0 = 0;
  y1 = h;
  for (x0 = x1 = 0; x0 <= w; x0 += 5, x1 += 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }

  tft.fillScreen(ILI9341_BLACK);

  x0 = w / 2;
  y0 = h / 2;
  x1 = 0;
  for (y1 = 0; y1 < h; y1 += 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }

  y1 = h;
  for (x1 = 0; x1 < w; x1 += 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }

  x1 = w;
  for (y1 = h; y1 >= 0; y1 -= 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }

  y1 = 0;
  for (x1 = w; x1 >= 0; x1 -= 5) {
    tft.drawLine(x0, y0, x1, y1, color);
  }

  return 0;
}

void testRectangle(uint16_t color) {
  int rw, rh;
  int div = 60;
  int w = tft.getWidth();
  int h = tft.getHeight();

  tft.clr();

  for (rw = w / div, rh = h / div; rw < w; rw += w / div, rh += h / div) {
    tft.drawRectangle((w / 2 - rw), (h / 2 - rh), (rw * 2), (rh * 2), color);
  }
}

void testCircles(uint8_t radius, uint16_t color) {
  int x, y;
  int w = tft.getWidth();
  int h = tft.getHeight();

  tft.clr();

  for (x = radius; x < w; x += radius * 2) {
    for (y = radius; y < h; y += radius * 2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const unsigned short *color) {
  tft.clr();
  tft.drawBitmap(x, y, w, h, color);
  delay(500);
}
