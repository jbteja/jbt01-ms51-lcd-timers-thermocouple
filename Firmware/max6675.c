#include "MAX6675.h"

/********** Define SPI CS pin **********/
/*
 * #define MAX6675_CS P11
 *
 * #define MAX6675_GPIO_init() do{\
 *  P01_QUASI_MODE; \
 *  P10_QUASI_MODE; \
 *  P11_QUASI_MODE; \
 *  } while(0)
*/

void Max6675_SPI_init(void) {
    set_SPCR_SPR1;
    clr_SPCR_SPR0;
	
	/* /SS General purpose I/O ( No Mode Fault ) */
    set_SPSR_DISMODF;
    clr_SPCR_SSOE;

    /* SPI in Master mode */
    set_SPCR_MSTR;

    /* MSB first */
    clr_SPCR_LSBFE;

    clr_SPCR_CPOL;
    set_SPCR_CPHA;
    
    /* Enable SPI function */
    set_SPCR_SPIEN;
}

/*
void Max6675_SPI_HwInit(uint8_t clk_value) {
	switch(clk_value) {
		case 1: {
			clr_SPR1;
			set_SPR0;
			break;
		}
		case 2: {
			set_SPR1;
			clr_SPR0;
			break;
		}
		case 3: {
			set_SPR1;
			set_SPR0;
			break;
		}
		default: {
			clr_SPR1;
			clr_SPR0;
			break;
		}
	}
	
	set_DISMODF;
	set_MSTR;
	clr_CPOL;
	set_CPHA;
	set_SPIEN;
}
*/
/*
uint8_t Max6675_getADC(uint16_t *ADC_data) {
	uint16_t temp_data;

	unsigned long avg_value = 0;
	unsigned char samples = no_of_samples;
	
	while(samples > 0) {
	MAX6675_CS = 0;
	delay_us(10);

	temp_data = Spi_Read_Byte(0x00);
	temp_data <<= 8;
	temp_data |= Spi_Read_Byte(0x00);
	//temp_data &= 0x7FFF;

	MAX6675_CS = 1;

	avg_value += (unsigned long)temp_data;
		 
	samples--;
	delay_ms(5);
	}

	temp_data = (avg_value >> 4);
			 

	if((temp_data & 0x04) == close_contact) {
		*ADC_data = (temp_data >> 3);
		return close_contact;
	} else {
		*ADC_data = (count_max + 1);
		return open_contact;
	}
}
*/
float Max6675_readCelsius(void) {
	uint16_t tvalue;
	
	MAX6675_CS = 0;
	delay_us(10);

	tvalue = Spi_Read_Byte(0x00);
	tvalue <<= 8;
	tvalue |= Spi_Read_Byte(0x00);
	tvalue &= 0x7FFF;

	MAX6675_CS = 1;
	
	if (tvalue & 0x04) {
		return 0.00; // Failed
	}
	
	tvalue >>= 3;
	display_uint(0, 0, tvalue);
	return ((float)tvalue) * 0.25;
}

float Max6675_readFahrenheit(void) { 
	return (Max6675_readCelsius() * 9.0 / 5.0 + 32); 
}

/*
uint8_t Max6675_getADC(uint16_t *ADC_data) {
	unsigned char lb = 0;
	unsigned char hb = 0;
	unsigned char samples = no_of_samples;
	unsigned int temp_data = 0;
	unsigned long avg_value = 0;

	while(samples > 0) {
		 MAX6675_CS = 0;
			
		 SPDR = 0x00;
		 while(!(SPSR & SET_BIT7));  
		 hb = SPDR;
		 clr_SPSR_SPIF;

		 SPDR = 0x00;
		 while(!(SPSR & SET_BIT7));          
		 lb = SPDR;
		 clr_SPSR_SPIF;

		 MAX6675_CS = 1;
		 
		 temp_data = hb;
		 temp_data <<= 8;
		 temp_data |= lb;
		 temp_data &= 0x7FFF;
		 
		 avg_value += (unsigned long)temp_data;
		 
		 samples--;
		 delay_ms(5);
	}

	temp_data = (avg_value >> 4);

	if((temp_data & 0x04) == close_contact) {
		*ADC_data = (temp_data >> 3);
		return close_contact;
	} else {
		*ADC_data = (count_max + 1);
		return open_contact;
	}
}

float Max6675_getT(uint16_t ADC_value, uint8_t T_unit) {
	float tmp = 0.0;
	tmp = (((float)ADC_value) * scalar_deg_C);

	switch(T_unit) {
		case deg_F: {
			 tmp *= scalar_deg_F_1;
			 tmp += scalar_deg_F_2;
			 break;
		}
		case tmp_K: {
			tmp += scalar_tmp_K;
			break;
		}
		default: {
			break;
		}
	}
	return tmp;
}
*/