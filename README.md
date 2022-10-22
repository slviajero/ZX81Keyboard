# ZX81Keyboard

This is a library to read a ZX81 keyboard with an Arduino.

ZX81 keyboard are 5 columns time 8 rows and can be scanned directly with 13 I/O pins.

The library uses the I/O pins only during keyboard access and keeps them on INPUT i.e. high resistance at any time except during read. The row pins are used for scanning, they change to OUTPUT during the read cycle one at a time. The column pins are kept on INPUT_PULLUP during scan time. 

The keyboard pins can be used for other peripherals when the keyboard is not scanned. All row pins can be used for reading outside the keyboard scan cycle without precautions. If they are used for writing they have to be set to the right mode before use. A keysan will set them to INPUT. Devices on the row pins have to be tolerant against an OUTPUT pulse caused by the keyboard scan. Devices on column pins have to be tolerant against the INPUT_PULLUP the library causes.

The following methods are implemented:

    void begin(byte* pins);
    bool available();
    int read();
    int peek();
    void getkey();

begin() needs an array of 13 pins as argument. The first 8 values are the row pins, the next 5 the columns pins.

available() returns 1 is a key is actually pressed.

peek() returns the ASCII value of the actually pressed key.

getkey() scans the keyboard and stored the raw key information in the variables state_key and state_shift.

read() is the main interface of the library. If no key is pressed, read returns immediately with a return value 0. If a key is pressed at the time read() is called, the functions waits indefinitely for a key release and returns the ASCII value.

There is currently no interrupt or timer mechanism implemented to read the keyboard asynchronously.


