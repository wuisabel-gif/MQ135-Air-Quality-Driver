# MQ135

MQ-135 air quality / hazardous gas sensor driver for Particle devices
(Photon, Argon, Boron, P1, Electron).

Converts the sensor's analog output to resistance and to a gas concentration
in ppm (CO2-equivalent by default) using the datasheet curve, with optional
temperature/humidity correction.

## Wiring

| MQ-135 module | Particle |
|---------------|----------|
| VCC           | 5V (VIN / VUSB) |
| GND           | GND |
| AOUT          | A0 (through a divider, see below) |
| DOUT          | not used |

**ADC voltage warning.** MQ-135 modules run at 5V and AOUT can swing above
3.3V. Particle analog pins are 3.3V max. Put AOUT through a resistor divider
(e.g. 3.3k top / 1.8k bottom ≈ ratio 2.83) so the pin stays under 3.3V, then
tell the driver the ratio:

```cpp
gas.setVoltageDivider(2.83); // Vsensor / Vpin
```

If your board's AOUT already stays under 3.3V, leave the divider at 1.0.

## Usage

```cpp
#include "MQ135.h"

MQ135 gas(A0, 76.63); // pin, calibrated RZero

void setup() { Serial.begin(9600); }

void loop() {
    Serial.printlnf("%.1f ppm", gas.getPPM());
    delay(10000);
}
```

## Calibration

RZero is the sensor resistance in clean air and differs per sensor. Run the
`Calibrate` example outdoors after warm-up, average the printed RZero, and pass
it to the constructor or `setRZero()`. A fresh sensor needs a 24-48h burn-in.

## API

- `MQ135(pin, rzero = 76.63, rload = 10.0)`
- `getResistance()` / `getCorrectedResistance(t, h)` — kOhm
- `getPPM()` / `getCorrectedPPM(t, h)` — ppm
- `getRZero()` / `getCorrectedRZero(t, h)` — calibration
- `setSupplyVoltage(v)`, `setVoltageDivider(ratio)`, `setRZero(r)`

`t` is temperature in C, `h` is relative humidity in %.

## License

MIT
