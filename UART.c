#include "stm32f10x.h"
#include "UART.h"

char* revieved_data = 0 ;

void USART_TX_init(void)
{
	// check if the clock is already enabled
	if(getbit(RCC->APB2ENR,14)==0){
		// USART1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN ;
	}

    // choose the word length
	USART1->CR1 &= ~USART_CR1_M;              // 8 data bits
	// set the number of stop bits
	USART1->CR2 &= ~USART_CR2_STOP;           // 1 stop bit
	// set the USART baud rate
	USART1->BRR = baud_rate_9600;
    // enable the transmitter
	USART1->CR1 |= USART_CR1_TE ;
	// enable the USART Peripheral
	USART1->CR1 |= USART_CR1_UE;
}


void USART_RX_init(char*Ptr_revieved_data)
{
	revieved_data = Ptr_revieved_data ;

	// check if the clock is already enabled
	if(getbit(RCC->APB2ENR,14)==0){
		// USART1 clock enable
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN ;
	}

	// choose the word length
	USART1->CR1 &= ~USART_CR1_M;                  // 8 data bits
	// set the number of stop bits
	USART1->CR2 &= ~USART_CR2_STOP;               // 1 stop bit
	// set the USART baud rate
	USART1->BRR = baud_rate_9600;
	// interrupt enable
	USART_set_Interrupt();
    // enable the receiver
	USART1->CR1 |= USART_CR1_RE ;
	// enable the USART Peripheral
	USART1->CR1 |= USART_CR1_UE;
	}

void USART_GPIO_init(void)
{
	// check if the clock is already enabled
	if(getbit(RCC->APB2ENR,2)==0){
		// port A clock enable
	    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	}
	//  port A pin 9 (TX1)(Alternate function output Push-pull(50MHZ))
	GPIOA->CRH |= GPIO_CRH_MODE9 ;
	GPIOA->CRH |= GPIO_CRH_CNF9_1 ;
	//  port A pin 10 (RX1) (input floating)
	GPIOA->CRH &= ~(GPIO_CRH_MODE10) ;
	GPIOA->CRH |= GPIO_CRH_CNF10_0 ;

}

void USART_TX(char x)
{
    // wait until TXE flag in SR is high
	while(!(((USART1->SR & (1<<7))>>7))){}
    // put the data in the DR
	USART1->DR = x ;
    // wait until the transmission is complete (TC goes high)
	while(!((USART1->SR & (1<<6))>>6)){}

}
void USART_TX_int(int x)
{
	// wait until TXE flag in SR is high
		while(!(((USART1->SR & (1<<7))>>7))){}
	    // put the data in the DR
		USART1->DR = x ;
	    // wait until the transmission is complete (TC goes high)
		while(!((USART1->SR & (1<<6))>>6)){}
}
void USART_TX_String_int(int *x,int length)
{
	 for(int i = 0 ; i < length ; i ++){
		while(!(((USART1->SR & (1<<7))>>7)));
		USART1->DR = *x;
		x++;
		while(!((USART1->SR & (1<<6))>>6));
     }

}
void USART_TX_String(char *x)
{
 while(*x != '\0')
 {
	while(!(((USART1->SR & (1<<7))>>7)));
	USART1->DR = *x;
	x++;
	while(!((USART1->SR & (1<<6))>>6));
 }
}


void USART_set_Interrupt(void)
{
      // enable global interrupt
     	__enable_irq();
	  // enable event interrupt in NVIC
		NVIC_EnableIRQ(USART1_IRQn);
	  // Receiver not empty interrupt enable
		USART1->CR1 |= USART_CR1_RXNEIE ;
	}


void USART1_IRQHandler(void)
{

	if (getbit((USART1->SR),5)==1) {

		*revieved_data = USART1->DR ;

	}

}

