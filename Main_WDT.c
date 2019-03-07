#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "TIM1CH2CONFIG.h"
#include "IWDGCONFIG.h"
#include "LEDcheckingIWDG.h"

void delay(unsigned int n)
{
	int i,j;
  for( i=0; i <= n; i++)
  {
	  for(j =0; j<=1000 ;j++);
  }
}

int main(void)
{
    RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN; //Config GPIO port B

    GPIOB -> CRH &= ~(GPIO_CRH_CNF12_1 | GPIO_CRH_CNF12_0 ); //Config B12 PIN for red led "indicating Reset flag"
	GPIOB -> CRH |= GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0 ;

	if (RCC->CSR & RCC_CSR_IWDGRSTF)   // checking if IWDG Reset Flag set
    {
	GPIOB->BSRR = GPIO_BSRR_BS12 ;  // Red LED on indicating the setting of the flag

	RCC->CSR |= (RCC_CSR_RMVF);    // Clear Reset Flags
    }

	else
	{
	GPIOB->BRR  = GPIO_BRR_BR12;    // red LED off, flag isn't set, as no reset released
	}

	LED_config(); //EXAMPLE(1) FOR A CODE Setting a PIN A10 FOR YELLOW LED

	tim1_ch2_config(65535,1000); //TIM1_CH2_CONFIGURATION & 65535->PSC, 1000->ARR,"PWM EXAMPLE(2)"

	IWDG_config();// IWDG CONFIGURATION

	while(1)
	{
	  delay(10000);        /* delay simulation DELAY(10000) -> RESET ,
	                      DELAY(1000) -> NO RESET*/

	  IWDG->KR = 0xAAAA;  // feed the dog before the IWDG counter reaches zero TO AVOID RESET

	  TIM1 -> CCR2 = 900; // PWM - DUTY cycle 90%  "tim1_ch2_config FUNCTION"

	  GPIOA -> BSRR |= GPIO_BSRR_BS10; //setting led pin A10 "LED_CONFIG FUNCTION"
}
}
