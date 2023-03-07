#include "lcd.h"

/************ Define MS51 pins for display **********/
/*
 * #define RS_PIN  P04
 * #define EN_PIN  P03
 * #define DB4_PIN P02
 * #define DB5_PIN P12
 * #define DB6_PIN P13
 * #define DB7_PIN P14
*/

uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;

uint8_t _numlines;
uint8_t _row_offsets[4];

/************ Low level data pushing commands **********/
void pulseEnable(void) {
  EN_PIN = 0x01;
  delay_us(10);
  EN_PIN = 0x00;
  delay_us(100);
}

void write4bits(uint8_t value) {
  DB4_PIN = (value & 0x01);
  DB5_PIN = ((value & 0x02) >> 1);
  DB6_PIN = ((value & 0x04) >> 2);
  DB7_PIN = ((value & 0x08) >> 3);
	
  pulseEnable();
}

// Write either command or data with selection
void send(uint8_t value, uint8_t mode) {
  RS_PIN = mode;
  write4bits(value>>4);
  write4bits(value);
}

/*********** Mid level commands, for sending data/cmds */
void command(uint8_t value) {
  send(value, 0x00);
}

void write(uint8_t value) {
  send(value, 0x01);
}

void setRowOffsets(int row0, int row1, int row2, int row3) {
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** High level commands, for the user! */
void LiquidCrystal_clear(void) {
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delay_ms(2); // this command takes a long time!
}

void LiquidCrystal_home(void) {
  command(LCD_RETURNHOME);  // set cursor position to zero
  delay_ms(2); // this command takes a long time!
}

void LiquidCrystal_putStr(uint8_t *strs) {
  do {
    write(*strs++);
	} while(*strs != '\0');
}

void LiquidCrystal_putChar(uint8_t charc) {
  write(charc);
}

void LiquidCrystal_setCursor(uint8_t col, uint8_t row) {
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_noDisplay(void) {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_display(void) {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_noCursor(void) {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_cursor(void) {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_noBlink(void) {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_blink(void) {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_createChar(uint8_t location, uint8_t charmap[]) {
  int i;
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (i=0; i<8; i++) {
    write(charmap[i]);
  }
}

void LiquidCrystal_init(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	delay_ms(20);

  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

	delay_ms(40);// wait min 40ms
  RS_PIN = 0x00;
  EN_PIN = 0x00;

  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
		delay_ms(5); // wait min 4.1ms

    // second try
    write4bits(0x03);
		delay_ms(5); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
		delay_us(150);

    // finally, set to 4-bit interface
    write4bits(0x02);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
		delay_ms(5); // wait min 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
		delay_us(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  LiquidCrystal_display();

  // clear it off
  LiquidCrystal_clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
}
