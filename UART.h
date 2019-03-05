#ifndef __UART_H
#define __UART_H



#define baud_rate_9600 0x1d4c
#define getbit(x,bitnum) (x &(1<<bitnum))>>bitnum

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Functions Prototypes ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void USART_TX_init(void);
void USART_RX_init(char*Ptr_revieved_data);
void USART_GPIO_init(void);
void USART_TX(char x);
void USART_TX_int(int x);
void USART_TX_String_int(int *x,int length);
void USART_TX_String(char *x);
void USART_set_Interrupt(void);



#endif
