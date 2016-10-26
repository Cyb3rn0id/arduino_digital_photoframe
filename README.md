# Arduino Digital Photoframe

This is the project I've used for the TV in the Rita's Dolls House.
Project is for an **Arduino UNO and Duemilanove** (both tested) with a shield 2.4 TFT LCD 240x320 pixels + microSD card reader. 

**Arduino Leonardo and other Arduino models are not suitable!**
(maybe using new libraries from Adafruit?)

Images MUST BE in BMP format, 320x240, mirrored from right to left. 
Each BMP image is 225Kb/228Kb. An old SD card of 128Mb will contain about 574 images.
Image filenames MUST BE in 8.3 format. My SD is formatted with FAT filesystem.

![image](https://github.com/Cyb3rn0id/arduino_digital_photoframe/blob/master/project_pic.jpg)

* [Link to a compatible display I've purchased and successfully tested](http://www.ebay.it/itm/2-4-TFT-LCD-Visualizzazione-panel-SD-carta-240x320-Arduino-Elettronica-Shield-/331769583790?hash=item4d3f0218ae:g:48oAAOSwJb9Ws3rI)
* [Original firmware](http://apcmag.com/arduino.htm/)
* [Original Project description](http://apcmag.com/arduino-project-make-digital-photo-frame.htm/)

**Warning**: display I've linked has no Touch Screen, some examples in the directory requires touch screen. For a simple Photoframe, touch screen is not needed
