#include "stm32f10x.h"
#include "i2c.h"
#include "main.h"

/* Master Function */
void Streaming_Slave_TX (i2c_parameters *i2c);
void Streaming_Master_RX (i2c_parameters *i2c,char mode);
void I2C_MasterReceive_RXNE (i2c_parameters *i2c);
void I2C_MasterReceive_BTF (i2c_parameters *i2c);

/* Slave Function */
void Streaming_Slave_TX (i2c_parameters *i2c);
void Streaming_Slave_RX (i2c_parameters *i2c,char mode);
void I2C_SlaveReceive_BTF (i2c_parameters *i2c);
void I2C_SlaveReceive_RXNE (i2c_parameters *i2c);
void I2C_Slave_STOPF(i2c_parameters *i2c);

//ACK need to be set after Enable of peripheral or it won't work///////////////////////////////
///before selecting mode you should specify address of device and own address otherwise it won't
///work

void  I2C1_EV_IRQHandler (void)
{
  /* flags for each interrupt */
   int tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0 , tmp5 ;

   /* Master mode */
    if(MASTER_SLAVE_SELECTION)
    {
       /* Master Transmiter */
    	if(MASTER_OR_SLAVE_TRANSMITER_OR_RECIVER)
    	{/* use polling version Streaming_Master_TX */}

       /* Master Receiver */
    	else
    	{
    		tmp1 =  (((I2C1->SR1)  & (1<<6)) >> 6);
    		tmp2 =  (((I2C1->CR2)  & (1<<10)) >> 10);
    		tmp3 =  (((I2C1->SR1)  & (1<<2)) >> 2);
    		tmp4 =  (((I2C1->CR2)  & (1<<9)) >> 9);
            /* RXNE set and BTF reset -----------------------------------------------*/
            if((tmp1) && (tmp2) && (!tmp3))
            {
            I2C_MasterReceive_RXNE(HANDLER_STRUCTURE);
            }
            /* BTF set -------------------------------------------------------------*/
            else if((tmp3) && (tmp4))
            {
            I2C_MasterReceive_BTF(HANDLER_STRUCTURE);
            }
    	}
    }

   /* Slave mode */
    else
    {
       /* Slave Transmiter */
    	if(MASTER_OR_SLAVE_TRANSMITER_OR_RECIVER)
    	{/* use polling version Streaming_Slave_TX */}

       /* Slave Receiver */
    	else
    	{
    		tmp1 =  (((I2C1->SR1)  & (1<<6)) >> 6);
    		tmp2 =  (((I2C1->CR2)  & (1<<10)) >> 10);
    		tmp3 =  (((I2C1->SR1)  & (1<<2)) >> 2);
    		tmp4 =  (((I2C1->CR2)  & (1<<9)) >> 9);
    		tmp5 =  (((I2C1->SR1)  & (1<<4)) >> 4 );
    		/* RXNE set and BTF reset ----------------------------------------------*/
    		if((tmp1 ) && (tmp2 ) && (! tmp3))
    		{
    		   I2C_SlaveReceive_RXNE(HANDLER_STRUCTURE);
    	    }
    		/* BTF set -------------------------------------------------------------*/
    		else if((tmp3) && (tmp4))
    		{
    		  I2C_SlaveReceive_BTF(HANDLER_STRUCTURE);
    		}
    		else if (tmp5)
    		{
    			I2C_Slave_STOPF(HANDLER_STRUCTURE);
    		}
    	}
    }

}

void  I2C1_ER_IRQHandler (void)
{
	if(ACK_FAILURE)
	I2C_ACK_FAILURE(HANDLER_STRUCTURE);
}

void I2C_enable (i2c_parameters *i2c)
{
	/*Clock enable i2c*/
    RCC->APB1ENR |= (1<<21) ;            /// enable clock for I2C1 should be first
	GPIOB->CRL |= 0xEE000000 ;           /// open drain outputs
	AFIO->MAPR &=~(1<<1);                /// no mapping

	/*i2c configuration*/
	I2C1->CR1 &=~ (1<<0);                ///disable i2c to make sure nothing goes wrong
	I2C1->CR1 &=~ (1<<7);                 ///disable clock stretching
	I2C1->CR2 |= 0x8 ;                   ///peripheral input clock = 8 Mhz
	I2C1->OAR1 |= ((i2c->OwnAddress)<<1) ;    ///own address of OBC STM
	I2C1->OAR1 &=~ (1<<15) ;             ///address 7 bit mode selected
	I2C1->OAR1 |= (1<<14) ;              ///should kept 1 by software
    I2C1->OAR2 &=~ (1<<0);               ///only OAR1 is used
	//I2C1->CCR |= 0x801E ;
	I2C1->CCR |= 0x28 ;                  ///CCR seting
	I2C1->CCR &=~ (1<<15) ;              /// SM mode
    I2C1->TRISE = 0x9 ;                ///max rise time
    I2C1->CR1 |= (1<<0);                 ///enable i2c


}
void I2C_enable_error_interrupt (void)
{
	__enable_irq();                      ///enable global interrupt
	NVIC_EnableIRQ(31);                  ///enable event interrupt in NVIC
	NVIC_EnableIRQ(32);                  ///enable error interrupt in NVIC
	I2C1->CR2 |= (1<<8);                 ///enable peripheral error interrupt

}
void I2C_enable_interrupt(void)
{
    /*enable interrupt*/
	__enable_irq();                      ///enable global interrupt
	NVIC_EnableIRQ(31);                  ///enable event interrupt in NVIC
	NVIC_EnableIRQ(32);                  ///enable error interrupt in NVIC
	I2C1->CR2 |= (1<<8);                 ///enable peripheral error interrupt
	I2C1->CR2 |= (1<<9);                 ///enable peripheral event interrupt
	I2C1->CR2 |= (1<<10);                ///enable peripheral buffer interrupt

}
void gpio_B_clock_enable(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN ; /// enable clock for GPIO_B
}
void AFIO_clock_enable (void)
{
	RCC->APB2ENR |= (1<<0) ;             /// enable clock for AFIO
}

void Streaming_Master_TX (i2c_parameters *i2c)
{

	             while(BUSY_BUS);
	             I2C1->CR1 |= (1<<8);
	    		 while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );                                      /// waiting for complete start geneartion (SB)
	    		 I2C1->SR1 ;
	    		 I2C1->DR = ((i2c->deviceAddress)<<1) | (0);
	    		 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
	    		 I2C1->SR1 ;                                                        /// to completely clear (ADDR) bit
	    		 I2C1->SR2 ;
	    		 ///TXE = 1
	         	 I2C1->DR = *(i2c->datatobesent++);
	         	 ///TXE = 1
	         	 I2C1->DR = *(i2c->datatobesent++);
	         	 ///TXE = 0
	         	 for(int i_ = 2 ; i_ < (i2c->lengthtobesent) ; i_++  )
	             {
	         	 while(!(((I2C1->SR1)  & (1<<7)) >> 7 ));
	             ///TXE = 1
	             I2C1->DR = *(i2c->datatobesent++);
	             }

	             while((!(((I2C1->SR1)  & (1<<7)) >> 7 )) &&(!(((I2C1->SR1)  & (1<<2)) >> 2 )) );

			     I2C1->CR1 |= (1<<9)   ;
}
void BYTE_Master_RX (i2c_parameters *i2c)
{
	                 I2C1->CR1 &=~(1<<10);
		             while(BUSY_BUS);
	                 I2C1->CR1 |= (1<<8);
		    		 while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );                                      /// waiting for complete start geneartion (SB)
		    		 I2C1->SR1 ;
		    		 I2C1->DR = ((i2c->deviceAddress)<<1) | (1);
		    		 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
		    		 I2C1->SR1 ;
		    		 I2C1->SR2 ;
		    		 I2C1->CR1 |= (1<<9) ;
		    		 while(!DATA_R_NOT_EMPTY_RXNE);
		    		 *(i2c->datatobereceived++) =  I2C1->DR ;

}
void Streaming_Master_RX_2BYTE (i2c_parameters *i2c)
{
	                     char p ;
	                     I2C1->CR1 |=(1<<10);  ///ACK = 1
	                     I2C1->CR1 |=(1<<11);  /// POS= 1
	    	             while(BUSY_BUS);
	                     I2C1->CR1 |= (1<<8);
			    		 while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );
			    		 I2C1->SR1 ;
			    		 I2C1->DR = ((i2c->deviceAddress)<<1) | (1);
			    		 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
			    		 I2C1->SR1 ;
			    		 I2C1->SR2 ;
	                     I2C1->CR1 &=~(1<<10);  ///ACK = 0
			    		 while(!SUCCESSFUL_SENDING_or_RECEVING_BYTE_BTF);
			    		 I2C1->CR1 |= (1<<9) ;
			    		 *(i2c->datatobereceived++) =  I2C1->DR ;
			    		 *(i2c->datatobereceived++) =  I2C1->DR ;
}
void Streaming_Master_RX (i2c_parameters *i2c,char mode)
{
	   int p ;

    /* polling mode */
	    if(mode == 'p')
	    {
	    	   I2C_disable_interrupt();
	           I2C1->CR1 |=(1<<10); ///ACK = 1
	           I2C1->CR1 |=(1<<11); /// POS= 1
	    	   while(BUSY_BUS);
	           I2C1->CR1 |= (1<<8);
			   while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );
			   I2C1->SR1 ;
			   I2C1->DR = ((i2c->deviceAddress)<<1) | (1);
			   while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
			   I2C1->SR1 ;
			   I2C1->SR2 ;
			    /*need handler interrupt*/
			    for(int f = 0 ; f < (i2c->lengthtobereceived) - 3 ; f++)
			    {
			    		 while(!DATA_R_NOT_EMPTY_RXNE);
			    		 *(i2c->datatobereceived++) =  I2C1->DR ;
			    }
			    while(!SUCCESSFUL_SENDING_or_RECEVING_BYTE_BTF);
			    I2C1->CR1 &=~(1<<10);  ///ACK = 0
			    *(i2c->datatobereceived++)=  I2C1->DR ;
	    		I2C1->CR1 |= (1<<9) ;
	    		*(i2c->datatobereceived++)  =  I2C1->DR ;
                while(!DATA_R_NOT_EMPTY_RXNE);
                *(i2c->datatobereceived++)=  I2C1->DR ;
	    }
	    /* interrupt mode */
	    else if (mode == 'i')
	    {
	    	I2C1->CR1 |=(1<<10); ///ACK = 1
	    	I2C1->CR1 |=(1<<11); /// POS= 1
	    	while(BUSY_BUS);
	    	I2C1->CR1 |= (1<<8);
	    	while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );
	    	I2C1->SR1 ;
	        I2C1->DR = ((i2c->deviceAddress)<<1) | (1);
	    	while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
	    	I2C1->SR1 ;
	    	I2C1->SR2 ;
	    	I2C_enable_interrupt();
	    }


}
void Streaming_Slave_TX (i2c_parameters *i2c)
{
     I2C1->CR1 |=(1<<10); ///ACK = 1
	 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
	 I2C1->SR1 ;
	 I2C1->SR2 ;
	 int f = 0 ;
	 for( f = 0 ; f < (i2c->lengthtobesent) ; f++)
	 {
		 while(!DATA_R_EMPTY_TRANSMIT_TXE);
		 if(f == 9)
		 PC13_on_off(1);
		 I2C1->DR = *(i2c->datatobesent++) ;
		 if(ACK_FAILURE)
		 {
		 I2C1->SR1 &=~(1<<10) ;
		 break;
		 }
	 }

	 while(!WAITING_UNTIL_MASTER_GENERATION_STOP_STOPF);
	 I2C1->SR1 ;
     I2C1->CR1 &=~ (1<<10); ///ACK = 0

}
void Streaming_Slave_RX (i2c_parameters *i2c,char mode)
{
 /*polling mode*/
 if(mode == 'p')
  {
	 I2C_disable_interrupt();
     I2C1->CR1 |=(1<<10); ///ACK = 1
	 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
	 I2C1->SR1 ;
	 I2C1->SR2 ;
	 for(int f = 0 ; f < (i2c->datatobereceived)-1 ; f ++)
	 {
		 while(!DATA_R_NOT_EMPTY_RXNE);
		 *(i2c->datatobereceived++) = I2C1->DR ;
	 }
	 while(!WAITING_UNTIL_MASTER_GENERATION_STOP_STOPF);
	 I2C1->SR1 ;
     I2C1->CR1 &=~ (1<<10); ///ACK = 0
  }
 /* interrupt mode*/
 else if (mode == 'i')
 {
	 I2C1->CR1 |=(1<<10); ///ACK = 1
	 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
     I2C1->SR1 ;
	 I2C1->SR2 ;
	 I2C_enable_interrupt();
 }

}
void I2C_MasterReceive_RXNE (i2c_parameters *i2c)
{

	  if((i2c->lengthtobereceived) > 3)
	  {
	    /* Read data from DR */
		  *(i2c->datatobereceived)= I2C1->DR ;
		  *(i2c->datatobereceived)++;

	    /* Decrease counter */
		(i2c->lengthtobereceived)--;

	  }
	    else if(((i2c->lengthtobereceived) == 2) || ((i2c->lengthtobereceived) == 3))
	    {

	      /* Disable BUF interrupt */
	   		 I2C1->CR2 &=~ (1<<10);                 ///disable peripheral buffer interrupt
	    }
	    else
	    {

	      /* Disable EVT, BUF and ERR interrupt */
	    	 I2C1->CR2 &=~ (1<<8);                 ///disable peripheral error interrupt
	   		 I2C1->CR2 &=~ (1<<9);                 ///disable peripheral event interrupt
	   		 I2C1->CR2 &=~ (1<<10);                ///disable peripheral buffer interrupt

	      /* Read data from DR */
	   		*(i2c->datatobereceived++)= I2C1->DR ;

		  /* Decrease counter */
	   		(i2c->lengthtobereceived)--;
	    }
}
void I2C_MasterReceive_BTF (i2c_parameters *i2c)
{
	     if((i2c->lengthtobereceived) == 3)
	       {

	        /* Disable Acknowledge */
			I2C1->CR1 &=~(1<<10);  ///ACK = 0

	        /* Read data from DR */
			*(i2c->datatobereceived) = I2C1->DR ;
			*(i2c->datatobereceived)++;

			/* Decrease counter */
			(i2c->lengthtobereceived)--;


	       }
	       else if((i2c->lengthtobereceived) == 2)
	       {
	         /* Generate Stop */
	         I2C1->CR1 |= (1<<9) ;

	         /* Read data from DR */
	         *(i2c->datatobereceived) = I2C1->DR ;
	         *(i2c->datatobereceived)++ ;

			/* Decrease counter */
	         (i2c->lengthtobereceived)--;


	         /* Read data from DR */
	         *(i2c->datatobereceived)= I2C1->DR ;
	         *(i2c->datatobereceived)++;


	         /* Decrease counter */
		     (i2c->lengthtobereceived)--;


	         /* Disable EVT and ERR interrupt */
		 	 I2C1->CR2 &=~ (1<<8);                 ///disable peripheral error interrupt
		 	 I2C1->CR2 &=~ (1<<9);                 ///disable peripheral event interrupt

	       }
	       else
	       {
	         /* Read data from DR */
	    	   *(i2c->datatobereceived) = I2C1->DR ;
	    	   *(i2c->datatobereceived++);

	    	   /* Decrease counter */
	    	   (i2c->lengthtobereceived)--;
	       }
}
void I2C_SlaveReceive_RXNE (i2c_parameters *i2c)
{
	  if((i2c->lengthtobereceived) != 0)
	  {
		 /* Read data from DR */
		  *(i2c->datatobereceived)  = I2C1->DR ;
		  *(i2c->datatobereceived)++;

		  /* Decrease counter */
		  (i2c->lengthtobereceived)--;
	  }
}
void I2C_SlaveReceive_BTF (i2c_parameters *i2c)
{
	  if((i2c->lengthtobereceived) != 0)
	  {
		 /* Read data from DR */

		  *(i2c->datatobereceived) = I2C1->DR ;
		  *(i2c->datatobereceived)++;


		  /* Decrease counter */
		  (i2c->lengthtobereceived)--;
	  }
}
 void I2C_Slave_STOPF(i2c_parameters *i2c)
{

  /* Disable EVT, BUF and ERR interrupt */
	 I2C1->CR2 &=~ (1<<8);                 ///disable peripheral error interrupt
	 I2C1->CR2 &=~ (1<<9);                 ///disable peripheral event interrupt
	 I2C1->CR2 &=~ (1<<10);                ///disable peripheral buffer interrupt

  /* Clear STOPF flag */
	 I2C1->SR1 ;


  /* Disable Acknowledge */
	I2C1->CR1 &=~ (1<<10); ///ACK = 0

}
void I2C_disable_Error_interrupt (void)
{
	I2C1->CR2 &=~ (1<<8);                 ///disable peripheral error interrupt

}

void I2C_disable_interrupt (void)
{
	I2C1->CR2 &=~ (1<<8);                 ///disable peripheral error interrupt
	I2C1->CR2 &=~ (1<<9);                 ///disable peripheral event interrupt
	I2C1->CR2 &=~ (1<<10);                ///disable peripheral buffer interrupt
}
void I2C_ACK_FAILURE(i2c_parameters *i2c)
{

   /*SCCB mode*/
  if(i2c->SCCB.SCCB_FLAG == 1)
  {
	 /*Clear ACK flag*/
	 I2C1->SR1 &=~(1<<10) ;
     I2C1->CR1 |= (1<<8);

   }

}
void SCCB_Master_TX (i2c_parameters *i2c)
{

     while(BUSY_BUS);
     I2C1->CR1 |= (1<<8);
	 while( !(((I2C1->SR1)  & (1<<0)) >> 0 ) );                                      /// waiting for complete start geneartion (SB)
	 I2C1->SR1 ;
	 I2C1->DR = ((i2c->deviceAddress)<<1) | (0);
	 while(!(((I2C1->SR1)  & (1<<1)) >> 1 ));
	 I2C1->SR1 ;                                                        /// to completely clear (ADDR) bit
	 I2C1->SR2 ;
}
