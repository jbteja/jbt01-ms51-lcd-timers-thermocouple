#include "MS51_16K.H"
#include "globals.h"

uint8_t heaterState = 0, lastState = 0;

void display_on_state(void){
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(0, 0);
    LiquidCrystal_putStr("Switching On");
    delay_ms(500);
}

void display_off_state(void){
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(0, 0);
    LiquidCrystal_putStr("Switching Off");
    delay_ms(1000);
}

void display_normal_state(void){
    LiquidCrystal_clear();
    LiquidCrystal_setCursor(0, 0);
    LiquidCrystal_putStr("Common text");
    LiquidCrystal_setCursor(0, 1);
    LiquidCrystal_putStr("blaa blaa blaa");
    delay_ms(100);
}

void heater_on(void) {
    RLY_PIN = LOW;
    LED_GRN = LOW;
    LED_RED = HIGH;
    heaterState = HIGH;
    delay_ms(5);
}

void heater_off(void) {
    RLY_PIN = HIGH;
    LED_GRN = HIGH;
    LED_RED = LOW;
    heaterState = LOW;
    delay_ms(5);
}

void INT1_ISR(void) interrupt 2 {
	_push_(SFRS);
    heater_off();
    //display_off_state();
	clr_TCON_IE1;
	_pop_(SFRS);
}

void setup(void) {
	GPIO_init();
	
	// Ext Int 1
	set_P17;
	INT1_LOW_LEVEL_TRIG;
	set_IE_EX1;
    ENABLE_GLOBAL_INTERRUPT;	

	// Heater
    heater_off();

    // LCD
    LiquidCrystal_init(16, 2, 0);
    LiquidCrystal_clear();

    LiquidCrystal_setCursor(1, 0);
    LiquidCrystal_putStr("== WELCOME ==");
    LiquidCrystal_setCursor(2, 1);
    LiquidCrystal_putStr("Sandy Micro");
    delay_ms(1000);

}

void main(void) {	
	setup();

	while(1) {
    if ((heaterState == LOW) && (SW_SART == LOW)) {
        display_on_state();
        heater_on();
        lastState = HIGH;
        heaterState = HIGH;
    } else if ((heaterState == LOW) && (lastState == HIGH)) {
        display_off_state();
        lastState = LOW;
    } else if (heaterState == HIGH) {
        //Timer
    } else {
        display_normal_state(); //TODO: need to add check
    }
  }
}
