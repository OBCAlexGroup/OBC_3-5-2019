#include "newspi.h"


uint8_t *recieved_data = -1 ;
char SPI_counter = 0;
extern int cmd ;
//GPIO configuration for SPI1 (master )
void GPIO_SPI_init_M(void)
{

    if(getbit(RCC->APB2ENR,2)==0){
    	// port A clock enable
    	RCC->APB2ENR |= (1<<2);
    }

   	GPIOA->CRL = 0xBBB34444;

}
//GPIO configuration for SPI1 (slave)

// SPI1 master configuration
void SPI1_M_init(void)
{
	SPI_counter = 0;
	char dummy = 0 ;
	// check if the clock is already enabled
    if(getbit(RCC->APB2ENR,12)==0){
    	// SPI1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    }

    // select the suitable Baud Rate
	SPI1->CR1 |= (1<<5);
	SPI1->CR1 |= (1<<4);
	SPI1->CR1 |= (1<<3);
    // select the clock polarity and clock phase
	SPI1->CR1 &= ~SPI_CR1_CPHA ;                            // clock phase mode 0
	SPI1->CR1 &= ~SPI_CR1_CPOL ;                           //  clock polarity (low level when idle)

	/* define 8-bit or 16-bit data frame format*/
	SPI1->CR1 &= ~SPI_CR1_DFF;                           // 8 bit data mode
	// data transfer mode
	SPI1->CR1 &=~ SPI_CR1_BIDIMODE;                           //  2-line unidirectional data mode
    // set Frame format
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;                            // MSB will be transmitted first
    // slave select
	SPI1->CR1 &= ~SPI_CR1_SSM;                             //  set SSM bit (slave select (Hardware mode))
    // set the SPI mode
	SPI1->CR1 |= SPI_CR1_MSTR;                             // master mode
	SPI1->CR2 |= SPI_CR2_SSOE;

	//read the SPI DR to prevent any spurious interrupt
	dummy = SPI1->DR ;
    // interrupt enable
	//SPI_set_interrupt();

	// enable the SPI Peripheral
	SPI1->CR1 |= SPI_CR1_SPE;                             // SPI enable
	SPIx_DisableSlave();
	}





// data transfer (1 byte)
void SPI1_tranfer(uint8_t sent_byte,uint8_t *ptr_recieved_byte)
{
	recieved_data = ptr_recieved_byte ;
    // wait until TXE flag in SR is high
	while((((SPI1->SR)&(1<<1))>>1)==0);
	while((((SPI1->SR)&(1<<1))>>7)==1);
	// put the data in the DR
	SPI1->DR = sent_byte ;
	while((((SPI1->SR)&(1<<0))>>0)==0);
	*ptr_recieved_byte = SPI1->DR ;
	if(*ptr_recieved_byte == 0 && cmd == 77)
		LED_ON() ;
}



// data transfer (stream of bytes)
void SPI1_tranfer_Stream(uint8_t *tranmitted_data,uint8_t length,uint8_t *ptr_recieved_data)
{
	recieved_data = ptr_recieved_data ;
	int i= 0 ;
    while(i<length-1){
    	// wait until TXE flag in SR is high
		while((((SPI1->SR)&(1<<1))>>1)==0){}
		Delay(1000);
		// put the data in the DR
		SPI1->DR = *(tranmitted_data+i) ;
		i++;
    }
}

void SPI_set_interrupt(void)
{
	// enable global interrupt
	   __enable_irq();
    // enable event interrupt in NVIC
	NVIC_EnableIRQ(SPI1_IRQn);
	// RX buffer not empty interrupt enable
	SPI1->CR2 |= (1<<6);

}


void SPI1_IRQHandler(void)
{
	INT_FLAG = 1 ;
	int Flag = -1 ;
	// check if RXNE flag in SR is high
    if((((SPI1->SR)&(1<<0))>>0)==1)
    {

       Flag = SPI1->DR ;
       
       

    }

}

void SPIx_EnableSlave(void)
{
	GPIOA->BSRR &=~(1<<4);
	GPIOA->BSRR |=(1<<20);
}
void SPIx_DisableSlave(void)
{
    // Set slave SS pin high
	GPIOA->BSRR |=(1<<4);
	GPIOA->BSRR &=~(1<<20);
}
void Delay_ (int n)
{
	while(n--);
}
