#ifndef MS51_GLOBALS_H
#define MS51_GLOBALS_H

#include "MS51_16K.H"
#include "lcd.h"
#include "spi.h"
#include "max6675.h"

// CountDown Timer
#define ONHRS 0
#define ONMIN 15
#define ONSEC 0

// Retry interval
#define LCD_NRL_DPY_INT 10*1000 // 10 Sec

// Timer 1
#define DIV12_1ms TIMER_DIV12_VALUE_1ms_FOSC_16000000+26

// Soft Delay 
#define osc_freq_in_MHz 16
#define osc_scalar (24 / osc_freq_in_MHz)

// LCD
#define RS_PIN  P04
#define EN_PIN  P03
#define DB4_PIN P02
#define DB5_PIN P12
#define DB6_PIN P13
#define DB7_PIN P14

// LED
#define LED_RED P06
#define LED_GRN P07

// Buttons
#define SW_SART P05
#define SW_STOP P17
#define SW_CNFG P16

// Relay
#define RLY_PIN P30

// Thermocouple K-Type
#define MAX6675_SO P01
#define MAX6675_CK P10
#define MAX6675_CS P11

// PinMode
#define GPIO_init() do{\
    P04_PUSHPULL_MODE; \
    P03_PUSHPULL_MODE; \
    P02_PUSHPULL_MODE; \
    P12_PUSHPULL_MODE; \
    P13_PUSHPULL_MODE; \
    P14_PUSHPULL_MODE; \
    P06_PUSHPULL_MODE; \
    P07_PUSHPULL_MODE; \
    P05_INPUT_MODE; \
    P16_INPUT_MODE; \
    P17_QUASI_MODE; \
    P30_PUSHPULL_MODE; \
    } while(0);

#define MAX6675_GPIO_init() do{\
    P01_QUASI_MODE; \
    P10_QUASI_MODE; \
    P11_QUASI_MODE; \
    } while(0);

void delay_us(uint32_t value);
void delay_ms(uint32_t value);

void set_timer0_tl_th(uint16_t value);
void start_timer0_mode1(uint16_t value);
void stop_timer0_mode1(void);
void set_timer1_tl_th(uint16_t value);

uint8_t count_digits(uint32_t digt);
void float_to_str(float x, uint8_t decimalPoint, uint8_t *outStr);

void display_char(uint8_t x_pos, uint8_t y_pos, uint8_t value);
void display_uint(uint8_t x_pos, uint8_t y_pos, uint16_t value);

#endif
