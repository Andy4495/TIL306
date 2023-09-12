# TIL306 / TIL307 Library

[![Check Markdown Links](https://github.com/Andy4495/IIL30x/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TIL306/actions/workflows/CheckMarkdownLinks.yml)

This library can be used to control Texas Instruments TIL306 and TIL307 numeric LED displays.

Currently, only single-digit configurations are supported. **Future updates to the library will support multiple digits.**

## Usage

*Refer to the sketches in the `examples` folder.*

1. Include the library header file:  

    ```C++
    #include "TIL306.h"
    ```

2. Instantiate the object.

    For single-digit configurations:

    ```C++
    myLED = TIL306(byte BI, byte CLR, byte CLK, byte DP, byte LS);
    ```

3. Initialize the object:

    ```C++
    myLED.begin();
    ```

4. Prepare display:

    ```c++
    myLED.blank(false);        // turns on the display
    myLED.latch_strobe(true);  // sets display to match counter
    ```

5. Explain useful methods:

    ```C++
    void count(byte val);  // increments the display by val counts
    void blank(bool v);    // true = display off ; false = display on
    void clear(bool v);    // true = set counter to zero ; false = normal counting
    void clear_counter();  // sends a pulse to clear the counter
    void decimal_point(bool v); // right-most decimal point on (true) / off (false)
    void decimal_point(byte digit, bool v); // Multi-digit configurations
    void latch_strobe(bool v); // true = display matches counter ; false = counter updates without changing display
    void write(uint32_t val);  // write val to the display right-justified; extra digits truncated
    void pulse_counter(bool v); // set display to act like a pulse counter; rising edge clock increments the display
    ```

## Example Sketches

**EX1_Single_Digit.ino**  
Runs through various display control examples.

## Device Pinout

Internal to the LED, there is a BCD counter that is updated by the `CLK` signal. This counter data is loaded into the LED segment display latches depending on the `/LS` signal. Other signals are used to further control the display and allow for multi-digit configurations.

| Pin | In/Out | Signal Name   | Description                                                                                     |
| --- | ------ | ------------- | ----------------------------------------------------------------------------------------------- |
|  1  |   Out  | `QB`          | BCD data that drives the decoder. The library does not use this signal.                         |
|  2  |   Out  | `QC`          | BCD data that drives the decoder. The library does not use this signal.                         |
|  3  |   Out  | `QD`          | BCD data that drives the decoder. The library does not use this signal.                         |
|  4  |   Out  | `QA`          | BCD data that drives the decoder. The library does not use this signal.                         |
|  5  |   In   | `/LS`         | LOW: Counter data drives LED decoder latches. HIGH: Counter operated independent of latches.    |
|  6  |   In   | `/RBI`        | Used in multi-digit configurations. The library does not use this signal.                    |
|  7  |   Out  | `MAX-COUNT`   | Used in multi-digit configurations. The library does not use this signal.                       |
|  8  |   Pwr  | `GND`         | Device ground connection.                                                                       |
|  9  |   In   | `/PCEI`       | Used in multi-digit configurations. The library does not use this signal.                     |
| 10  |   In   | `/SCEI`       | Used in multi-digit configurations. The library does not use this signal.                    |
| 11  |   Out  | `RBO`         | Used in multi-digit configurations. The library does not use this signal.                       |
| 12  |   In   | `/CLR`        | HIGH: Normal operation. LOW: Reset and hold counter at zero.                                  |
| 13  |   In   | `DP`          | HIGH: Turn on decimal point (if display is not blanked). LOW: Turn off decimal point.           |
| 14  |   In   | `BI`          | LOW: Normal operation. HIGH: Blank the display; set `RBO` output LOW.                           |
| 15  |   In   | `CLK`         | RISING EDGE: Increment the counter (as long as `/PCEI` and `/SCEI` are LOW and `/CLR` is HIGH). |
| 16  |   Pwr  | `Vcc`         | Device 5 volt power connection. Each display may draw up to 200 mA with all segments on.        |

TIL506 has the decimal point on the left and TIL507 has decimal point on the right. Otherwise, the displays are the same.

The following signals are the only ones that can be controlled with the library. Not all of these are needed for every configuration:

```text
CLK
/LS
BI
/CLR
DP
```

Other signals, while not controlled by the library, require specific connections depending on the configuration. Some configurations are described below. For others, consult the device [data sheet][1] and [application notes][3]

## Typical Configurations

Below are some typical display configurations that can be used with the library, from a minimal-pin single digit operation all the way to full control of a multi-digit diplay. Other configurations are also possible; consult the device datasheet for further information.

### Single Digit Counter; Display Always On

This configuration uses the least amount of pins and creates a counter that cycles from 0 - 9 with each rising clock edge.

Use the constructor:

```cpp
myLED = TIL306(clk_pin);
```

Connect the pins as follows:

| LED Pin | Signal Name | Connect To                                              |
| ------- | ----------- | ------------------------------------------------------- |
|  5      | `/LS`       | GND                                                     |
|  6      | `/RBI`      | GND                                                     |
|  8      | `GND`       | GND                                                     |
|  9      | `/PCEI`     | GND                                                     |
| 10      | `/SCEI`     | GND                                                     |
| 12      | `/CLR`      | +5V                                                     |
| 13      | `DP`        | HIGH: On, LOW: Off                                      |
| 14      | `BI`        | GND                                                     |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor. |
| 16      | `VCC`       | +5V                                                     |

### Single Digit Counter; Display Intensity Controlled By Library

Similar to above and also allows the library to control whether the display is on or off, and brightness level if connected to a PWM pin.

Use the constructor:

```cpp
myLED = TIL306(clk_pin, bi_pin);            // Decimal point not controlled by library
myLED = TIL306(clk_pin, bi_pin, dp_pin);    // Decimal point controlled by library
```

Connect the pins as follows:

| LED Pin | Signal Name | Connect To                                              |
| ------- | ----------- | ------------------------------------------------------- |
|  5      | `/LS`       | GND                                                     |
|  6      | `/RBI`      | GND                                                     |
|  8      | `GND`       | GND                                                     |
|  9      | `/PCEI`     | GND                                                     |
| 10      | `/SCEI`     | GND                                                     |
| 12      | `/CLR`      | +5V                                                     |
| 13      | `DP`        | See [Note 1](#notes) below.                             |
| 14      | `BI`        | Arduino output pin defined as `bi_pin` in constructor.  |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor. |
| 16      | `VCC`       | +5V                                                     |

### Single Digit With Full Control

Display can be configured as a counter or directly display any single decimal digit.

Use the constructor:

```cpp
// Decimal point not controlled by library
myLED = TIL306(clk_pin, bi_pin, clr_pin, ls_pin);

// Decimal point controlled by library
myLED = TIL306(clk_pin, bi_pin, clr_pin, ls_pin, dp_pin);
```

Connect the pins as follows:

| LED Pin | Signal Name | Connect To                                              |
| ------- | ----------- | ------------------------------------------------------- |
|  5      | `/LS`       | Arduino output pin defined by ls_pin in constructor.    |
|  6      | `/RBI`      | GND                                                     |
|  8      | `GND`       | GND                                                     |
|  9      | `/PCEI`     | GND                                                     |
| 10      | `/SCEI`     | GND                                                     |
| 12      | `/CLR`      | Arduino output pin defined as `clr_pin` in constructor. |
| 13      | `DP`        | See [Note 1](#notes) below.                             |
| 14      | `BI`        | Arduino output pin defined as `bi_pin` in constructor.  |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor. |
| 16      | `VCC`       | +5V                                                     |

### Multi-Digit Display

Recommended wiring to create a multiple-digit display (from [*The Optoelectronics Data Book for Design Engineers*][3], Texas Instruments, Fifth Edition, p. 238):

![Cascade][2]

Use the constructor (this defines the common signal lines and the least significant digit):

```cpp
// Don't control CLR line with library; tie to +5V externally
myLED = TIL306(clk_pin, bi_pin, TIL306::NO_PIN, ls_pin, dp_pin);

// Control CLR line with library
myLED = TIL306(clk_pin, bi_pin, clr_pin, ls_pin, dp_pin);
```

Then, define additional display digits. Digits are added right to left (least significant to most significant). The constructor defined the least significant digit, so the first call to `addDigit` creates the second least significant digit.

```cpp
myLED.addDigit(dpn_pin);
```

For example, for a 3 digit display, the following sequence would be used:

```cpp
// Create overall display object, including least significant digit
myLED = TIL306(clk_pin, bi_pin, TIL306::NO_PIN, ls_pin, dp_pin);
// Add "mid-significant" digit
myLED.addDigit(dp2_pin);
// Add most significant digit (left-most digit)
myLED.addDigit(dp3_pin);
```

Connect the pins as follows:

| LED Pin | Signal Name | Connect To                                              |
| ------- | ----------- | ------------------------------------------------------- |
|  5      | `/LS`       | Arduino output pin defined by ls_pin in constructor.    |
|  6      | `/RBI`      | Most significant digit -> connect RBI to GND. For remaining digits, connect RBI to RBO of the digit to the left. |
|  8      | `GND`       | GND                                                     |
|  9      | `/PCEI`     | Least significant digit -> connect PCEI to GND. For remaining digits, connect PCEI together and also to MAX-COUNT of least significant digit. |
| 10      | `/SCEI`     | Least significant digit -> connect SCEI to GND. For the next least significant digit, connect SCEI to GND. For remaining digits, connect SCEI to MAX-COUNT of digit to the right. |
| 12      | `/CLR`      | +5V                                                     |
| 13      | `DP`        | See [Note 2](#notes) below.                             |
| 14      | `BI`        | Arduino output pin defined as `bi_pin` in constructor.  |
| 15      | `CLK`       | Arduino output pin defined as `clk_pin` in constructor. |
| 16      | `VCC`       | +5V                                                     |

#### Notes

1. DP: /// explanation of controlling or not controlling DP
2. /// Add explanation on how muti-digit decimal points are controlled by the library.

## References

- TIL306/307 [datasheet][1]
- [*The Optoelectronics Data Book for Design Engineers*][3] by Texas Instruments

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: http://cdn.goldmine-elec.com/datasheet/G25577.pdf
[2]: ./extras/docs/TIL306-N-digit.jpg
[3]: https://archive.org/details/optoelectronicsd00texa
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/TIL306)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
