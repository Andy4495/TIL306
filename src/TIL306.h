/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original
   11-Sep-2023 - Andy4495 - Fill in more code

*/

#ifndef TIL306_H
#define TIL306_H

#include "Arduino.h"

class TIL306 {
public:
  enum {NO_PIN = 255};

  // Constructor for all pins
  // Use "TIL306::NO_PIN" for pins that are not tied to the microcontroller.
  TIL306(byte num_digits, byte CLK,  byte BI, byte CLR, byte* dp_array, byte LS);

  // Constructor for single-digit configurations
  // Included for simplicity; can also use above constructor and set num_digits = 1
  TIL306(byte BI, byte CLR, byte CLK, byte DP, byte LS);

  void begin(); // Set everything to default values
  void count(byte val); // toggles CLK (rising edge) val times // What is default state of CLK?
  void pwm(byte val); // 255 = signal low = display fully on ; 0 = signal high = display off
  void blank(bool v); // true = signal high = display off ; false = signal low = display on
  void clear(bool v); // true = signal low = counter clear ; false = signal high = normal counting
  void clear_counter(); // sends a pulse to clear the counter
  void decimal_point(bool v); // Single digit setup: true = signal high = decimal piont on 
  void decimal_point(byte digit, bool v); // Multi-digit setup. digit in range [0, num_digits -1]; other values ignored
  void latch_strobe(bool v); // true = signal low = data follows latches ; false = signal high = latches held, counter counts
  void parallel_count_enable(bool v); // true = signal low = normal counting
  void ripple_blank(bool v); // Should this be controlled from library, or connected to RBO?
  void serial_count_enable(bool v); // true = signal low = normal counting ; false = signal high = counter inhibit, MAX-COUNT high
  void write(uint32_t val);
  void pulse_counter(bool v); // Sets up display so that it acts like a pulse counter

private:
  byte _clk, _bi, _clr, _ls;
  byte *_dp;
  byte _num_digits;

  void toggle_clk();

};

#endif
