#ifndef SDCARD_H
#define SDCARD_H
/*=============================================================================================*/
/*=============================================================================================*/
/*======================================= SD CARD API's =======================================*/
/*=============================================================================================*/
/*=============================================================================================*/

/*Initization of sdcard */
int  SDInit (void);


/*  Read a block from the sdcard. */
void SD_read(unsigned long sector, unsigned int *rbuffer);


/*  Write a block to the sdcard. */
void SD_write(unsigned long sector, unsigned int *wbuffer);


/* Read multi blocks from the sdcard. */
void  SDMultiBlockRead (int *rBuffer, unsigned long rStartBlockIndex, unsigned long rdBlockNumber) ;


/* Write multi blocks to the sdcard. */
void  SDMultiBlockWrite (const int *wBuffer, unsigned long wStartBlockIndex, unsigned long wBlocksNumber) ;

/*   Erase some blocks. */
void  SDBlockErase (unsigned long eraStartBlockIndex, unsigned long eraBlocksNumber) ;

/*=============================================================================================*/
/*=============================================================================================*/
/*======================================= SD CARD COMMANDS ====================================*/
/*=============================================================================================*/
/*=============================================================================================*/
/** GO_IDLE_STATE */
#define  CMD0  0x40
/** IDLE STATE IS DELETED AND SD_CARD_INITIZALIZATION*/
#define  CMD1  0x41
/** SET_BLOCK_SIZE_R_W */
#define  CMD16 0x50
/** READ_SINGLE_BLOCK */
#define  CMD17 0x51
/** READ_MULTI_BLOCK */
#define  CMD18 0x52
/*=============================================================================================*/
/*=============================================================================================*/
/*======================================= SD CARD RESPONSES ===================================*/
/*=============================================================================================*/
/*=============================================================================================*/
/**  READY_STATE */
#define R1_READY_STATE  0X00
/**  IDLE STATE  */
#define R1_IDLE_STATE  0X01
/** ILLEGEL command */
#define R1_ILLEGAL_COMMAND  0X04
/** DATA STARTED for read or write single block */
#define DATA_START_BLOCK  0XFE
/** stop token for write multiple blocks */
#define STOP_TRAN_TOKEN  0XFD
/** start data token for write multiple blocks */
#define WRITE_MULTIPLE_TOKEN  0XFC;

/*=============================================================================================*/
/*=============================================================================================*/
/*======================================= SD CARD FLAGS =======================================*/
/*=============================================================================================*/
/*=============================================================================================*/


#endif
