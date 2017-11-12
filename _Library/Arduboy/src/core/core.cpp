#include "core.h"


const uint8_t PROGMEM lcdBootProgram_NEW[] = {
  0x0ae,				/* display off, sleep mode */
  0x0d5, 0x080,		/* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
  0x0a8, 0x03f,		/* */

  0x0d3, 0x000,		/*  */

  0x040,				/* start line */
  
  0x08d, 0x014,		/* [2] charge pump setting (p62): 0x014 enable, 0x010 disable */

  0x020, 0x000,		/* */
  0x0a1,				/* segment remap a0/a1*/
  0x0c8,				/* c0: scan dir normal, c8: reverse */
  0x0da, 0x012,		/* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
  0x081, 0x0cf,		/* [2] set contrast control */
  0x0d9, 0x0f1,		/* [2] pre-charge period 0x022/f1*/
  0x0db, 0x040,		/* vcomh deselect level */
  
  0x02e,				/* 2012-05-27: Deactivate scroll */ 
  0x0a4,				/* output ram to display */
  0x0a6,				/* none inverted normal display mode */
  0x0af,				/* display on */
};

const uint8_t PROGMEM lcdBootProgram[] = {
  // boot defaults are commented out but left here incase they
  // might prove useful for reference
  //
  // Further reading: https://www.adafruit.com/datasheets/SSD1306.pdf
  //
  // Display Off
  0xAE,     

  // Set Display Clock Divisor v = 0xF0
  // default is 0x80
  0xD5, 0xF0,

  // Set Multiplex Ratio v = 0x3F
  // 0xA8, 0x3F,   

  // Set Display Offset v = 0
  // 0xD3, 0x00, 

  // Set Start Line (0)
  // 0x40,     

  // Charge Pump Setting v = enable (0x14)
  // default is disabled
  0x8D, 0x14,   

  0x20,
  0x00,

  // Set Segment Re-map (A0) | (b0001)
  // default is (b0000)
  0xA1,     

  // Set COM Output Scan Direction
  0xC8,     

  // Set COM Pins v
  0xDA, 0x12,   

  // Set Contrast v = 0xCF
  0x81, 0xCF,   

  // Set Precharge = 0xF1
  0xD9, 0xF1,
    
  // Set VCom Detect
  0xDB, 0x40,   

  // Entire Display ON
  0xA4,     

  // Set normal/inverse display
  0xA6,  

  // Display On
  0xAF,     

  // set display mode = horizontal addressing mode (0x00)
  0x20, 0x00,  

  // set col address range 
  // 0x21, 0x00, COLUMN_ADDRESS_END,

  // set page address range
  // 0x22, 0x00, PAGE_ADDRESS_END
};

ArduboyCore::ArduboyCore() {}


void ArduboyCore::boot()
{
  #if F_CPU == 8000000L
  slowCPU();
  #endif

  // I2C Init
  Wire.begin();

  bootLCD();
  
  ideaBoard.begin();
  
  #ifdef SAFE_MODE
  if (buttonsState() == (LEFT_BUTTON | UP_BUTTON))
    safeMode();
  #endif

  saveMuchPower();
}

void ArduboyCore::wiretransfer(uint8_t c) {
    // I2C
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    Wire.beginTransmission(SSD1306_ADDR);
    Wire.write(control);
    Wire.write(c);
    Wire.endTransmission();
}

#if F_CPU == 8000000L
// if we're compiling for 8Mhz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz
void ArduboyCore::slowCPU()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  SREG = oldSREG;       // restore interrupts
}
#endif

void ArduboyCore::bootLCD()
{
  // run our customized boot-up command sequence against the
  // OLED to initialize it properly for Arduboy

  for (int8_t i=0; i < sizeof(lcdBootProgram); i++) {
    wiretransfer(pgm_read_byte(lcdBootProgram + i));
  }

}



void ArduboyCore::safeMode()
{
  blank(); // too avoid random gibberish
  while (true) {
    asm volatile("nop \n");
  }
}


/* Power Management */

void ArduboyCore::idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}

void ArduboyCore::saveMuchPower()
{
  power_adc_disable();
  power_usart0_disable();
  //pkj
  // power_twi_disable();
  // timer 0 is for millis()
  // timers 1 and 3 are for music and sounds
  power_timer2_disable();
  //power_usart0_disable();
  // we need USB, for now (to allow triggered reboots to reprogram)
  // power_usb_disable()
}

uint8_t ArduboyCore::width() { return WIDTH; }

uint8_t ArduboyCore::height() { return HEIGHT; }


/* Drawing */

void ArduboyCore::paint8Pixels(uint8_t pixels)
{
  wiretransfer(pixels);
}

void ArduboyCore::paintScreen(const unsigned char *image)
{
/*
   for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
  {
    wiretransfer(pgm_read_byte(image + i));
  }
*/
  paintScreen(image);
}


// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
void ArduboyCore::paintScreen(unsigned char image[])
{
	/*
  uint8_t c;
  int i = 0;

  SPDR = image[i++]; // set the first SPI data byte to get things started

  // the code to iterate the loop and get the next byte from the buffer is
  // executed while the previous byte is being sent out by the SPI controller
  while (i < (HEIGHT * WIDTH) / 8)
  {
    // get the next byte. It's put in a local variable so it can be sent as
    // as soon as possible after the sending of the previous byte has completed
    c = image[i++];

    while (!(SPSR & _BV(SPIF))) { } // wait for the previous byte to be sent

    // put the next byte in the SPI data register. The SPI controller will
    // clock it out while the loop continues and gets the next byte ready
    SPDR = c;
  }
  while (!(SPSR & _BV(SPIF))) { } // wait for the last byte to be sent
  */
  
  wiretransfer(SSD1306_COLUMNADDR);
  wiretransfer(0);   // Column start address (0 = reset)
  wiretransfer(WIDTH-1); // Column end address (127 = reset)

  wiretransfer(SSD1306_PAGEADDR);
  wiretransfer(0); // Page start address (0 = reset)
  wiretransfer(7); // Page end address

  {
    // save I2C bitrate
#ifdef TWBR
    uint8_t twbrbackup = TWBR;
    TWBR = 12; // upgrade to 400KHz!
#endif

    //Serial.println(TWBR, DEC);
    //Serial.println(TWSR & 0x3, DEC);

    // I2C
    for (uint16_t i=0; i<((HEIGHT * WIDTH) / 8); i++) {
      // send a bunch of data in one xmission
      Wire.beginTransmission(SSD1306_ADDR);
      Wire.write(0x40);
      for (uint8_t x=0; x<16; x++) {
        Wire.write(image[i]);
        i++;
      }
      i--;
      Wire.endTransmission();
    }
#ifdef TWBR
    TWBR = twbrbackup;
#endif
  }
}


void ArduboyCore::blank()
{
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
    wiretransfer(0x00);
}

void ArduboyCore::sendLCDCommand(uint8_t command)
{
  wiretransfer(command);
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void ArduboyCore::invert(boolean inverse)
{
  sendLCDCommand(inverse ? OLED_PIXELS_INVERTED : OLED_PIXELS_NORMAL);
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void ArduboyCore::allPixelsOn(boolean on)
{
  sendLCDCommand(on ? OLED_ALL_PIXELS_ON : OLED_PIXELS_FROM_RAM);
}

// flip the display vertically or set to normal
void ArduboyCore::flipVertical(boolean flipped)
{
  sendLCDCommand(flipped ? OLED_VERTICAL_FLIPPED : OLED_VERTICAL_NORMAL);
}

#define OLED_HORIZ_FLIPPED 0xA0 // reversed segment re-map
#define OLED_HORIZ_NORMAL 0xA1 // normal segment re-map

// flip the display horizontally or set to normal
void ArduboyCore::flipHorizontal(boolean flipped)
{
  sendLCDCommand(flipped ? OLED_HORIZ_FLIPPED : OLED_HORIZ_NORMAL);
}

/* RGB LED */

void ArduboyCore::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
  ideaBoard.setColorLED(red, green, blue);	
}

/* Buttons */

uint8_t ArduboyCore::getInput()
{
  return buttonsState();
}


uint8_t ArduboyCore::buttonsState()
{
  uint8_t buttons = 0;
  bitWrite(buttons,5,ideaBoard.getButtonRight());//right
  bitWrite(buttons,4,ideaBoard.getButtonLeft());//left
  bitWrite(buttons,7,ideaBoard.getButtonDown());//down
  bitWrite(buttons,6,ideaBoard.getButtonUp());//up
  bitWrite(buttons,2,ideaBoard.getButtonA());
  bitWrite(buttons,3,ideaBoard.getButtonB());

  return buttons;
}
