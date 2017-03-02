/*
 * IR Remote Control for XBMC/Kodi
 * 
 * Send the standard arrow keys/enter/backspace and play/FFWD/rewind based
 * on receipt of commands from a TV remote.
 * 
 * Uses the IRRemote library from
 * http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html
 * (GitHub: https://github.com/z3t0/Arduino-IRremote).
 * 
 * Specifically works with the oCOSMO CE4031's remote, as is.
 *
 *
 * Made in 2017 by Corban Mailloux
 * corb.co
 */

#include <IRremote.h>
#include "Keyboard.h"

const int receivePin = 11;
IRrecv irrecv(receivePin);
decode_results results;

const int minimumPressTime = 100; // MS to keep the key pressed for

bool repeatable = false;
unsigned long pressTime = 0;

void setup()
{
  Keyboard.begin();
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    // Serial.println(results.value, HEX); // Debug line
    switch (results.value) {
      case 0xFFFFFFFF: // Repeat code
        // Don't release the key unless it's a non repeatable key.
        if (!repeatable) {
          Keyboard.releaseAll();
        }
        break;
      case 0xA50: // Source
        // After pressing the Source button, you have to use the arrow keys.
        // To avoid accidentally picking up those presses as Kodi keys,
        // ignore presses for 5 seconds.
        repeatable = false;
        delay(5000);
        break;
      case 0x2F0: // Up
        repeatable = true;
        setKeyboard(KEY_UP_ARROW);
        break;
      case 0xAF0: // Down
        repeatable = true;
        setKeyboard(KEY_DOWN_ARROW);
        break;
      case 0x2D0: // Left
        repeatable = true;
        setKeyboard(KEY_LEFT_ARROW);
        break;
      case 0xCD0: // Right
        repeatable = true;
        setKeyboard(KEY_RIGHT_ARROW);
        break;
      case 0xD10: // Enter
        repeatable = false;
        setKeyboard(KEY_RETURN);
        break;
      case 0x690: // Back
        repeatable = false;
        setKeyboard(KEY_BACKSPACE); // Backspace
        break;
      case 0x9B0: // Play/Pause
        repeatable = false;
        setKeyboard(' '); // Spacebar
        break;
      case 0x1B0: // Rewind
        repeatable = false;
        setKeyboard('r'); // R
        break;
      case 0x7B0: // Fast Forward
        repeatable = false;
        setKeyboard('f'); // F
        break;
      case 0xFB0: // Star (Context Menu)
        repeatable = false;
        setKeyboard('c'); // C
        break;

      // Soundbar Controls
      case 0xB70: // SoundBar - Power
        repeatable = false;
        setKeyboard(KEY_ESC);
        break;

      case 0xCF0: // SoundBar - Volume Down
      case 0x4F0: // SoundBar - Volume Up
      case 0x0F0: // SoundBar - Source
      case 0x8F0: // SoundBar - Mode
        repeatable = false;
        break;

      // Letter/Color Buttons
      case 0x5F0: // A - Red
        // Nothing
        repeatable = false;
        break;
      case 0xDF0: // B - Green
        // Nothing
        repeatable = false;
        break;
      case 0x3F0: // C - Yellow
        repeatable = false;
        // setKeyboard(0x73); // F24, to trigger the resolution script
        setKeyboard(KEY_LEFT_SHIFT);
        delay(25);
        setKeyboard(KEY_F12);
        break;
      case 0xBF0: // D - Orange
        repeatable = false;
        setKeyboard('w'); // W, to toggle watched status
        repeatable = false;
        break;

      // Non-matching IR
      default:
        repeatable = false;
        break;
    }

    delay(25);
    irrecv.resume(); // Receive the next value
  }
  else {
    unsigned long now = millis();
    if (now - pressTime > minimumPressTime) {
      Keyboard.releaseAll();
    }
  }
}

void setKeyboard(char key)
{
  Keyboard.press(key);
  pressTime = millis();
}
