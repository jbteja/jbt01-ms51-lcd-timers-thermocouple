#include "global.h"

// Soft delay in micro sec
void delay_us(unsigned int value) {
  value = (value / osc_scalar);
	
  while(value > 0) {
    _nop_();
    value--;
  }
}

// Soft delay in milli sec
void delay_ms(unsigned int value) {
  while(value > 0) {
    --value;
    delay_us(1000);
  }
}
