#include "main.h"
#include <stdio.h>
void main()
{
  SystemInit(); //시스템 전반적인 Clock_Init설정
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //인터럽트 
  
  Debug_Init();
  GPS_Init();
  
  GPS_Power_On();
  while(1)
  {
    Debug_Rx();
    
    GPS_Rx();
  }
  
}