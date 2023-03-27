#ifndef SPI_MAX6675_H
#define SPI_MAX6675_H
#include "global.h"
/*
#define T_min             			0
#define T_max              			1024

#define count_max          			4096

#define no_of_pulses       			16

#define deg_C              			0
#define deg_F              			1
#define tmp_K              			2

#define open_contact       			0x04
#define close_contact      			0x00

#define scalar_deg_C       			0.25
#define scalar_deg_F_1     			1.8
#define scalar_deg_F_2     			32.0
#define scalar_tmp_K       			273.0

#define no_of_samples      			16
*/
void Max6675_SPI_init(void);
float Max6675_readCelsius(void);
float Max6675_readFahrenheit(void);

#endif
