#include "SDcard.h"
int IDLE_FLAG = 0 ;
int READY_FLAG = 0 ;
int INT_FLAG  = 0;

void delay (void)
{
    for(int i = 0 ; i < 1000 ; i++);

}
int  SDInit (void)
{
    int i    = 0 ;
	int data = 0 ;
	/** SPI INIZIALIZATION */
	GPIO_SPI_init_M();
    SPI1_M_init();

    /** must supply min of 74 clock cycles with CS high. */
	SPIx_DisableSlave();
	for(int i = 0 ; i < 10 ; i++)
	SPI1_tranfer(0xff,&data);

	/** Select SDcard. */
	for(i=0; i<10 && cardCommand(CMD0  , 0x00000000) != R1_IDLE_STATE; i++)  ;
	if(i == 10)
	return -1; // card did not respond to idle cmd

	for(i=0; i<10 && cardCommand(CMD1  , 0x00000000) != R1_READY_STATE; i++)  ;
	if(i == 10)
	return -2; // card did not respond to initialization cmd

	for(i=0; i<10 && cardCommand(CMD16 , 0x00000200) != R1_READY_STATE; i++)  ;
	if(i == 10)
	return -3; // card did not respond to set block cmd

	/* return zero in case success*/
    return 0 ;




    /** Deselect SDcard. */


}
//------------------------------------------------------------------------------
/** . */
int cardCommand(int cmd, int arg) {

  int i        = 0;
  int ret      = 0xFF;
  int rdata[8] = {0};
  // select card
  SPIx_EnableSlave();

  // send command
  SPI1_tranfer(cmd,&rdata);

  // send argument
  for (int s = 24; s >= 0; s -= 8)
  SPI1_tranfer(arg >> s,&rdata);

  // send CRC
  int crc = 0xFF;
  if (cmd == CMD0) crc = 0X95;  // correct crc for CMD0 with arg 0x00000000
  SPI1_tranfer(crc,&rdata);

  // wait for response
  for( i = 0 ; i < 8 ; i++)
  SPI1_tranfer(0xff,&rdata[i]);

  //dumy delay
  delay();
  SPIx_DisableSlave();

  //return response
  for( i=0; i<8; i++ )
  {
  		if(rdata[i] != 0xFF)
  	    ret = rdata[i];
  		//if(ret == 0 && cmd == 0x50)
  		//LED_ON() ;
  }

  return ret ;


}

void SD_read(unsigned long sector, unsigned int *rbuffer)
{
	int i = 0 ;
	int dumy = 0 ;
	SPIx_EnableSlave();
	SPI1_tranfer(0x51,&dumy);
	SPI1_tranfer(sector>>15,&dumy); // sector*512 >> 24
	SPI1_tranfer(sector>>7,&dumy);  // sector*512 >> 16
	SPI1_tranfer(sector<<1,&dumy);  // sector*512 >> 8
	SPI1_tranfer(0,&dumy);          // sector*512
	SPI1_tranfer(0xFF,&dumy);

	for(i=0; i<100 && dumy != R1_READY_STATE; i++) {
		SPI1_tranfer(0xFF,&dumy);
	} // wait for 0

	for(i=0; i<100 && dumy != DATA_START_BLOCK; i++) {
	SPI1_tranfer(0xFF,&dumy);
	} // wait for data start

	for(i=0; i< 32; i++)
	{
		SPI1_tranfer(0xFF,&rbuffer[i]);

	}

    delay();

	SPIx_DisableSlave();

	/*STOP CMD*/
	SPIx_EnableSlave();
	SPI1_tranfer(77,&dumy);
	SPI1_tranfer(0,&dumy); // sector*512 >> 24
	SPI1_tranfer(0,&dumy);  // sector*512 >> 16
	SPI1_tranfer(0,&dumy);  // sector*512 >> 8
	SPI1_tranfer(0,&dumy);          // sector*512
	SPI1_tranfer(0xFF,&dumy);
	for(int i = 0 ; i < 8 ; i++)
	SPI1_tranfer(0xff,&dumy);
	Delay();
	SPIx_DisableSlave();
}
void SD_write(unsigned long sector, unsigned int *wbuffer)
{
	int i = 0 ;
	int dumy = 0 ;
	SPIx_EnableSlave();
	SPI1_tranfer(88,&dumy);
	SPI1_tranfer(sector>>15,&dumy); // sector*512 >> 24
	SPI1_tranfer(sector>>7,&dumy);  // sector*512 >> 16
	SPI1_tranfer(sector<<1,&dumy);  // sector*512 >> 8
	SPI1_tranfer(0,&dumy);          // sector*512
	SPI1_tranfer(0xFF,&dumy);

	for(i=0; i<100 && dumy != R1_READY_STATE; i++) {
		SPI1_tranfer(0xFF,&dumy);
	} // wait for 0

   	SPI1_tranfer(DATA_START_BLOCK,&dumy);

	for(i=0; i< 512; i++)
	{
		SPI1_tranfer(wbuffer[i],&dumy);

	}

    delay();

	SPIx_DisableSlave();
	int data ;
	/*STOP CMD*/
	SPIx_EnableSlave();
	SPI1_tranfer(77,&data);
	SPI1_tranfer(0,&data); // sector*512 >> 24
	SPI1_tranfer(0,&data);  // sector*512 >> 16
	SPI1_tranfer(0,&data);  // sector*512 >> 8
	SPI1_tranfer(0,&data);          // sector*512
	SPI1_tranfer(0xFF,&data);
	for(int i = 0 ; i < 8 ; i++)
	SPI1_tranfer(0xff,&data);
	Delay();
	SPIx_DisableSlave();

}
