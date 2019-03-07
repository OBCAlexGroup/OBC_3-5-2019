#ifndef timerconfig
#define timerconfig

void tim1_ch2_config(unsigned int i,unsigned int j)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	/* TIM1 Configuration */
	TIM1 -> CR1 |= TIM_CR1_ARPE ; //auto reload preload
	TIM1 -> CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS_0 | TIM_CR1_CMS_1);
	TIM1 -> BDTR |= TIM_BDTR_MOE; //ENAble timer o/p
	TIM1 -> EGR |= TIM_EGR_UG;
	TIM1 -> CR1 |= TIM_CR1_CEN;
	TIM1 -> PSC = i;
	TIM1 -> ARR = j;
	 /* TIM1 CH2 PIN A9 SERVO 2 ( UPWARD SERVO ) DIRECTION UP & DOWN */

	GPIOA -> CRH |=  GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 ;
	GPIOA -> CRH &= ~(GPIO_CRH_CNF9_0);
	TIM1 -> CCER |= TIM_CCER_CC2E;
	TIM1 -> CCMR1 &= ~(TIM_CCMR1_CC2S_1 | TIM_CCMR1_CC2S_0);
	TIM1 -> CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 ;
	TIM1 -> CCMR1 &= ~(TIM_CCMR1_OC2M_0);
	TIM1 -> CCER &= ~(TIM_CCER_CC2P); //polarity
}

#endif
