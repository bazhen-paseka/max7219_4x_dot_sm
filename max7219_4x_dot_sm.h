#ifndef MAX7219_4X_DOT_SM_H_INCLUDED
#define MAX7219_4X_DOT_SM_H_INCLUDED

typedef struct
{
	SPI_HandleTypeDef	*spi;
	GPIO_TypeDef 		*cs_port;
    uint16_t 			 cs_pin;
    uint8_t				data[8];
} max7219_struct;


void max7219_init(max7219_struct *max7219_handler);

#endif 	//	MAX7219_4X_DOT_SM_H_INCLUDED

