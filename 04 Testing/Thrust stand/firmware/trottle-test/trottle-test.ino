#include <Arduino.h>
#include <DShotRMT.h>
#include "HX711.h"

// =========================
// DShot
// =========================

const gpio_num_t MOTOR_PIN = GPIO_NUM_6;

DShotRMT motor(MOTOR_PIN, DSHOT300, false);

// =========================
// HX711
// =========================

#define DT_PIN 4
#define SCK_PIN 5

HX711 scale;

const float CALIBRATION_FACTOR = 1127.93;

int throttle = 0;
int thrust;

// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("=== THRUST STAND TEST ===");
  Serial.println("PROP OFF!");
  
  // Initialize DShot
  dshot_result_t result = motor.begin();

  Serial.print("DShot begin: ");
  Serial.println(result.success ? "OK" : "FAILED");

  // Initialize load cell
  scale.begin(DT_PIN, SCK_PIN);

  scale.set_scale(CALIBRATION_FACTOR);

  Serial.println("Taring load cell...");
  scale.tare(100);

  Serial.println("Load cell ready.");

  delay(3000);
}

// =========================
// Loop
// =========================

void loop() {

  // -------------------------
  // STOP / zero-throttle
  // -------------------------

  Serial.println();
  Serial.println("ZERO");

  for (int i = 0; i < 2000; i++) {

    motor.sendThrottle(0);

    delay(1);
  }

  // -------------------------
  // 10% throttle
  // -------------------------
  throttle = 0;
  thrust = 0;

  Serial.println("Throttle 10%");
  while (scale.get_units(5)<62 && throttle < 100) {
    motor.sendThrottlePercent(throttle);
    throttle++;   
  } 
  Serial.print("Throttle: ");
  Serial.println(throttle);

  // -------------------------
  // STOP
  // -------------------------

  Serial.println("STOP");

  for (int i = 0; i < 3000; i++) {

    motor.sendThrottle(0);

    delay(1);
  }
}