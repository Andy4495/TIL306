/* -----------------------------------------------------------------
   TIL306 Library Example Sketch 1: Display a Single Digit
   https://github.com/Andy4495/TIL306

   Test library with a single TIL306/307 chip.
   - Blank the display
   - Intensity levels
   - Clear -- hold counter at zero
   - Turn decimal point on and off
   - Pulse counter: latch connected to counter
   - Direct update: counter updated with latch disconnected

   01-May-2023 - Andy4495 - Original

*/

#include "TIL306.h"

// Be sure to use the correct pin numbers for your setup.
// Use PWM output for BI to control intensity, otherwise BI just controls display on or off
//           CLK,  BI, CLR,  DP,  LS, PCEI, RBI, SCEI
TIL306 myLED( A3,   8,   7,   6,   5,    4,   2,    3);

void setup() {
    Serial.begin(38400);
    Serial.println("");
    Serial.println("TIL30x Library Example Sketch 1: Display a Single Digit.");

    myLED.begin();
}

void loop() {
    myLED.blank(false);
    myLED.clear(false);
    myLED.parallel_count_enable(true);
    myLED.serial_count_enable(true);

    Serial.println("Updating LED by 3s...");
    for (int i = 1; i < 100; i += 3) {
        myLED.count(i);
        delay(1000);
    }

    Serial.println("Clearing display to 0..."); 
    myLED.clear(true);
    delay(3000);

    Serial.println("Blanking display...");
    myLED.blank(true);
    delay(3000);
    myLED.blank(false);

    Serial.println("Turning on decimal point...");
    myLED.decimal_point(true);
    delay(3000);

    Serial.println("Turning off decimal point..."); 
    myLED.decimal_point(true);
    delay(3000);


}
