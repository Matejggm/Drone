#include <Arduino.h>
#include <DShotRMT.h>

const gpio_num_t MOTOR_PIN = GPIO_NUM_6;

// DShot300, unidirectional
DShotRMT motor(MOTOR_PIN, DSHOT300, false);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("=== DShot300 TEST ===");
  Serial.println("PROP OFF!");

  dshot_result_t result = motor.begin();

  Serial.print("begin() result: ");
  Serial.println(result.success ? "OK" : "FAILED");

  delay(3000);

  Serial.println("ESC initialized");
}

void loop() {

  // Keep sending STOP commands
  // so the ESC receives DShot continuously.
  for (int i = 0; i < 2000; i++) {
    motor.sendThrottle(0);
    delay(1);
  }

  Serial.println("Throttle 20%");

  for (int i = 0; i < 3000; i++) {
    motor.sendThrottlePercent(20.0);
    delay(1);
  }

  Serial.println("STOP");

  for (int i = 0; i < 3000; i++) {
    motor.sendThrottle(0);
    delay(1);
  }
}