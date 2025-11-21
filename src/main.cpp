#include <Arduino.h>
#include <Servo.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>

Servo servo1, servo2, servo3, servo4, servo5;

WiFiUDP udp;
IPAddress clientIP;
unsigned int clientPort;
unsigned int port = 4210;
const char* ap_ssid = "ArduinoAP";
const char* ap_pass = "12345678";

void setup() {
    Serial.begin(115200);
    servo1.attach(10); // Index
    servo2.attach(9); // Middle
    servo3.attach(6);  // Ring
    servo4.attach(5);  // Pinky
    servo5.attach(3);  // Thumb
    
    Serial.println("Starting acces point...");
    
    WiFi.beginAP(ap_ssid, ap_pass);
    IPAddress ip = WiFi.localIP();
    udp.begin(port);
    
    Serial.println("Succesfully started acces point");
    Serial.print("IP: ");
    Serial.println(ip);
    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("PASS: ");
    Serial.println(ap_pass);
    Serial.print("PORT: ");
    Serial.println(port);
}

void loop() {
    if (int packetSize = udp.parsePacket()) {
        clientIP = udp.remoteIP();
        clientPort = udp.remotePort();
        
        int a1, a2, a3, a4, a5;
        char buffer[64];
        int len = udp.read(buffer, sizeof(buffer)-1);
        buffer[len] = '\0';

    
        if (sscanf(buffer, "%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &a5) == 5) {
            servo1.write(a1);
            servo2.write(a2);
            servo3.write(a3);
            servo4.write(a4);
            servo5.write(a5);
        }
    }
}
