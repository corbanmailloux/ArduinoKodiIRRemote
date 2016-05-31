/*
 * IR Remote Control for XBMC/Kodi
 * 
 * Send the standard arrow keys/enter/backspace and play/FFWD/rewind based
 * on receipt of commands from my remote.
 * 
 * Uses the IRRemote library from
 * http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html
 * (GitHub: https://github.com/z3t0/Arduino-IRremote)
 * and http://mitchtech.net/arduino-usb-hid-keyboard/'s walk-through on setting up
 * an Arduino to work as a keyboard.
 * 
 * Specifically works with the oCOSMO CE4031's remote, as is.
 *
 *
 * Copyright 2015 Corban Mailloux
 * corb.co
 */

#include <IRremote.h>

uint8_t buf[8] = { 
  0 };  /* Keyboard report buffer */

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    switch (results.value) {
      case 0xA50: // Source
        // After pressing the Source button, you have to use the arrow keys.
        // To avoid accidentally picking up those presses as Kodi keys,
        // ignore presses for 5 seconds.
        delay(5000);
        break;
      case 0x2F0: // Up
        buf[2] = 0x52;
        break;
      case 0xAF0: // Down
        buf[2] = 0x51;
        break;
      case 0x2D0: // Left
        buf[2] = 0x50;
        break;
      case 0xCD0: // Right
        buf[2] = 0x4F;
        break;
      case 0xD10: // Enter
        buf[2] = 0x28;
        break;
      case 0x690: // Back
        buf[2] = 0x2A; // Backspace
        break;
      case 0x9B0: // Play/Pause
        buf[2] = 0x2C; // Spacebar
        break;
      case 0x1B0: // Rewind
        buf[2] = 0x15; // R
        break;
      case 0x7B0: // Fast Forward
        buf[2] = 0x09; // F
        break;
      case 0xFB0: // Star (Context Menu)
        buf[2] = 0x06; // C
        break;
      case 0xB70: // SoundBar - Power (Stop)
        buf[2] = 0x1B; // X
        break;
      case 0xCF0: // SoundBar - Volume Down
        buf[2] = 0x2D; // -
        break;
      case 0x4F0: // SoundBar - Volume Up
        buf[2] = 0x2E; // +
        break;
      case 0x0F0: // SoundBar - Source (Home Screen)
        buf[2] = 0x29; // Esc
        break;
      case 0x8F0: // SoundBar - Mode (Toggle Watched)
        buf[2] = 0x1A; // W
        break;
      // The next four buttons are user-defined, but I don't have anything
      // good for them to do yet, so they send the spacebar.
      case 0x5F0: // A - Red
      case 0xDF0: // B - Green
      case 0x3F0: // C - Yellow
      case 0xBF0: // D - Orange
        buf[2] = 0x2C; // Spacebar
        break;
      default:
        irrecv.resume(); // Receive the next value
        return;
    }

    Serial.write(buf, 8); // Send keypress
    releaseKey();

    irrecv.resume(); // Receive the next value
  }

  delay(100);
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}
