/*
 * MQ135.h - MQ-135 air quality / gas sensor driver for Particle devices.
 *
 * Reads the analog output of an MQ-135 module and converts the sensor
 * resistance into a gas concentration (ppm) using the datasheet curve.
 * Default curve constants are tuned for CO2.
 *
 * Photon/Argon ADC is 12-bit (0..4095) referenced to 3.3V. MQ-135 modules
 * are usually powered at 5V and their AOUT can swing above 3.3V. Feed AOUT
 * through a divider (or level shift) so the pin never exceeds 3.3V, and set
 * the divider ratio with setVoltageDivider(). See README.
 */
#pragma once

#include "Particle.h"

// Curve/board defaults. Override in the constructor or with the setters.
#define MQ135_DEFAULT_RLOAD      10.0   // load resistor on the module, kOhm
#define MQ135_DEFAULT_RZERO      76.63  // Rs in clean air (calibrate per sensor!)
#define MQ135_ADC_MAX            4095.0 // 12-bit Particle ADC
#define MQ135_ADC_VREF           3.3    // ADC reference voltage
#define MQ135_SUPPLY_VOLTAGE     5.0    // MQ-135 heater/sensor supply

// CO2 curve: ppm = PARA * (Rs/Rzero) ^ -PARB
#define MQ135_PARA               116.6020682
#define MQ135_PARB               2.769034857
#define MQ135_ATMO_CO2           419.0  // current atmospheric CO2 baseline (ppm)

// Temperature/humidity correction (linear fit from the datasheet curves).
#define MQ135_CORA               0.00035
#define MQ135_CORB               0.02718
#define MQ135_CORC               1.39538
#define MQ135_CORD               0.0018
#define MQ135_CORE              -0.003333333
#define MQ135_CORF              -0.001923077
#define MQ135_CORG               1.130128205

class MQ135 {
public:
    // pin: analog pin wired to AOUT. rload/rzero optional overrides.
    explicit MQ135(int pin,
                   float rzero = MQ135_DEFAULT_RZERO,
                   float rload = MQ135_DEFAULT_RLOAD);

    // Supply voltage of the sensor and the divider ratio (Vsensor/Vpin).
    // Example: a 5V AOUT through a 3.3k/1.8k divider gives ratio ~2.83.
    void setSupplyVoltage(float volts) { _supply = volts; }
    void setVoltageDivider(float ratio) { _divider = ratio; }
    void setRZero(float rzero) { _rzero = rzero; }
    float getRZeroStored() const { return _rzero; }

    // Raw sensor resistance in kOhm.
    float getResistance();
    // Temp/humidity-corrected resistance. t in C, h in %RH.
    float getCorrectedResistance(float t, float h);

    // Gas concentration in ppm (CO2 by default curve).
    float getPPM();
    float getCorrectedPPM(float t, float h);

    // Calibration helpers: run in clean air, store the returned value as rzero.
    float getRZero();
    float getCorrectedRZero(float t, float h);

private:
    int   _pin;
    float _rzero;
    float _rload;
    float _supply  = MQ135_SUPPLY_VOLTAGE;
    float _divider = 1.0;

    float getCorrectionFactor(float t, float h);
};
