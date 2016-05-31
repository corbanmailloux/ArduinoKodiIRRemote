/*
 * IR Remote Control for XBMC/Kodi
 * 
 * Send the standard arrow keys/enter/backspace and play/FFWD/rewind based
 * on receipt of commands from a TV remote.
 * 
 * Uses the IRRemote library from
 * http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html
 * (GitHub: https://github.com/z3t0/Arduino-IRremote)
 * and the guide at http://mitchtech.net/arduino-usb-hid-keyboard/ to set up
 * an Arduino to work as a keyboard.
 * 
 * Specifically works with the oCOSMO CE4031's remote, as is.
 *
 *
 * Made in 2015 by Corban Mailloux
 * corb.co
 */

#include <IRremote.h>

uint8_t buf[8] = { 
  0 };  /* Keyboard report buffer */

int RECV_PIN = 11;
// Send pin = 3

IRrecv irrecv(RECV_PIN);
IRsend irsend; // Send pin 3

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

      // Air Conditioner Controls
      case 0xB70: // SoundBar - Power
        sendAirConditionerIR(0x8877); // AC power
        return;
      case 0xCF0: // SoundBar - Volume Down
        sendAirConditionerIR(0xB04F); // Temperature down
        return;
      case 0x4F0: // SoundBar - Volume Up
        sendAirConditionerIR(0x708F); // Temperature up
        return;
      case 0x0F0: // SoundBar - Source
        sendAirConditionerIR(0x906F); // AC mode
        return;
      case 0x8F0: // SoundBar - Mode
        sendAirConditionerIR(0xE01F); // Fan mode
        return;
      case 0x5F0: // A - Red
        sendAirConditionerIR(0x20DF); // Fan down
        return;
      case 0xDF0: // B - Green
        sendAirConditionerIR(0x807F); // Fan up
        return;
      case 0x3F0: // C - Yellow
        // Nothing yet...
        break;
      case 0xBF0: // D - Orange
        sendAirConditionerIR(0xF00F); // Fan auto
        return;

      // Non-matching IR
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

void sendAirConditionerIR(unsigned long value)
{
  irsend.sendNEC(value, 16);
  irrecv.resume(); // Receive the next value
}
