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

/* To convert int, float to string for display */
uint8_t count_digits(uint32_t digt) {
    uint8_t count = 0;
    while(digt != 0) {
        digt = digt/10;
        count++;
    }
    return count;
}

void float_to_str(float x, uint8_t decimalPoint, uint8_t *outStr) {
    uint32_t units, decimals, i = 0, mul = 1;
    while(i++ < decimalPoint) {
        mul *= 10;
    }
    decimals = (int)(x * mul) % mul;
    units = (int)x;
    i = count_digits(units) + decimalPoint + 1;

    memset(outStr, '0', i);
    outStr[i--] = '\0';
    if (decimalPoint > 0) {
        while(decimalPoint > 0) {
            outStr[i--] = (decimals % 10) + '0';
            decimals /= 10;
            decimalPoint--;
        }
        outStr[i--] = '.';
    }
    
    while (units > 0) {
        outStr[i--] = (units % 10) + '0';
        units /= 10;
    }
}

/* Liquid crystal display helper functions */
void display_char(uint8_t x_pos, uint8_t y_pos, uint8_t value) {
    LiquidCrystal_setCursor(x_pos, y_pos);
    LiquidCrystal_putChar((value / 10) + 0x30);
    LiquidCrystal_setCursor((x_pos + 1), y_pos);
    LiquidCrystal_putChar((value % 10) + 0x30);
}

void display_uint(uint8_t x_pos, uint8_t y_pos, uint16_t value) {
    uint8_t ch[6] = {'\0'};
    
    if(value > 9999) {
        ch[0] = ((value / 10000) + 0x30);
        ch[1] = (((value % 10000)/ 1000) + 0x30);
        ch[2] = (((value % 1000) / 100) + 0x30);
        ch[3] = (((value % 100) / 10) + 0x30);
        ch[4] = ((value % 10) + 0x30);
        ch[5] = '\0';
    
    } else if((value > 999) && (value <= 9999)) {
        ch[0] = (((value % 10000)/ 1000) + 0x30);
        ch[1] = (((value % 1000) / 100) + 0x30);
        ch[2] = (((value % 100) / 10) + 0x30);
        ch[3] = ((value % 10) + 0x30);
        ch[4] = '\0';
        ch[5] = '\0';
    
    } else if((value > 99) && (value <= 999)) {
        ch[0] = (((value % 1000) / 100) + 0x30);
        ch[1] = (((value % 100) / 10) + 0x30);
        ch[2] = ((value % 10) + 0x30);
        ch[3] = '\0';
        ch[4] = '\0';
        ch[5] = '\0';
    
    } else if((value > 9) && (value <= 99)) {
        ch[0] = (((value % 100) / 10) + 0x30);
        ch[1] = ((value % 10) + 0x30);
        ch[2] = '\0';
        ch[3] = '\0';
        ch[4] = '\0';
        ch[5] = '\0';

    } else {
        ch[0] = ((value % 10) + 0x30);
        ch[1] = '\0';
        ch[2] = '\0';
        ch[3] = '\0';
        ch[4] = '\0';
        ch[5] = '\0';
    }
    LiquidCrystal_setCursor(x_pos, y_pos);
    LiquidCrystal_putStr(ch);
}
