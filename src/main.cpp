#include <Servo.h>
#include <Arduino.h>

Servo servo1, servo2, servo3, servo4, servo5;

void setup() {
    Serial.begin(115200);
    servo1.attach(10); // Index
    servo2.attach(9); // Middle
    servo3.attach(6);  // Ring
    servo4.attach(5);  // Pinky
    servo5.attach(3);  // Thumb
}

void loop() {
    if (Serial.available()) {
        int a1, a2, a3, a4, a5;
        
        String data = Serial.readStringUntil('\n');
        if (sscanf(data.c_str(), "%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &a5) == 5) {
            servo1.write(a1);
            servo2.write(a2);
            servo3.write(a3);
            servo4.write(a4);
            servo5.write(a5);
            
            // print servo angles
            char headersBuffer[64];
            char anglesBuffer[64];
            
            sprintf(headersBuffer, "Index  | Middle | Ring   | Pinky  | Thumb");
            sprintf(anglesBuffer, "%-6d | %-6d | %-6d | %-6d | %d", a1, a2, a3, a4, a5);
            
            Serial.println(headersBuffer);
            Serial.println("-------|--------|--------|--------|------");
            Serial.println(anglesBuffer);
        } else {
            Serial.print("Invalid data: ");
            Serial.println(data);
        }
    }
}
