#include <cstdio>
#include <cmath>
#include <cassert>
static int g_adc = 0;
void pinMode(int, int) {}
int  analogRead(int) { return g_adc; }
#include "../src/MQ135.h"
#include "../src/MQ135.cpp"

int main() {
    MQ135 gas(0);
    gas.setADC(4095, 3.3);   // board-neutral: assert the math, not the defaults
    g_adc = 1500;
    float rs = gas.getResistance();
    assert(rs > 0 && std::isfinite(rs));
    float rzero = gas.getRZero();
    gas.setRZero(rzero);
    float ppm = gas.getPPM();
    assert(fabs(ppm - MQ135_ATMO_CO2) < 0.5);   // clean-air round trip
    gas.setVoltageDivider(2.0);
    assert(gas.getResistance() < rs);           // divider lowers computed Rs
    g_adc = 0;
    assert(std::isnan(gas.getResistance()));     // unpowered -> NAN
    printf("OK  Rs=%.2f RZero=%.2f ppm=%.2f\n", rs, rzero, ppm);
    return 0;
}
