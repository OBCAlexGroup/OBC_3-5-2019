#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"
#include "SDcard.h"


#define getbit(x,bitnum) (x &(1<<bitnum))>>bitnum
extern int INT_FLAG  ;


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Functions Prototypes ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


void SPI1_tranfer(uint8_t sent_byte,uint8_t *ptr_recieved_byte);
void SPI1_tranfer_Stream(uint8_t *tranmitted_data,uint8_t length,uint8_t *ptr_recieved_data);
void SPI_set_interrupt(void);
void GPIO_SPI_init_M(void);
void GPIO_SPI_init_S(void);
void SPI1_M_init(void);
void SPI1_S_init(void);
void SPIx_DisableSlave(void);
void SPIx_EnableSlave(void);
#endif
