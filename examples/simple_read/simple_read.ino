/*
 * Test program for ZX81Keyboard
 * Stefan Lenz, 2022
 * GPL v3.0
 */
 
#include <ZX81Keyboard.h>

ZX81Keyboard keyboard;

void setup() {
  /* start Serial */
  Serial.begin(9600);
  
  /* first the 8 rows, then the 5 columns or the keyboard */
  const byte pins[] = {7, 8, 9, 10, 11, 12, 13, A0, 2, 3, 4, 5, 6 };
  keyboard.begin(pins);
}

void loop() {
  keyboard.getkey();
  
  /* scan the keyboard */
  if (keyboard.state_key != 255) {
    Serial.print("Keycode found: "); 
    Serial.println(keyboard.state_key);
    Serial.print("Shift state: ");
    Serial.println(keyboard.state_shift);
    Serial.print("ASCII value: "); 
    Serial.println(keyboard.peek());
    Serial.print("Alt state: ");
    Serial.println(keyboard.alt);
  }
  
  /* now read it */
  char ch=keyboard.read();
  if (ch != 0) {
    Serial.print("Key: ");
    if (ch > 31) Serial.write(ch); 
    Serial.println();
  }
  delay(100);
}
