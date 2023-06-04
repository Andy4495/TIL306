/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original
*/

#include "TIL306.h"

// Constructor with all possible pins
TIL306::TIL306(byte CLK,  byte BI, byte CLR, byte DP, byte LS, byte PCEI, byte RBI, byte SCEI) {
  _clk  = CLK;
  _bi   = BI;
  _clr  = CLR;
  _dp   = DP;
  _ls   = LS;
  _pcei = PCEI;
  _rbi  = RBI;
  _scei = SCEI;

}

// Constructor for minimal pins connected to microcontroller
// Assumes:
// - ... To be filled in later....
TIL306::TIL306(byte  CLK) {
  _clk = CLK;
}

void TIL306::begin() {
  // Set up all the outputs to default levels
  // Default levels are generally idle or disabled states
  digitalWrite(_clk, HIGH);   // CLK is active on low->high transitions
  pinMode(_clk, OUTPUT);
  if (_bi != NO_PIN) {
    digitalWrite(_bi, HIGH);    // BI HIGH turns off the display
    pinMode(_bi, OUTPUT);
  }
  if (_clr != NO_PIN) {
    digitalWrite(_clr, LOW);    // CLR LOW holds counter at zero
    pinMode(_clr, OUTPUT);
  }
  if (_dp != NO_PIN) {
    digitalWrite(_dp, LOW);     // DP LOW turns off decimal point
    pinMode(_dp, OUTPUT);
  }
  if (_ls != NO_PIN) {
    digitalWrite(_ls, HIGH);    // LS HIGH data latches are constant (counter independent of latches)
    pinMode(_ls, OUTPUT);
  }
  if (_pcei != NO_PIN) {
    digitalWrite(_pcei, HIGH);  // PCEI HIGH counter is inhibited. Do not change when CLK is LOW
    pinMode(_pcei, OUTPUT);
  }
  if (_rbi != NO_PIN) {
    digitalWrite(_rbi, HIGH);   // RBI HIGH does not blank display when latches are zero
    pinMode(_rbi, OUTPUT);
  }
  if (_scei != NO_PIN) {
    digitalWrite(_scei, HIGH);  // SCEI HIGH counter is inhibited, MAX-COUNT is HIGH. Do not change when CLK is LOW
    pinMode(_scei, OUTPUT);
  }
}

// toggles CLK (rising edge) val times 
void TIL306::count(byte val) {
  byte i;
  for (i = 0; i < (val % 10); i++) toggle_clk();
}

// PWM pin
// 255 = signal low = display fully on ; 0 = signal high = display off
// values in between control intensity if connected to PWM pin
void TIL306::blank(byte val) {
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

// true = signal low = counter 0 ; false = signal low = normal counting
void TIL306::clear(bool v) {
  if (_clr != NO_PIN) {
    if (v) digitalWrite(_clr, LOW);
    else   digitalWrite(_clr, HIGH);
  }
}

// true = signal high = decimal piont on // May need some additional logic for multi-digit
void TIL306::decimal_point(bool v) {
  if (_dp != NO_PIN) {
    if (v) digitalWrite(_dp, HIGH);
    else   digitalWrite(_dp, LOW);
  }
}

// true = signal low = data follows latches ; false = signal high = latches held, counter counts
void TIL306::latch_strobe(bool v) {
  if (_ls != NO_PIN) {
    if (v) digitalWrite(_ls, LOW);
    else   digitalWrite(_ls, HIGH);
  }
}

// true = signal low = normal counting
void TIL306::parallel_count_enable(bool v) {
  if (_pcei != NO_PIN) {
    if (v) digitalWrite(_pcei, LOW);
    else   digitalWrite(_pcei, HIGH);
  }
}

// Should this be controlled from library, or connected to RBO?
// true = signal low = display blanked when latches = 0
void TIL306::ripple_blank(bool v) {
  if (_rbi != NO_PIN) {
    if (v) digitalWrite(_rbi, LOW); 
    else   digitalWrite(_rbi, HIGH);
  }
}

// true = signal low = normal counting ; false = signal high = counter inhibit, MAX-COUNT high
void TIL306::serial_count_enable(bool v) {
  if (_scei != NO_PIN) {
    if (v) digitalWrite(_scei, LOW);
    else   digitalWrite(_scei, HIGH);
  }
}

void TIL306::write(uint32_t val) {

}

void TIL306::write(const char* s) {

}
// Sets up display so that it acts like a pulse counter
void TIL306::pulse_counter(bool v) {

}

void TIL306::toggle_clk() {
  digitalWrite(_clk, LOW);
  digitalWrite(_clk, HIGH);
}
