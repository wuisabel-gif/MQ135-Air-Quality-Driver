/*
 * Calibrate the MQ-135. Run in clean outdoor air, let the sensor warm up
 * (24-48h burn-in for a new sensor, ~5 min otherwise), then average the
 * RZero readings and set that value as rzero in your sketch.
 * Works on Particle and Arduino.
 */
#include "MQ135.h"

const int MQ135_PIN = A0;
MQ135 gas(MQ135_PIN);

void setup() {
    Serial.begin(9600);
    // gas.setVoltageDivider(2.83); // if AOUT is fed through a divider
}

void loop() {
    Serial.print("RZero: ");
    Serial.print(gas.getRZero(), 2);
    Serial.println("  (use this as rzero)");
    delay(1000);
}
