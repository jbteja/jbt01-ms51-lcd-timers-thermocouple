#include "MS51_16K.H"
#include "delay_soft.h"

void delay_us(unsigned int value) {
  value = (value / osc_scalar);
  
  while(value > 0) {
    _nop_();
		value--;
  }
}

void delay_ms(unsigned int value) { 
  while(value > 0) {
    --value;
    delay_us(1000);
  }
}
