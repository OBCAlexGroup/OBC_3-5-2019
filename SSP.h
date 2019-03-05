#ifndef __SSP_H
#define __SSP_H

#include "stm32f10x.h"
typedef struct{

	int Destination ;
	int Source ;
	int Type ;
	int Opt_header ;
	int Opt_header_byte ;
	int *Data ;
	int Data_length ;
	int *Framed_data;
	int Frame_length;

}SSP_Init;

#define FEND  0xC0
#define FESC  0xDB
#define TFEND 0xDC
#define TFESC 0xDD

int ssp2bytecrc(int *data,int length);
void SSP_Framing(SSP_Init *SSP_Prameters);
void SSP_Deframing(SSP_Init *SSP_Prameters);

#endif
