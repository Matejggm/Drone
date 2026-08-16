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

// =========================
// Test settings
// =========================

const int MEASUREMENTS = 10;
const int SETTLE_TIME_MS = 1000;


float sum = 0;
float grams;
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
  Serial.println();
  Serial.println("ZERO");

  for (int i = 0; i < 2000; i++) {

    motor.sendThrottle(0);

    delay(1);
  }

  // Test every 10%
  for (int throttle = 10; throttle <= 100; throttle += 10) {

    Serial.println();
    Serial.print("=== THROTTLE ");
    Serial.print(throttle);
    Serial.println("% ===");

    // Set throttle
    
    for (int j = 0; j <= throttle +1; j++) {
      for (int i = 0; i < 10; i++) {
        motor.sendThrottlePercent(j);
        delay(10);
      }
    }
    sum = 0;

    for (int j = 0; j < 10; j++) {
      //for (int i = 0; i < 10; i++) {

      motor.sendThrottlePercent(throttle);
      float grams = scale.get_units(5);
        //delay(10);
      //}

    
      sum += grams;

      Serial.print("Measurement ");
      Serial.print(j + 1);
      Serial.print(": ");
      Serial.print(grams, 2);
      Serial.println(" gf");
    }
    

    // -------------------------
    // Measure 10 values
    // -------------------------
    // -------------------------
    // Calculate average
    // -------------------------

    float average = sum / 10;

    Serial.print("AVERAGE | Throttle: ");
    Serial.print(throttle);
    Serial.print("% | Thrust: ");
    Serial.print(average, 2);
    Serial.println(" gf");

    // Stop motor between tests
    for (int i = 0; i < SETTLE_TIME_MS; i++) {

      motor.sendThrottle(0);

      delay(1);
    }

  }

  // -------------------------
  // Test finished
  // -------------------------

  Serial.println();
  Serial.println("=== TEST COMPLETE ===");

  motor.sendThrottle(0);

  // Wait before repeating
  delay(5000);
}