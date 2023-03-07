#ifndef LiquidCrystal_H
#define LiquidCrystal_H
#include "MS51_16K.H"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void pulseEnable(void);
void write4bits(uint8_t value);
void send(uint8_t value, uint8_t mode);

void command(uint8_t value);
void write(uint8_t value);
void setRowOffsets(int row0, int row1, 
                   int row2, int row3);

void LiquidCrystal_clear(void);
void LiquidCrystal_home(void);
void LiquidCrystal_putStr(uint8_t *strs);
void LiquidCrystal_putChar(uint8_t charc);
void LiquidCrystal_setCursor(uint8_t col, uint8_t row);

void LiquidCrystal_noDisplay(void);
void LiquidCrystal_display(void);
void LiquidCrystal_noCursor(void);
void LiquidCrystal_cursor(void);
void LiquidCrystal_noBlink(void);
void LiquidCrystal_blink(void);
void LiquidCrystal_scrollDisplayLeft(void);
void LiquidCrystal_scrollDisplayRight(void);
void LiquidCrystal_leftToRight(void);
void LiquidCrystal_rightToLeft(void);
void LiquidCrystal_autoscroll(void);
void LiquidCrystal_noAutoscroll(void);
void LiquidCrystal_createChar(uint8_t location, 
                              uint8_t charmap[]);
void LiquidCrystal_init(uint8_t cols, uint8_t lines, 
                        uint8_t dotsize);

#endif
