#include "global.h"

data char allStringsDpy[][16] = {
    " Green to Start ", 
    "Pres Red to Stop" };

uint16_t ms = 0;
uint32_t millies = 0, preNrlDpy = 0;
int8_t sec = 0, min = 0, hrs = 0, lastSec = -1, dpy = 0;
uint8_t heaterState = 0, lastState = 0, toggle = 1, lastToggle = 0;

void set_timer0_tl_th(uint16_t value) {
  TL0 = LOBYTE(value);
  TH0 = HIBYTE(value);  
}

void set_timer1_tl_th(uint16_t value) {
  TL1 = LOBYTE(value);
  TH1 = HIBYTE(value);  
}

void timer_on(void) {
    ms = 0;
    hrs = ONHRS;
    min = ONMIN;
    sec = ONSEC;
    toggle = 0;
    set_timer1_tl_th(DIV12_1ms);
    ENABLE_TIMER1_INTERRUPT;
    set_TCON_TR1; //Timer 1 run
}

void timer_off(void) {
    clr_TCON_TR1; //Timer 1 stop
    DISABLE_TIMER1_INTERRUPT;
}

void timer_count_down(void) {
    ms++;
    if(ms == 499) {
      toggle = ~toggle;
    } else if (ms > 999) {
        ms = 0;
        sec--;
        if (sec == -1) {
            sec = 59;
            min--;
            if (min == -1) {
                min = 59;
                if(hrs != 0) {
                    hrs --;
                }
            }
        }
    }
}

void display_char(uint8_t x_pos, uint8_t y_pos, uint8_t value) {
    LiquidCrystal_setCursor(x_pos, y_pos);
    LiquidCrystal_putChar((value / 10) + 0x30);
    LiquidCrystal_setCursor((x_pos + 1), y_pos);
    LiquidCrystal_putChar((value % 10) + 0x30);
}

void display_on_state(void){
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(0, 0);
    LiquidCrystal_putStr("Switching On");
    delay_ms(250);
    LiquidCrystal_clear();
}

void display_off_state(void){
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(0, 0);
    LiquidCrystal_putStr("Switching Off");
    delay_ms(250);
    LiquidCrystal_clear();
}

void display_execution_state(void) {
    if (sec != lastSec) {
        LiquidCrystal_setCursor(0, 0);
        LiquidCrystal_putStr("Time ->");
        display_char(8, 0, hrs);
        display_char(11, 0, min);
        display_char(14, 0, sec);
        
        LiquidCrystal_setCursor(0, 1);
        LiquidCrystal_putStr("Temp ->");
        LiquidCrystal_setCursor(8,1);
        LiquidCrystal_putStr("30.06");
        LiquidCrystal_setCursor(14, 1);
        LiquidCrystal_putChar(0xDF);
        LiquidCrystal_putChar('C');
        lastSec = sec;
    }

    if (toggle != lastToggle) {
        if(toggle) {
            LiquidCrystal_setCursor(10, 0);
            LiquidCrystal_putChar(':');
            LiquidCrystal_setCursor(13, 0);
            LiquidCrystal_putChar(':');
        } else {
            LiquidCrystal_setCursor(10, 0);
            LiquidCrystal_putChar(' ');
            LiquidCrystal_setCursor(13, 0);
            LiquidCrystal_putChar(' ');
        }
        lastToggle = toggle;
    }
}

void display_normal_state(uint8_t strIdx) {
        LiquidCrystal_clear();
        LiquidCrystal_setCursor(1, 0);
        LiquidCrystal_putStr("Drop Used Pads");
        LiquidCrystal_setCursor(0, 1);
        LiquidCrystal_putStr(allStringsDpy[strIdx]);
}

void heater_on(void) {
    RLY_PIN = LOW;
    LED_GRN = LOW;
    LED_RED = HIGH;
    heaterState = HIGH;
}

void heater_off(void) {
    RLY_PIN = HIGH;
    LED_GRN = HIGH;
    LED_RED = LOW;
    heaterState = LOW;
}

void heater_auto_cutoff(void) {
    if ((sec == 0) && (min == 0) && (hrs == 0)) {
        heater_off();
        display_off_state();
        timer_off();
        lastState = LOW;
    } else {
        display_execution_state();
    }
}

void Timer0_ISR (void) interrupt 1 {
    _push_(SFRS);
    millies++;
    set_timer0_tl_th(DIV12_1ms);
    clr_TCON_TF0;
    _pop_(SFRS);
}

void INT1_ISR (void) interrupt 2 {
	_push_(SFRS);
    heater_off();
	clr_TCON_IE1;
	_pop_(SFRS);
}

void Timer1_ISR (void) interrupt 3 {
    _push_(SFRS);
    timer_count_down();
    set_timer1_tl_th(DIV12_1ms);
    clr_TCON_TF1;
    _pop_(SFRS);
}

void setup(void) {
	GPIO_init();
	
	// Ext Int 1
	set_P17;
	INT1_LOW_LEVEL_TRIG;
	set_IE_EX1;
    SET_INT_INT1_LEVEL2;
    
    // Timer 0
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS_DIV12; // Timer Clock = Fsys/12
    SET_INT_TIMER0_LEVEL3;
    set_timer0_tl_th(DIV12_1ms);
    ENABLE_TIMER0_INTERRUPT;
    set_TCON_TR0; //Timer 0 run

    // Timer 1
    ENABLE_TIMER1_MODE1;
    TIMER1_FSYS_DIV12; // Timer Clock = Fsys/12

	// Heater
    heater_off();

    // LCD
    LiquidCrystal_init(16, 2, 0);
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(5, 0);
    LiquidCrystal_putStr("WELCOME");
    LiquidCrystal_setCursor(3, 1);
    
    LiquidCrystal_putStr("Sandi Micro");
    delay_ms(500);
    display_normal_state(0);
    
    ENABLE_GLOBAL_INTERRUPT;
}

void main(void) {	
	setup();

	while(1) {
    if ((heaterState == LOW) && (SW_SART == LOW)) {
        heater_on();
        display_on_state();
        timer_on();
        lastState = HIGH;
        heaterState = HIGH;
    } else if ((heaterState == LOW) && (lastState == HIGH)) {
        display_off_state();
        timer_off();
        lastState = LOW;
    } else if (heaterState == HIGH) {
        heater_auto_cutoff();
    } else {
        if ((millies - preNrlDpy) >= LCD_NRL_DPY_INT) {
            display_normal_state(dpy);
            (dpy < (sizeof(allStringsDpy[0])/16)) ? dpy++ : (dpy = 0);
            preNrlDpy = millies;
        }
    }
    }
}
