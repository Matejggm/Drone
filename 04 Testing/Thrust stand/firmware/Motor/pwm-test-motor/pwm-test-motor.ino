#include <ESP32Servo.h>

Servo esc;

const int ESC_PIN = 6;

void setup() {
  Serial.begin(115200);

  esc.attach(ESC_PIN, 1000, 2000);

  // Arm ESC
  esc.writeMicroseconds(1000);
  Serial.println("ESC armed");

  delay(5000);
}

void loop() {

  Serial.println("20%");
  esc.writeMicroseconds(1200);
  delay(3000);

  Serial.println("Stop");
  esc.writeMicroseconds(1000);
  delay(3000);
}