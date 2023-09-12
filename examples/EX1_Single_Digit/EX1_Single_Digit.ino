/* -----------------------------------------------------------------
   TIL306 Library Example Sketch 1: Display a Single Digit
   https://github.com/Andy4495/TIL306

   Test library with a single TIL306/307 chip.

   01-May-2023 - Andy4495 - Original
   11-Sep-2023 - Andy4495 - Updates for library changes.

*/

#include "TIL306.h"

// Be sure to use the correct pin numbers for your setup.
// Use PWM output for BI to control intensity, otherwise BI just controls display on or off
//            BI, CLR, CLK,  DP,  LS
TIL306 myLED( 19,  12,  15,  14,  13 );

unsigned long prev_millis = 0;
#define DELAY_MS 1000

unsigned int state = 0;
unsigned int count = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("TIL30x Library Example Sketch 1: Display a Single Digit.");

    myLED.begin();
    myLED.blank(false);
    myLED.latch_strobe(true);
}

void loop() {
    if (millis() - prev_millis > DELAY_MS) {
        prev_millis = millis();
        switch (state) {
            case 0:
                myLED.clear_counter();
                next_state();
                Serial.println("Count 0 - 9");
                break;
            case 1: 
                myLED.count(1);
                if (++count >= 9) {
                    next_state();
                }
                break;
            case 2: 
                Serial.println("Resetting counter to 0..."); 
                myLED.clear_counter();
                next_state();
                break;
            case 3: 
                Serial.println("Blanking display...");
                myLED.blank(true);
                next_state();
                break;
            case 4:
                Serial.println("Un-blanking display...");
                myLED.blank(false);
                next_state();
                break;
            case 5: 
                Serial.println("Turning on decimal point...");
                myLED.decimal_point(true);
                next_state();
                break;
            case 6: 
                Serial.println("Turning off decimal point..."); 
                myLED.decimal_point(false);
                next_state();
                break;
            case 7: 
                Serial.println("Counting by 2...");
                myLED.latch_strobe(false);
                myLED.count(2);
                myLED.latch_strobe(true);
                if (++count >= 4) {
                    initial_state();
                }
                break;
            default:
                initial_state();
                break;
        }
    }
}

void next_state() {
    state++;
    count = 0;
}

void initial_state() {
    state = 0;
    count = 0; 
}
