/*
 * Serial monitor for ZX81Keyboard
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
  /* read and echo Serial*/
  if (char ch=keyboard.read()) Serial.write(ch);

  /* this yields, needed on some platforms */
  delay(0);
}
