#include "MQ135.h"
#include <math.h>

MQ135::MQ135(int pin, float rzero, float rload)
    : _pin(pin), _rzero(rzero), _rload(rload) {
    pinMode(_pin, INPUT);
}

// Sensor resistance from the divider: Vpin = adc/ADC_MAX * VREF,
// Vsensor_out = Vpin * divider, Rs = RL * (Vsupply - Vout) / Vout.
float MQ135::getResistance() {
    int adc = analogRead(_pin);
    if (adc <= 0) return NAN;                 // shorted / unpowered
    float vout = (adc / MQ135_ADC_MAX) * MQ135_ADC_VREF * _divider;
    if (vout <= 0 || vout >= _supply) return NAN;
    return _rload * (_supply - vout) / vout;
}

// Correction factor for temperature (C) and humidity (%RH).
float MQ135::getCorrectionFactor(float t, float h) {
    if (t < 20) {
        return MQ135_CORA * t * t - MQ135_CORB * t + MQ135_CORC
               - (h - 33.0) * MQ135_CORD;
    }
    return MQ135_CORE * t + MQ135_CORF * h + MQ135_CORG;
}

float MQ135::getCorrectedResistance(float t, float h) {
    float r = getResistance();
    return r / getCorrectionFactor(t, h);
}

float MQ135::getPPM() {
    return MQ135_PARA * powf(getResistance() / _rzero, -MQ135_PARB);
}

float MQ135::getCorrectedPPM(float t, float h) {
    return MQ135_PARA * powf(getCorrectedResistance(t, h) / _rzero, -MQ135_PARB);
}

// In clean air, ppm == ATMO_CO2, so Rzero = Rs * (ATMO_CO2/PARA)^(1/PARB).
float MQ135::getRZero() {
    return getResistance() * powf(MQ135_ATMO_CO2 / MQ135_PARA, 1.0 / MQ135_PARB);
}

float MQ135::getCorrectedRZero(float t, float h) {
    return getCorrectedResistance(t, h)
           * powf(MQ135_ATMO_CO2 / MQ135_PARA, 1.0 / MQ135_PARB);
}
