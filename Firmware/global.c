#include "global.h"

/* Soft delay in micro seconds */
void delay_us(uint32_t value) {
    value = (value / osc_scalar);
    
    while(value > 0) {
        _nop_();
        value--;
    }
}

/* Soft delay in milli seconds */
void delay_ms(uint32_t value) {
    while(value > 0) {
        --value;
        delay_us(1000);
    }
}

/* Timer 0 & 1 Mode 1 functions */
void set_timer0_tl_th(uint16_t value) {
    TL0 = LOBYTE(value);
    TH0 = HIBYTE(value);
}

void start_timer0_mode1(uint16_t value) {
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS_DIV12; //Clock = Fsys/12
    set_timer0_tl_th(value);
    ENABLE_TIMER0_INTERRUPT;
    set_TCON_TR0; //Timer 0 Run  
}

void stop_timer0_mode1(void) {
    clr_TCON_TR0; //Timer 0 Stop
    DISABLE_TIMER0_INTERRUPT;
}

void set_timer1_tl_th(uint16_t value) {
    TL1 = LOBYTE(value);
    TH1 = HIBYTE(value);
}

/* Liquid crystal display helper functions */
void display_char(uint8_t x_pos, uint8_t y_pos, uint8_t value) {
    LiquidCrystal_setCursor(x_pos, y_pos);
    LiquidCrystal_putChar((value / 10) + 0x30);
    LiquidCrystal_setCursor((x_pos + 1), y_pos);
    LiquidCrystal_putChar((value % 10) + 0x30);
}