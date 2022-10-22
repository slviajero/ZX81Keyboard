/* 
 *  ZX81 Keyboard for Arduino 
 *  Stefan Lenz, 2022
 *  
 *  based on 
 *    ZX81 USB Keyboard for Leonardo
 *    (c) Dave Curran
 *    2013-04-27
 *
 *    Modified with Function keys by Tony Smith
 *    2014-02-15
 * 
 * for the originals see 
 * https://blog.smittytone.net/2014/02/16/the-sinclair-zx81-a-raspberry-pi-retro-restyle-part-1/
 * https://github.com/smittytone/ZX81Alt
 * 
 * Code used: keymaps
 * 
 * This version uses a PSKeyboard interface function from the PS2Keyboard Arduino library. Original authors are
 *
 * Christian Weichel <info@32leaves.net>
 * Paul Stoffregen <paul@pjrc.com> 2010, 2011
 * Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com>  
 * 
 *  Code used: class definition of PS2Keyboard
 *
 * All other code rewritten from scratch by Stefan Lenz, 2022
 * 
 * 
 *
 */

#define ZX81KEYBOARD_H
#include "Arduino.h"

/*
 * 8*5 keyboard, can be extended later to other geometries 
 */

#define ZX81ROWS 8
#define ZX81COLS 5

/* where is the shift key */
#define ZX81_SHIFTROW 5
#define ZX81_SHIFTCOL 4

/* how much time for debounce */
#define ZX81_DEBOUNCE_TIME 10

/* 
 * Special keys mapped:
 *  Return is 10
 *  Backspace 8 (could also be 127)
 *  Arrows 28-31
 *  ZX81 specials 17 and 18 
 */

#define ZX81KEY_RETURN 10
#define ZX81KEY_LEFT_ARROW 28
#define ZX81KEY_ESC 27
#define ZX81KEY_DOWN_ARROW 29
#define ZX81KEY_UP_ARROW 30
#define ZX81KEY_RIGHT_ARROW 31
#define ZX81KEY_F6 18
#define ZX81KEY_F5 17
#define ZX81KEY_BACKSPACE 8

/* the scan code of the newline key */
#define ZX81SCAN_NEWLINE 34

/* 
 * the keymap from https://github.com/smittytone/ZX81Alt
 */

const byte keyMap[] PROGMEM = {
  '5', '4', '3', '2', '1', 
  't', 'r', 'e', 'w', 'q', 
  '6', '7', '8', '9', '0', 
  'g', 'f', 'd', 's', 'a', 
  'y', 'u', 'i', 'o', 'p',
  'v', 'c', 'x', 'z', 0, /* the shift key is mapped to zero */
  'h', 'j', 'k', 'l', ZX81KEY_RETURN,
  'b', 'n', 'm', '.', ' '
};

/* Keymap if Shift is pressed */
const byte keyMapShifted[] PROGMEM = {
  ZX81KEY_LEFT_ARROW, '$', '\\', '@', ZX81KEY_ESC, 
  'T', 'R', 'E', 'W', 'Q', 
  ZX81KEY_DOWN_ARROW, ZX81KEY_UP_ARROW, ZX81KEY_RIGHT_ARROW, ZX81KEY_F6, ZX81KEY_BACKSPACE, 
  'G', 'F', 'D', 'S', 'A', 
  'Y', 'U', 'I', 'O', 'P',
  'V', 'C', 'X', 'Z', 0, 
  'H', 'J', 'K', 'L', ZX81KEY_F5,
  'B', 'N', 'M', ',', ' '
};

/* keymap if Shift is pressed plus New Line
 *Next* key read should be from this table */
const byte keyMapAlt[] PROGMEM = {
  ZX81KEY_LEFT_ARROW , '}' , '{' , ']' , '[', 
  '=' , '<' , 0 , '~' , 64, 
  ZX81KEY_DOWN_ARROW, ZX81KEY_UP_ARROW, ZX81KEY_RIGHT_ARROW, ZX81KEY_F6, ZX81KEY_BACKSPACE, 
  0 , 0 , 0 , '|' , 156, 
  '>', '$', '(', ')', '\"',
  '/', '?', ';', ':', 0, 
  '_', '-', '+', '=', ZX81KEY_RETURN,
  '*', '<', '>', '\'', '#'
};

/**
 * Class definition like in PS2Keyboard, taken from there, originally 
 * invented by Christian Weichel
 */
class ZX81Keyboard {
  public:
    ZX81Keyboard();
/*
 * 8 row pins followed by 5 column pins in one array as an argument
 */
    void begin(byte* pins);
    
/*
 * Returns true if there is a char to be read, false if not.
*/
    bool available();
    
/*
 * Returns the char last read from the keyboard.
 * If there is no char available, 0 is returned.
 */
    int read();

/*
 * peek() returns the next character 
 * without removing it, else 0. SL 2022.
 */
    int peek();

/* probably better private but easier to test that way*/
    char lastKey = 0;
    uint8_t alt = 0;
    uint8_t state_shift = 0;
    uint8_t state_key = 255;

/* the raw keymatrix */ 
    void getkey();

  private:
    uint8_t row[ZX81ROWS];
    uint8_t col[ZX81COLS];
    void scan();
};
