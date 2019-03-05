/*1- change handler of I2C1 with handler in step 2 at startup code */
/*2- extern void I2C1_EV_IRQHandler (void) */
/*3- delete I2C_handler = default handler in startup code */
/*4- make global structure variable of "i2c_parameters" inside "main.h" */
/*5- make Definition of name "HANDLER_STRUCTURE" equals to your structure variable ADDRESS */
/*5- fill structure variable parameters inside your main.c in case of "datatobesent" or
"datatobereceived" assign them to arrays */
/*6- call "gpio_B_clock_enable" then  "AFIO_clock_enable" if you didn't enable them before */
/*7- call I2C_enable  */
/*8- call any of 5 functions below  */


/* NOTE -----------------------------------------------*/
/*for SCCB_Master_TX function you need to use RegisterAddress , deviceAddress , datatobesent */
/* -polling is used with Transmitter and either polling or interrupt is used with RECEVIER */
/* -at RX functions Mode parameter is either 'p' for polling or 'i' for interrupt */
/* -ERROR is not implemented in this code */
/* -in point 5 if you enable clock of GPIOB previously then call "gpio_B_clock_enable" GPIOB_clock
won't work means no data is found on SDA1 or SCL1 lines */
/* -this code is dedicated for I2C1 peripheral only also in Standard mode 100khz to re-adjust it
for I2C2 or Fast mode 400khz change "I2C_enable" parameters also "gpio_B_clock_enable" */

/* HINTS ----------------------------------------------*/
/* STM32F103C8t6 SCL1 PB6 SDA1 PB7 */
/* Arduino SCL1 A5 SDA1 A4 */

#ifndef __HAL_I2C_H
#define __HAL_I2C_H

#define WAITING_UNTIL_START_BIT_GENERATION_SB          (((I2C1->SR1)  & (1<<0)) >> 0 )
#define WAITING_UNTIL_MASTER_GENERATION_STOP_STOPF     (((I2C1->SR1)  & (1<<4)) >> 4 )
#define WAITING_ACK_FROM_SLAVE_OR_MATCHED_ADDRESS_ADDR (((I2C1->SR1)  & (1<<1)) >> 1 )
#define DATA_R_EMPTY_TRANSMIT_TXE                      (((I2C1->SR1)  & (1<<7)) >> 7 )
#define DATA_R_NOT_EMPTY_RXNE                          (((I2C1->SR1)  & (1<<6)) >> 6 )
#define SUCCESSFUL_SENDING_or_RECEVING_BYTE_BTF        (((I2C1->SR1)  & (1<<2)) >> 2 )
#define BUSY_BUS                                       (((I2C1->SR2)  & (1<<1)) >> 1 )
#define EVENT_INTERRUPT_CR_ENABLE                      (((I2C1->CR2)  & (1<<9)) >> 9 )
#define BUFFER_INTERRUPT_CR_ENABLE                     (((I2C1->CR2)  & (1<<10)) >> 10 )
#define ERROR_INTERRUPT_CR_ENABLE                      (((I2C1->CR2)  & (1<<8)) >> 8 )
#define ACK_FAILURE                                    (((I2C1->SR1)  & (1<<10)) >> 10 )
#define MASTER_SLAVE_SELECTION                         (((I2C1->SR2)  & (1<<0)) >> 0 )
#define MASTER_OR_SLAVE_TRANSMITER_OR_RECIVER          (((I2C1->SR2)  & (1<<2)) >> 2 )





typedef struct
{
    int SCCB_FLAG;
    int SCCB_RegisterAddress ;
    int SCCB_AckRegisterAddress ;
    int SCCB_WriteAddress ;
    int SCCB_ReadAddress ;
    int SCCB_lengthtobesent;
    int SCCB_lengthtobereceived;
    int *SCCB_datatobesent;
    int *SCCB_datatobereceived ;
}SCCB_parameters;

typedef struct
{
	int OwnAddress;
    int deviceAddress;
    int *datatobesent ;
    int lengthtobesent;
    int *datatobereceived;
    int lengthtobereceived;
    SCCB_parameters SCCB ;

}i2c_parameters;



void I2C1_EV_IRQHandler (void);
void I2C1_ER_IRQHandler(void);

/*call this function before I2C_enable otherwise I2C won't work*/
void gpio_B_clock_enable(void);
void AFIO_clock_enable (void);
void I2C_enable (i2c_parameters *i2c);

/* Master Function */
void Streaming_Master_TX (i2c_parameters *i2c);
void Streaming_Master_RX (i2c_parameters *i2c,char mode);

/*SCCB Master Function */
void SCCB_Master_TX (i2c_parameters *i2c);
/* Slave Function */
void Streaming_Slave_TX (i2c_parameters *i2c);
void Streaming_Slave_RX (i2c_parameters *i2c,char mode);

#endif
