/*
 * Read CO2-equivalent air quality (ppm) from an MQ-135.
 * Works on Particle and Arduino. Replace RZERO with the value from the
 * Calibrate example for your sensor.
 */
#include "MQ135.h"

const int   MQ135_PIN = A0;
const float RZERO     = 76.63; // <-- your calibrated value

MQ135 gas(MQ135_PIN, RZERO);

void setup() {
    Serial.begin(9600);
    // gas.setADC(4095, 3.3);       // e.g. ESP32; auto on Particle/AVR
    // gas.setVoltageDivider(2.83); // if AOUT is fed through a divider
}

void loop() {
    float ppm = gas.getPPM();
    Serial.print("Air quality: ");
    Serial.print(ppm, 1);
    Serial.println(" ppm");
#if defined(PARTICLE) || defined(SPARK)
    Particle.publish("air_quality_ppm", String(ppm, 1), PRIVATE);
#endif
    delay(10000);
}
