/*
 * MQ135.h - MQ-135 air quality / gas sensor driver for Particle and Arduino.
 *
 * Reads the analog output of an MQ-135 module and converts the sensor
 * resistance into a gas concentration (ppm) using the datasheet curve.
 * Default curve constants are tuned for CO2.
 *
 * ADC differs by board: Particle (Photon/Argon/Boron) is 12-bit (0..4095)
 * at 3.3V; a classic 5V Arduino (Uno/Nano/Mega) is 10-bit (0..1023) at 5V.
 * The right defaults are chosen at compile time. For anything else (ESP32,
 * SAMD, 3.3V AVR, external ADC) call setADC(counts, vref).
 *
 * MQ-135 modules are usually powered at 5V and their AOUT can swing above a
 * 3.3V pin. On 3.3V boards feed AOUT through a divider so the pin never
 * exceeds its reference, and set the ratio with setVoltageDivider().
 */
#pragma once

#if defined(PARTICLE) || defined(SPARK)
  #include "Particle.h"
  #define MQ135_DEFAULT_ADC_MAX  4095.0  // 12-bit
  #define MQ135_DEFAULT_VREF     3.3
#else
  #include "Arduino.h"
  #define MQ135_DEFAULT_ADC_MAX  1023.0  // 10-bit classic AVR
  #define MQ135_DEFAULT_VREF     5.0
#endif

// Board defaults. Override in the constructor or with the setters.
#define MQ135_DEFAULT_RLOAD      10.0   // load resistor on the module, kOhm
#define MQ135_DEFAULT_RZERO      76.63  // Rs in clean air (calibrate per sensor!)
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

    // ADC full-scale counts and reference voltage. Defaults are set by the
    // board; call this for ESP32/SAMD/3.3V-AVR or an external ADC.
    void setADC(float counts, float vref) { _adcMax = counts; _vref = vref; }

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
    float _adcMax  = MQ135_DEFAULT_ADC_MAX;
    float _vref    = MQ135_DEFAULT_VREF;

    float getCorrectionFactor(float t, float h);
};
