#include <Arduino.h>
#include <Servo.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo servo1, servo2, servo3, servo4, servo5;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool useDisplay = true;

// 0 = Wired | 1 = Wifi AP
int connectionMode = 0;

const int togglePin = D4;
const int LEDPin = D6;

bool wifiAPIsInitialized = false;
WiFiUDP udp;
IPAddress clientIP;
unsigned int clientPort;
unsigned int port = 4210;
const char* ap_ssid = "ArduinoAP";
const char* ap_pass = "12345678";

static char inputBuffer[64]; // global static buffer for safe returns

void initializeWifiAP()
{
    if (wifiAPIsInitialized) return;
    
    Serial.println("Starting access point...");
    
    WiFi.beginAP(ap_ssid, ap_pass);
    IPAddress ip = WiFi.localIP();
    udp.begin(port);

    Serial.println("AP started!");
    Serial.print("IP: "); Serial.println(ip);
    Serial.print("SSID: "); Serial.println(ap_ssid);
    Serial.print("PASS: "); Serial.println(ap_pass);
    Serial.print("PORT: "); Serial.println(port);
    
    wifiAPIsInitialized = true;
}

char* readDataStringWired(bool *success)
{
    if (!Serial.available()) {
        *success = false;
        return nullptr;
    }

    String data = Serial.readStringUntil('\n');
    data.trim();
    data.toCharArray(inputBuffer, sizeof(inputBuffer));

    *success = true;
    return inputBuffer;
}

char* readDataStringWifi(bool *success)
{
    if (!wifiAPIsInitialized) initializeWifiAP();
    
    int packetSize = udp.parsePacket();
    if (!packetSize) {
        *success = false;
        return nullptr;
    }

    clientIP = udp.remoteIP();
    clientPort = udp.remotePort();

    int len = udp.read(inputBuffer, sizeof(inputBuffer) - 1);
    inputBuffer[len] = '\0';

    *success = true;
    return inputBuffer;
}

void getConnectionMode()
{
  connectionMode = digitalRead(togglePin);
  
  if (connectionMode == 1) initializeWifiAP();
  
  digitalWrite(LEDPin, connectionMode);
}

void showDisplayData()
{
  if (!useDisplay) return;
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println("Connection Mode:");
  if (connectionMode == 0)
    display.println("Wired");
  else if (connectionMode == 1)
  {
    display.println("Wifi AP");
    display.println("\nWifi SSID:");
    display.println(ap_ssid);
    display.println("Wifi Pass:");
    display.println(ap_pass);
  }

  display.display();
}

void setup() {
    Serial.begin(115200);
    delay(500);
    
    pinMode(togglePin, INPUT);
    pinMode(LEDPin, OUTPUT);

    servo1.attach(10);
    servo2.attach(9);
    servo3.attach(6);
    servo4.attach(5);
    servo5.attach(3);

    if (connectionMode == 1) {
        initializeWifiAP();
    }

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println(F("SSD1306 allocation failed"));
      useDisplay = false;
    }
}

void loop() {
    getConnectionMode();
    
    bool success = true;
    char* buffer = nullptr;

    if (connectionMode == 0)
        buffer = readDataStringWired(&success);
    else if (connectionMode == 1)
        buffer = readDataStringWifi(&success);

    if (success && buffer != nullptr)
    {
      int a1, a2, a3, a4, a5;

      if (sscanf(buffer, "%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &a5) == 5) {
          servo1.write(160 - map(a1, 0, 180, 0, 160-90));
          servo2.write(160 - map(a2, 0, 180, 0, 160-90));
          servo3.write(160 - map(a3, 0, 180, 0, 160-90));
          servo4.write(160 - map(a4, 0, 180, 0, 160-90));
          servo5.write(160 - map(a5, 0, 180, 0, 160-90));
      }
    }
    
    showDisplayData();
}
