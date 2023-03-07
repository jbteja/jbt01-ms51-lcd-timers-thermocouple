#ifndef DELAY_SOFT_H
#define DELAY_SOFT_H

#define osc_freq_in_MHz  16
#define osc_scalar       (24 / osc_freq_in_MHz) 

void delay_us(unsigned int value);
void delay_ms(unsigned int value);

#endif
