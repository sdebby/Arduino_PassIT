# PassIT
Arduino programed USB HID strikes.

This arduino program will get the text from the mypass.txt file in the SD Card.
Encrypt text if it is not encrypted, or decrypt it, using buildin key.
On encryption led blink 3 times.

On a button push:
Led goes off.
Strike the decrypted text as keyboard HID.
Hit Enter at the end.

-------------------------------------------
Using Arduino uno R3.
and SD Card breakout :
https://www.sparkfun.com/products/12941

DavyLandman Arduino AES encryption labrary:
https://github.com/DavyLandman/AESLib

Update arduino uno firmware to enable USB HID using this manual:
https://www.arduino.cc/en/Hacking/DFUProgramming8U2

Flashig Arduino with coopermaa USB HID flash:
https://github.com/coopermaa/USBKeyboard/tree/master/firmware

Youtube:
https://youtu.be/eHl4A1D0S90

Limitations:
1. Text file containes only 1 line.
2. maximum text length will be 15 charecters long.
