#ifndef IWDGconfiguration
#define IWDGconfiguration

void IWDG_config(void)
{
	          IWDG->KR = 0x5555; //enable access PSC & Reload value
			  IWDG->PR |= IWDG_PR_PR_0; //PR= PRESCALER REGISTER -> 001: divider /8
			  IWDG->PR &= ~(IWDG_PR_PR_2 | IWDG_PR_PR_1);
			  IWDG->RLR = 0xFFF;  //reload value
			  IWDG->KR = 0x0000; //disable access PSC & Reload value
			  IWDG->KR= 0xCCCC; //start watchdog
}

#endif
