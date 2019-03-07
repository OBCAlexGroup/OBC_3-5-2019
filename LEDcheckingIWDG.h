#ifndef LEDconfiguration
#define LEDconfiguration

void LED_config(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN ; //ENABLING GPIO PORT A
	GPIOA -> CRH |=    GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 ; //ENABLING PIN A10 LED
	GPIOA -> CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF10_0);
}

#endif
