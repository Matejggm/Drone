#include "HX711.h"

#define DT_PIN 4
#define SCK_PIN 5

HX711 scale;

void setup() {
  Serial.begin(115200);

  scale.begin(DT_PIN, SCK_PIN);

  scale.set_scale(1127.93); // tvůj faktor
  scale.tare(100);
}

void loop() {
  float grams = scale.get_units(10);

  Serial.print("Hmotnost: ");
  Serial.print(grams);
  Serial.println(" g");

  delay(200);
}