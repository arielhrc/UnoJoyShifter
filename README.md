[Leia esta página em português](https://github.com/arielhrc/UnoJoyShifter/blob/master/README-pt.md)

G27 shifter USB adapter using an Arduino UNO R3

Prerequisite: Install [Flip](https://www.microchip.com/en-us/development-tool/flip) and Arduino IDE (duh?)
1. Connect your Arduino UNO and upload the code UnoShifter.ino
2. Then, put your Arduino in DFU mode. Short the two pins near the USB port.
3. Now, runs TurnIntoAJoystick.bat, inside UnoJoy folder. 
4. When the process is over, unplug and replug your board, it will be detected as UnoJoy Joystick. (check on control panel)

WIRING
--------------------


|DB9 | Color      |Shifter |Description          |Arduino      |
|----|------------|--------|---------------------|-------------|
|  1 | Purple     | 1      | Clock               |pin  2       |
|  2 | Grey       | 7      | Data                |pin  3       |
|  3 | Yellow     | 5      | Button !CS & !PL    |pin  4       |
|  4 | Orange     | 3      | Shifter X axis      |pin  A0      |
|  5 | White      | 2      | SPI input           |not connected|
|  6 | Black      | 8      | GND                 |GND          |
|  7 | Red        | 6      | +5V                 |VCC          |
|  8 | Green      | 4      | Shifter Y axis      |pin  A1      |
|  9 | Red        | 9      | +5V                 |VCC          |



Original Code: https://github.com/jssting/UnoJoyShifter
If you want to use your arduino normally again, just puts it in DFU again and runs TurnIntoAnArduino.bat
Eddited by Ariel Carvalho

