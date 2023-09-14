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
  TIL306(byte num_digits, byte CLK,  byte BI, byte CLR, byte LS);

  // Constructor for single-digit configurations
  // Included for simplicity; can also use above constructor and set num_digits = 1
  TIL306(byte BI, byte CLR, byte CLK, byte LS);

  void begin(); // Set everything to default values
  void count(byte val); // toggles CLK (rising edge) val times
  void pwm(byte val); // 255 = signal low = display fully on ; 0 = signal high = display off
  void blank(bool v); // true = signal high = display off ; false = signal low = display on
  void clear(bool v); // true = signal low = counter clear ; false = signal high = normal counting
  void clear_counter(); // sends a pulse to clear the counter
  void latch_strobe(bool v); // true = signal low = data follows latches ; false = signal high = latches held, counter counts
  void write(uint32_t val);

private:
  byte _clk, _bi, _clr, _ls;
  byte _num_digits;

  void toggle_clk();

};

#endif
