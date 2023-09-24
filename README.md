# TIL306 / TIL307 Library

[![Arduino Compile Sketches](https://github.com/Andy4495/TIL306/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/TIL306/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/TIL306/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TIL306/actions/workflows/CheckMarkdownLinks.yml)

This library can be used to control Texas Instruments TIL306 (decimal point on left) and TIL307 (decimal point on right) numeric LED displays.

## Hardware Connections

### Control Pins

As few as one or as many as four output pins are needed to control the display, as noted below:

| LED Pin | Signal | Name         | Required | Notes                                                     |
| ------- | ------ | ------------ | ---------| --------------------------------------------------------- |
|    15   | `CLK`  | Clock        |    Yes   | Required by the library to control the display.           |
|    14   |  `BI`  | Blanking     |     No   | May be hardwired LOW to keep the display on all the time. |
|    12   |  `CLR` | Clear        |     No   | May be hardwired HIGH, but the display cannot be forced to zero. |
|     5   |  `LS`  | Latch Strobe |     No   | May be hardwired LOW, but some display flicker may be visible when the display is updated with a new value. |

If any of the optional pins are hardwired, use `TIL306::NO_PIN` as the pin number in the constructor.

### Decimal Points

The decimal point (LED pin 13) is not controlled by the library, and may be controlled with a regular `OUTPUT` pin separately. One pin is needed for each LED digit. If you do not need to control the decimal point, tie it LOW to keep it off or HIGH to keep it on (do not leave it floating).

### Controlling Display Intensity with PWM

To control the level of the display's intensity, connect a PWM-capable pin to `BI`. The `intensity()` method can be use to set the brightness level, with `255` being brightes and `0` turning the display off.

If a non-PWM-capable pin is connected to `BI`, then use the `blank()` method to turn the display on (`true`) or off (`false`).

## Library Usage

*Refer to the sketch in the [`examples`][4] folder.*

1. Include the library header file:  

    ```C++
    #include "TIL306.h"
    ```

2. Instantiate the object.

    ```C++
    myLED = TIL306(byte CLK, byte BI, byte CLR, byte LS);
    ```

3. Initialize and prepare the display:

    ```C++
    myLED.begin();             // Initialize the objec
    myLED.blank(false);        // Turn on the display
    myLED.latch_strobe(true);  // Display latch is loaded with counter value
    ```

4. Methods to control the display:

    ```C++
    void increment(byte val);   // increments the display by val counts
    void intensity(byte val);   // 255 = display fully on, 0 = display off
    void blank(bool v);         // true = display off, false = display on
    void clear(bool v);         // true = set counter to zero, false = normal counting
    void clear_counter();       // sends a low-high pulse to clear the counter
    void latch_strobe(bool v);  // true = display matches counter, false = counter updates without changing display
    void print(uint32_t val);   // write val to the display right-justified; extra digits truncated
    void print(const char* s);  // Converts s to an integer and displays the integer
    ```

## Example Sketch

**EX1_Demo.ino**  
Runs through various display control examples using all of the library methods.

## Device Operation

### Counter and Latch

Internal to the LED, there is a BCD counter that is updated by the `CLK` signal. Each rising clock edge increments the counter by one. This counter data is loaded into the LED segment display latches depending on the `LS` signal. By controlling the `LS` signal, the display can be updated to an arbitray number without having each intermediate value display while clocking in the new digit.

### Other Control Signals

The other control signals on the LED are not used by the library. Consult the [datasheet][1] and [application note][3] for details. A few common configurations are described below.

None of the input signals on the LED should be left floating; each of them should be tied either HIGH or LOW depending on the configuration.

### Typical Configurations

Below are some typical display configurations that can be used with the library, from a minimal-pin single digit operation all the way to full control of a multi-digit diplay.

#### Single Digit Counter with Minimal Pins

This configuration uses a single I/O pin. The display is always on and the latch always displays what's in the counter, meaning that there may be some flicker visible on the display when updating the display by more than one. Also, the display cannot be forced to zero, so this configuration is best used for counting clock pulses rather than displaying arbitrary numbers.

Use the constructor:

```cpp
myLED = TIL306(clk_pin, TIL306::NO_PIN, TIL306::NO_PIN, TIL306::NO_PIN);
```

Connect the pins as follows (pins not listed should be left floating):

| LED Pin | Signal Name | Connect To                    |
| ------- | ----------- | ------------------------------------------------------- |
|  5      | `/LS`       | GND                                                     |
|  6      | `/RBI`      | +5V                                                     |
|  8      | `GND`       | GND                                                     |
|  9      | `/PCEI`     | GND                                                     |
| 10      | `/SCEI`     | GND                                                     |
| 12      | `/CLR`      | +5V                                                     |
| 13      | `DP`        | HIGH: On, LOW: Off            |
| 14      | `BI`        | GND                                                     |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor. |
| 16      | `VCC`       | +5V                                                     |

### Single Digit Using All Control Pins

By using four the control pins, all of the library functionality may be used (including `intensity()` if `BI` is connected to a PWM-capable pin).

Use the constructor:

```cpp
myLED = TIL306(clk_pin, bi_pin, clr_pin, ls_pin);
```

Connect the pins as follows:

| LED Pin | Signal Name | Connect To                    |
| ------- | ----------- | ----------------------------- |
|  5      | `/LS`       | Arduino output pin defined as `ls_pin` in constructor |
|  6      | `/RBI`      | +5V                           |
|  8      | `GND`       | GND                           |
|  9      | `/PCEI`     | GND                           |
| 10      | `/SCEI`     | GND                           |
| 12      | `/CLR`      | Arduino output pin defined as `clr_pin` in constructor                                             |
| 13      | `DP`        | HIGH: On, LOW: Off            |
| 14      | `BI`        | Arduino output pin defined as `bi_pin` in constructor |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor |
| 16      | `VCC`       | +5V                           |

### Multi-Digit Display

Recommended wiring to create a multiple-digit display (from [*The Optoelectronics Data Book for Design Engineers*][3], Texas Instruments, Fifth Edition, p. 238):

![Cascade][2]

Note that the above diagram has the least significant digit `RBI` (LED pin 6) connected to the second least significant digit `RBO` (LED pin 11). This will cause all digits to go blank when displaying zero across all digits. If you would like the value zero to be displayed with a zero in the least significant digit, then hardwire `RBI` HIGH on the least significant digit (and leave `RBO` floating on the second least significant digit).

## References

- TIL306/307 [datasheet][1]
- [*The Optoelectronics Data Book for Design Engineers*][3] by Texas Instruments

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: http://cdn.goldmine-elec.com/datasheet/G25577.pdf
[2]: ./extras/docs/TIL306-N-digit.jpg
[3]: https://archive.org/details/optoelectronicsd00texa
[4]: ./examples/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/TIL306)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
