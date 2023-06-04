/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original

*/

#ifndef TIL306_H
#define TIL306_H

#include "Arduino.h"

class TIL306 {
public:
  enum {NO_PIN = 255};

  // Constructor for all pins
  // Use "TIL306::NO_PIN" for pins that are not tied to the microcontroller.
  TIL306(byte CLK,  byte BI, byte CLR, byte DP, byte LS, byte PCEI, byte RBI, byte SCEI);

  // Minimal pins connected to microcontroller
  // Assumes:
  // - ... To be filled in later....
  TIL306(byte  CLK);

  void begin(); // Set everything to default values
  void count(byte val); // toggles CLK (rising edge) val times // What is default state of CLK?
  void blank(byte val); // 255 = signal low = display fully on ; 0 = signal high = display off
  void blank(bool v); // true = signal high = display off ; false = signal low = display on
  void clear(bool v); // true = signal low = counter 0 ; false = signal low = normal counting
  void decimal_point(bool v); // true = signal high = decimal piont on // May need some additional logic for multi-digit
  void latch_strobe(bool v); // true = signal low = data follows latches ; false = signal high = latches held, counter counts
  void parallel_count_enable(bool v); // true = signal low = normal counting
  void ripple_blank(bool v); // Should this be controlled from library, or connected to RBO?
  void serial_count_enable(bool v); // true = signal low = normal counting ; false = signal high = counter inhibit, MAX-COUNT high
  void write(uint32_t val);
  void write(const char* s);
  void pulse_counter(bool v); // Sets up display so that it acts like a pulse counter
  // Need to think through multiple digit cases; it does not seem as simple as connecting a cascade output
  // It seems like the microcontroller needs to do some work
  // See RBO signal -- does MCU need to monitor -- what does it connect to? 
  // RBO/RBI -- do these cascade? what about RBI on left-most digit?
  // MAX-COUNT -- does MCU need to monitor? waht does it connect to?
  // Having at least one more of these would make it easier to figure out and experiment....

private:
  byte _clk, _bi, _clr, _dp, _ls, _pcei, _rbi, _scei;
  void toggle_clk();

};

#endif
