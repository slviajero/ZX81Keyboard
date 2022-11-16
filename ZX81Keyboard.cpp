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
 */

#ifndef ZX81KEYBOARD_H
#include "ZX81Keyboard.h"

ZX81Keyboard::ZX81Keyboard() {}

/*
 * ZX81ROW+ZX81COL pins have to be supplied
 * for a standard ZX81 keyboard it is 8 rows and 5 columns 
 * i.e. 13 pins 
 * 
 * no state change of pinModes on construction, we do all 
 * pinMode stuff on the fly and reset them all to INPUT after use
 *
 */
void ZX81Keyboard::begin(const byte * p) {
    for(int i=0; i<ZX81ROWS; i++) row[i]=p[i];
    for(int i=0; i<ZX81COLS; i++) col[i]=p[i+ZX81ROWS];
}
    
/*
 * Returns true if there is a char to be read, false if not.
 */
bool ZX81Keyboard::available() {

/* has there been a previous peek and a key is available */
/* or is there a key pressed right now?*/
    if (lastKey) return 1; else return (peek() != 0);
}
 
/*
 * Returns the char last read from the keyboard.
 * this only returns on key release i.e is blocking while key pressed.
 * If there is no char available, 0 is returned.
 */
int ZX81Keyboard::read() {

    char ch;

/* do we have a key */
    if (lastKey) ch=lastKey; else ch=peek();

/* if a key is pressed wait for it to be released */
    if (ch) {
        while (peek()) delay(0);
        return ch;
    } else return 0;
}

/*
 * peek() returns the state of the keyboard as a pressed (!) key 
 * or 0 if there is none, peek is non blocking, SL 2022.
 */
int ZX81Keyboard::peek() {

/* get a debounced state of the keyboard */
    getkey();

/* a key pressed */
    if (state_key != 255) {
/* go to alt mode */
        if (state_shift && state_key == ZX81SCAN_NEWLINE) {
            alt=1; 
            return lastKey=0;
        }
/* no shift and no alt */
        if (!state_shift && !alt) {
#ifdef USEPROGMEM
            return lastKey=pgm_read_byte(keyMap + state_key%40);
#else 
            return lastKey=keyMap[state_key];
#endif
       } 
/* shift and no alt */
        if (state_shift && !alt) {
#ifdef USEPROGMEM
            return lastKey=pgm_read_byte(keyMapShifted + state_key%40);
#else
            lastKey=keyMapShifted[state_key];
#endif
        }
/* alt keys */
        if (alt) { 
            alt=0; 
#ifdef USEPROGMEM
        return lastKey=pgm_read_byte(keyMapAlt + state_key%40); 
#else
        return lastKey=keyMapAlt[state_key]; 
#endif
        }
    } else return lastKey=0;
}


/* read the undebounced state of the keyboard and store it in state_shift and state_key */
void ZX81Keyboard::scan() {

/* reset the states */
    state_key=255; 
    state_shift=0;

/* set the pins values, 5 column pin to pullup and the rest to INPUT
 * as in the original ZX81
 */
    for(int i=0; i<ZX81COLS; i++) pinMode(col[i], INPUT_PULLUP);
    for(int i=0; i<ZX81ROWS; i++) pinMode(row[i], INPUT);

/* scan all shift, store the index of the first key found and the shift state*/
    for (int i=0; i<ZX81ROWS; i++) {
        pinMode(row[i], OUTPUT);
        digitalWrite(row[i], LOW);
        // delay(1); 
        for (int j=0; j<ZX81COLS; j++) {
            if (!digitalRead(col[j])) {
                if (i == ZX81_SHIFTROW && j == ZX81_SHIFTCOL) state_shift=1; else state_key=j+ZX81COLS*i;
            }
        }
        pinMode(row[i], INPUT);
    } 
}

/* debounce the keyboard input and return if there is a stable state */
void ZX81Keyboard::getkey() {

/* scan once */
    scan();

/* if no key is detected, just return even if the state may be unstable */	
    if (state_key == 255 && state_shift == 0) return;

/* remember when we started */
    long startDebounce=millis();

/* remember the state */
    long debounceTime=millis();
    uint8_t prev_state_key=state_key;
    uint8_t prev_state_shift=state_shift;

/* see if the state remains stable */
    while(millis()-debounceTime < ZX81_DEBOUNCE_TIME) {
        scan();
        if (state_key != prev_state_key || state_shift != prev_state_shift) {
            debounceTime=millis();
            prev_state_key=state_key;
            prev_state_shift=state_shift;
        }
/* the longest we wait is DEBOUNCE_THRESHOLD, we give up after that */
        if (millis()-startDebounce > ZX81_DEBOUNCE_THRESHOLD) {
            state_key=255;
            state_shift=0;
        }
    } 
}
#endif
