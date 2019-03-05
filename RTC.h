#ifndef __RTC_H
#define __RTC_H


#define getbit(x,bitnum) (x &(1<<bitnum))>>bitnum

#define RTC_LSB_MASK     ((uint32_t)0x0000FFFF)
#define PRLH_MSB_MASK    ((uint32_t)0x000F0000)

#define Overflow_Interrupt 1
#define Alarm_Interrupt 2
#define second_Interrupt 3

#define PrescalerValue_1_sec 0x0000F423

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Functions Prototypes ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void RTC_init(uint32_t PrescalerValue,uint32_t CounterValue,uint32_t AlarmValue,uint8_t interrupt_num);
void Interrupt_Contol(uint8_t interrupt_num);
void RTC_Set_IRQHandler(void (*ptr_to_handler_function) (void) );
void RTC_WaitForSynchro(void);
void RTC_SetAlarm(uint32_t AlarmValue);
void RTC_SetCounter(uint32_t CounterValue);
void RTC_GetCounter(uint32_t x);
void RTC_SetPrescaler(uint32_t PrescalerValue);
void EXIT_CONF_MODE(void);
void ENTER_CONF_MODE(void);
void RTC_clock_EN(void);



#endif
