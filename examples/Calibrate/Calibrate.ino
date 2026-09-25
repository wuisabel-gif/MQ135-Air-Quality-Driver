/*
 * Calibrate the MQ-135. Run in clean outdoor air, let the sensor warm up
 * (24-48h burn-in for a new sensor, ~5 min otherwise), then average the
 * RZero readings and set that value as rzero in your sketch.
 */
#include "MQ135.h"

const int MQ135_PIN = A0;
MQ135 gas(MQ135_PIN);

void setup() {
    Serial.begin(9600);
    // gas.setVoltageDivider(2.83); // if AOUT is fed through a divider
}

void loop() {
    float rzero = gas.getRZero();
    Serial.printlnf("RZero: %.2f  (use this as rzero)", rzero);
    delay(1000);
}
