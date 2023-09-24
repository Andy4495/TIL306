/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original
   23-Sep-2023 - Andy4495 - Release v1.0.0

*/

#ifndef TIL306_H
#define TIL306_H

#include "Arduino.h"

class TIL306 {
public:
  enum {NO_PIN = 255};

  // CLK must be controlled by the library
  // Use NO_PIN if you hardwire any of the other pins:
  //   - BI:  May be hardwired LOW to keep the display on all the time. 
  //          blank() will have no effect.
  //   - CLR: May be hardwired HIGH for normal counting operation.
  //          clear() will have no effect.
  //   - LS:  May be hardwired LOW, but some display flicker may be visible 
  //          when the display is updated with a new value
  TIL306(byte CLK, byte BI, byte CLR, byte LS);

  void begin();               // Set everything to default values
  void increment(byte val);   // toggles CLK (rising edge) val times
  void intensity(byte val);   // 255 = signal low = display fully on ; 0 = signal high = display off
                              // BI must be configured with a PWM-capable pin for the pwm() method to work
  void blank(bool v);         // true = signal high = display off ; false = signal low = display on
  void clear(bool v);         // true = signal low = counter clear ; false = signal high = normal counting
  void clear_counter();       // sends a low-high pulse to clear the counter
  void latch_strobe(bool v);  // true = signal low = data follows latches ; false = signal high = latches held, counter counts
  void print(uint32_t val);   // Display val. Most significant digits in excess of display capacity are discarded
  void print(const char* s);  // Converts s to an integer and displays the integer

private:
  byte _clk, _bi, _clr, _ls;

  void toggle_clk();
};

#endif
