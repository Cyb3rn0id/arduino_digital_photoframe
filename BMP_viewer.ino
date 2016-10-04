#define LCD_CS A3    
#define LCD_CD A2    
#define LCD_WR A1   
#define LCD_RD A0    
#define LCD_RESET A4
#define SD_CS 10 // Card select for shield use

#include "TFTLCD.h"
#include <SD.h>

#define BUFFPIXEL 20

File root;
File bmpImage;

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) 
  {
  tft.reset();
  tft.initDisplay();
  tft.setRotation(1);
  tft.fillScreen(0x0000); // fill screen with BLACK
  pinMode(10,OUTPUT);
  
  if (!SD.begin(SD_CS)) 
    {
    return;
    }
    root = SD.open("/");
  } // end setup

void loop(void) 
  {
  tft.fillScreen(0x0000); // fill screen with BLACK
  bmpDrawFromRoot();
  delay(3000);
  } end loop

void bmpDrawFromRoot() 
  {
  File     bmpFile;
  int      bmpWidth, bmpHeight;         // W+H in pixels
  uint8_t  bmpDepth;                    // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;              // Start of image data in file
  uint32_t rowSize;                     // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL];       // pixel in buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer);  // Current position in sdbuffer
  boolean  goodBmp = false;             // Set to true on valid header parse
  boolean  flip    = true;              // BMP is stored bottom-to-top
  int      w, h, row, col, tW, tH;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  boolean  first = true;
  
  tW = tft.width(); tH = tft.height();
  
  bmpFile = root.openNextFile();
  if (! bmpFile ) { root.rewindDirectory(); return; }  
  
  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) 
    { // BMP signature
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    // Read DIB header
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) 
      { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) 
        { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) 
          {
          bmpHeight = -bmpHeight;
          flip      = false;
          }

        // Crop area to be loaded
        int start = bmpHeight+((tH-bmpHeight)/2);
        for (row=0; row<bmpHeight; row++) 
            { // For each scanline...
            pos = bmpImageoffset + (row * rowSize);
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
            tft.goTo(0, start - row);

            for (col=0; col<bmpWidth; col++) 
              { // For each column...
              if (buffidx >= sizeof(sdbuffer)) 
                { // Indeed
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0; // Set index to beginning
                }
            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.writeData(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
      } // end goodBmp
    }
  }

  bmpFile.close();
}

uint16_t read16(File f) 
  {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
  }

uint32_t read32(File f) 
  {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
  }
