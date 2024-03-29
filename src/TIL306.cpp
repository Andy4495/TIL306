/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original
   23-Sep-2023 - Andy4495 - Release v1.0.0

   Pins SCEI, PCEI, and RBI are not controlled by the library, since
   the datasheet and application notes indicate specific hardware 
   wirings between the digits for these signals.

   The library also does not read the status of the latch outputs 
   (QA, QB, QC, QD) or the MAX-COUNT signal.
*/

#include "TIL306.h"

// Constructor for single digit configuration
// BI can be set to NO_PIN if the pin is hard-wired to Vcc.
// LS can be set to NO_PIN if the pin is hard-wired to GND.
// 
TIL306::TIL306(byte CLK, byte BI, byte CLR, byte LS) {
  _bi  = BI;
  _clk = CLK;
  _clr = CLR;
  _ls  = LS; 
}

void TIL306::begin() {
  // Set up all the outputs to default levels
  // Default levels are generally idle or disabled states
  // 
  pinMode(_clk, OUTPUT);
  if (_bi != NO_PIN) {
    digitalWrite(_bi, HIGH);    // BI HIGH turns off the display
    pinMode(_bi, OUTPUT);
  }
  if (_clr != NO_PIN) {
    digitalWrite(_clr, LOW);    // CLR LOW holds counter at zero
    pinMode(_clr, OUTPUT);
  }
  if (_ls != NO_PIN) {
    digitalWrite(_ls, HIGH);    // LS HIGH data latches are constant (counter independent of latches)
    pinMode(_ls, OUTPUT);
  }
}

// toggles CLK (rising edge) val times 
void TIL306::increment(byte val) {
  for (; val > 0; val--) { 
    toggle_clk();
  }
}

// Display intensity with PWM-capable pin
// 255 = signal low = display fully on ; 0 = signal high = display off
// values in between control intensity if connected to PWM pin
void TIL306::intensity(byte val) {
  if (_bi != NO_PIN) analogWrite(_bi, 255 - val); 
}

// Non-PWM digital pin 
// true = signal high = display off ; false = signal low = display on
void TIL306::blank(bool v) {
  if (_bi != NO_PIN) {
    if (v) digitalWrite(_bi, HIGH); 
    else   digitalWrite(_bi, LOW);
  }
}

void TIL306::clear(bool v) {
  if (_clr != NO_PIN) {
    if (v) digitalWrite(_clr, LOW);
    else digitalWrite(_clr, HIGH);
  }
}

// Sends a low-high pulse to clear the counter 
void TIL306::clear_counter() {
  if (_clr != NO_PIN) {
    digitalWrite(_clr, LOW);
    digitalWrite(_clr, HIGH);
  }
}

// true = signal low = data follows latches ; false = signal high = latches held, counter counts
void TIL306::latch_strobe(bool v) {
  if (_ls != NO_PIN) {
    if (v) digitalWrite(_ls, LOW);
    else   digitalWrite(_ls, HIGH);
  }
}

void TIL306::print(uint32_t val) {
  clear_counter();
  latch_strobe(false);
  increment(val);
  latch_strobe(true);
}

void TIL306::print(const char* s) {
  uint32_t num;
  num = atoi(s);
  print(num);
}

void TIL306::toggle_clk() {
  digitalWrite(_clk, LOW);
  digitalWrite(_clk, HIGH);
}
