#ifndef __SSP_H
#define __SSP_H

#include "stm32f10x.h"

typedef struct{

	uint8_t Destination ;
	uint8_t Source ;
	uint8_t Type ;
	uint8_t Opt_header ;
	uint8_t Opt_header_byte ;
	uint8_t *Data ;
	uint8_t Data_length ;
	uint8_t *Ptr_Framed_data ;
	uint8_t G_Frame_length ;
	uint16_t Deframed_crc ;
	uint16_t crc_of_Deframed_data ;


}SSP_Init;

#define FEND  0xC0
#define FESC  0xDB
#define TFEND 0xDC
#define TFESC 0xDD

uint16_t ssp2bytecrc(uint8_t *data,uint8_t length);
void SSP_Framing(SSP_Init *SSP_Prameters);
void SSP_Deframing(SSP_Init *SSP_Prameters);

#endif
