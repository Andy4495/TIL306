/* -----------------------------------------------------------------
   TIL306/TIL307 Library
   https://github.com/Andy4495/TIL306

   01-May-2023 - Andy4495 - Original
   11-Sep-2023 - Andy4495 - Fill in more code

   Pins SCEI, PCEI, and RBI are not controlled by the library, since
   the datasheet and application notes indicate specific hardware 
   wirings between the digits for these signals.

   The library also does not read the status of the latch outputs 
   (QA, QB, QC, QD) or the MAX-COUNT signal.
*/

#include "TIL306.h"

// Constructor with all possible pins
TIL306::TIL306(byte num_digits, byte CLK,  byte BI, byte CLR, byte* dp_array, byte LS) {
  _clk  = CLK;
  _bi   = BI;
  _clr  = CLR;
  _dp   = dp_array;
  _ls   = LS;

  _num_digits = num_digits;

}

// Constructor for single digit configuration
// BI can be set to NO_PIN if the pin is hard-wired to Vcc.
// DP can be set to NO_PIN. Tie pin to Vcc to turn on DP, to GND to turn off DP.
// LS can be set to NO_PIN if the pin is hard-wired to GND.
// 
TIL306::TIL306(byte BI, byte CLR, byte CLK, byte DP, byte LS) {
  _bi  = BI;
  _clk = CLK;
  _clr = CLR;
  _dp  = new byte;
  _dp[0]  = DP;
  _ls  = LS; 

  _num_digits = 1; 

}

void TIL306::begin() {
  // Set up all the outputs to default levels
  // Default levels are generally idle or disabled states
  // 
  if (_clk != NO_PIN) {
    digitalWrite(_clk, HIGH);   // CLK is active on low->high transitions
    pinMode(_clk, OUTPUT);
  }
  if (_bi != NO_PIN) {
    digitalWrite(_bi, HIGH);    // BI HIGH turns off the display
    pinMode(_bi, OUTPUT);
  }
  if (_clr != NO_PIN) {
    digitalWrite(_clr, LOW);    // CLR LOW holds counter at zero
    pinMode(_clr, OUTPUT);
  }
  for (int i = 0; i < _num_digits; i++)
  {
    if (_dp[i] != NO_PIN) {
      digitalWrite(_dp[i], LOW);     // DP LOW turns off decimal point
      pinMode(_dp[i], OUTPUT);
    }
  }
  if (_ls != NO_PIN) {
    digitalWrite(_ls, HIGH);    // LS HIGH data latches are constant (counter independent of latches)
    pinMode(_ls, OUTPUT);
  }
}

// toggles CLK (rising edge) val times 
void TIL306::count(byte val) {
  byte i;
  for (i = 0; i < val; i++) toggle_clk();
}

// PWM pin
// 255 = signal low = display fully on ; 0 = signal high = display off
// values in between control intensity if connected to PWM pin
void TIL306::pwm(byte val) {
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

// Sends a pulse to clear the counter 
void TIL306::clear_counter() {
  if (_clr != NO_PIN) {
    digitalWrite(_clr, LOW);
    digitalWrite(_clr, HIGH);
  }
}

void TIL306::clear(bool v) {
  if (_clr != NO_PIN) {
    if (v) digitalWrite(_clr, LOW);
    else digitalWrite(_clr, HIGH);
  }
}

// Single-digit setup
// true = signal high = decimal point on
void TIL306::decimal_point(bool v) {
  if (_dp[0] != NO_PIN) {
    if (v) digitalWrite(_dp[0], HIGH);
    else   digitalWrite(_dp[0], LOW);
  }
}

// Multi-digit setup
// true = signal high = decimal point on
void TIL306::decimal_point(byte digit, bool v) {
  if (digit < _num_digits) {
    if (_dp[0] != NO_PIN) {
      if (v) digitalWrite(_dp[digit], HIGH);
      else   digitalWrite(_dp[digit], LOW);
    }
  }
}

// true = signal low = data follows latches ; false = signal high = latches held, counter counts
void TIL306::latch_strobe(bool v) {
  if (_ls != NO_PIN) {
    if (v) digitalWrite(_ls, LOW);
    else   digitalWrite(_ls, HIGH);
  }
}

void TIL306::write(uint32_t val) {

}

// Sets up display so that it acts like a pulse counter
void TIL306::pulse_counter(bool v) {

}

void TIL306::toggle_clk() {
  digitalWrite(_clk, LOW);
  digitalWrite(_clk, HIGH);
}
