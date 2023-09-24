/* -----------------------------------------------------------------
   TIL306 Library Example Sketch 1: Demo
   https://github.com/Andy4495/TIL306

   Example usage of all the library methods.

   This example only controls one decimal point, but will demonstrate
   the use of up to two digits. More digits can be controlled by 
   increasing the counts in cases 1 and 7 and the display value
   in cases 8 and 9.

   01-May-2023 - Andy4495 - Original
   23-Sep-2023 - Andy4495 - Release v1.0.0

*/

#include "TIL306.h"

// Be sure to use the correct pin numbers for your setup.
// Use PWM output pin for BI to control intensity, otherwise BI just controls display on or off
//           CLK,  BI, CLR,  LS
TIL306 myLED( 15,  14,  12,  13 );
// Pin used to control the decimal point
// Note that a separate pin is needed for each decimal point
#define DP 5

unsigned long prev_millis = 0;
#define DELAY_MS 1000

unsigned int state = 0;
unsigned int count = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("TIL30x Library Example Sketch 1: Demonstrate library functionality.");

    digitalWrite(DP, LOW);
    pinMode(DP, OUTPUT);

    myLED.begin();
    myLED.blank(false);         // Need to un-blank the display after startup
    myLED.latch_strobe(true);   // Data in LED latches follow the counter
}

void loop() {
    if (millis() - prev_millis > DELAY_MS) {
        prev_millis = millis();
        switch (state) {
            case 0:
                myLED.clear_counter();
                next_state();
                Serial.println("Count 0 - 18");
                break;
            case 1: 
                myLED.increment(1);
                if (++count >= 18) {
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
                digitalWrite(DP, HIGH);
                next_state();
                break;
            case 6: 
                Serial.println("Turning off decimal point..."); 
                digitalWrite(DP, LOW);
                next_state();
                break;
            case 7: 
                Serial.println("Counting by 2...");
                myLED.latch_strobe(false);
                myLED.increment(2);
                myLED.latch_strobe(true);
                if (++count >= 8) {
                    next_state();
                }
                break;
            case 8: 
                Serial.println("Display 33 as integer.");
                myLED.print(33);
                next_state();
                break;
            case 9:
                Serial.println("Display 21 as a string.");
                myLED.print("21");
                next_state();
                break;
            case 10:
                Serial.println("Display at one-third intensity using PWM");
                Serial.println("BI pin must be configured on a PWM-capable pin to work.");
                myLED.intensity(85);
                next_state();
                break;
            case 11:
                Serial.println("Back to full intensity.");
                myLED.intensity(255);
                next_state();
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
