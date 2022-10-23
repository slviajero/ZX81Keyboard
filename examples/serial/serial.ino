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
  /* a possible MEGA pinout */
  const byte megapins[] = {37, 35, 33, 31, 29, 27, 25, 23, 47, 45, 43, 41, 39};
  /* the UNO pinout */
  const byte unopins[] = {7, 8, 9, 10, 11, 12, 13, A0, 2, 3, 4, 5, 6 };
    
  keyboard.begin(megapins); 
}

void loop() {
  /* read and echo Serial*/
  while(!keyboard.available()) delay(0);
  if (char ch=keyboard.read()) Serial.write(ch);

  /* this yields, needed on some platforms */
  delay(0);
}
