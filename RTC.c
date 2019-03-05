#include "stm32f10x.h"
#include "RTC.h"


// global pointer to function
void (*g_ptr) (void);


void RTC_init(uint32_t PrescalerValue,uint32_t CounterValue,uint32_t AlarmValue,uint8_t interrupt_num)
{
	// configure the clock provided to RTC peripheral
	RTC_clock_EN();
	// set the Pre_scaler value
	RTC_SetPrescaler(PrescalerValue);
	// set the Counter value
	RTC_SetCounter(CounterValue);
	// set the Alarm value
	RTC_SetAlarm(AlarmValue);
	// specify the peripheral interrupt
	Interrupt_Contol(interrupt_num);

	}

void RTC_clock_EN(void)
{
	// set the PWREN bit , BKPEN bit in RCC_APB1ENR
	RCC->APB1ENR |= (1<<28) ;                         // PWREN bit
	RCC->APB1ENR |= (1<<27) ;                         // BKPEN bit
	// set DBP bit in PWR_CR
	PWR->CR |= (1<<8) ;                               // DBP bit
	// RTC clock source selection
	RCC->BDCR |= 0x0300 ;                             //   HSE oscillator clock divided by 128 used as RTC clock
	// enable RTC clock
	RCC->BDCR |= (1<<15) ;

	}

void ENTER_CONF_MODE(void)
{
	// Poll RTOFF in RTC_CRL, wait until its value goes to ‘1’ to check the end of the write operation.
	while(getbit((RTC->CRL),5)==0) ;
	// set CNF bit in RTC_CRL
	RTC->CRL |= (1<<4) ;

	}

void EXIT_CONF_MODE(void)
{
	// clear CNF bit in RTC_CRL
		RTC->CRL &=~ (1<<4) ;
	// Poll RTOFF in RTC_CRL, wait until its value goes to ‘1’ to check the end of the write operation.
		while(getbit((RTC->CRL),5)==0) ;

	}

void RTC_SetPrescaler(uint32_t PrescalerValue)
{

	  ENTER_CONF_MODE()  ;
	  //Set RTC PRESCALER MSB word
	  RTC->PRLH = (PrescalerValue & PRLH_MSB_MASK) >> 16 ;
	  // Set RTC PRESCALER LSB word
	  RTC->PRLL = (PrescalerValue & RTC_LSB_MASK) ;
	  EXIT_CONF_MODE() ;


	  }

void RTC_GetCounter(uint32_t x)
{
  uint16_t tmp = 0 ;
  tmp = RTC->CNTL ;
  x = (((uint32_t)RTC->CNTH << 16 ) | tmp) ;

  }


void RTC_SetCounter(uint32_t CounterValue)
{
	ENTER_CONF_MODE() ;
    /* Set RTC COUNTER MSB word */
    RTC->CNTH = CounterValue >> 16 ;
    /* Set RTC COUNTER LSB word */
    RTC->CNTL = (CounterValue & RTC_LSB_MASK) ;
    EXIT_CONF_MODE() ;

  }

void RTC_SetAlarm(uint32_t AlarmValue)
{
	ENTER_CONF_MODE() ;
    /* Set the ALARM MSB word */
    RTC->ALRH = AlarmValue >> 16 ;
    /* Set the ALARM LSB word */
    RTC->ALRL = (AlarmValue & RTC_LSB_MASK) ;
    EXIT_CONF_MODE() ;
}


/**
  *   Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
  *   are synchronized with RTC APB clock.
  *   This function must be called before any read operation after an APB reset
  *   or an APB clock stop.
  * */

void RTC_WaitForSynchro(void)
{

  RTC->CRL &= ~(1<<3) ;                                   // Clear RSF flag

  while (getbit((RTC->CRL),3) == 0);    //Loop until RSF flag is set

  }

void Interrupt_Contol(uint8_t interrupt_num)
{
	  // enable global interrupt
		__enable_irq();
	  // enable event interrupt in NVIC
		NVIC_EnableIRQ(RTC_IRQn);                  //  RTC handler
		NVIC_EnableIRQ(RTCAlarm_IRQn);                 // RTCAlarm handler

	// enable the peripheral interrupt
	if(interrupt_num==1)
	{
		// Poll RTOFF in RTC_CRL, wait until its value goes to ‘1’ to check the end of the write operation.
		while(getbit((RTC->CRL),5)==0) ;
		RTC->CRH |= (1<<2)	;               // Overflow interrupt enable

	}else if(interrupt_num==2){

		while(getbit((RTC->CRL),5)==0) ;
		RTC->CRH |= (1<<1)	;               // Alarm interrupt enable

	}else if(interrupt_num==3){

		while(getbit((RTC->CRL),5)==0) ;
		RTC->CRH |= (1<<0)	;               //Second interrupt enable
	}

}

/*  this function sets the function to be handled by the RTC_IRQHandler
 *  parameter : the name of the function
 */

void RTC_Set_IRQHandler(void (*ptr_to_handler_function) (void) )
{
	g_ptr=ptr_to_handler_function;

}

void RTC_IRQHandler(void)
{
	(*g_ptr)();      // call back function

}


