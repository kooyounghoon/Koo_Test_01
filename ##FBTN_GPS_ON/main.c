#include "main.h"
#include <stdio.h>
void main()
{
  SystemInit(); //�ý��� �������� Clock_Init����
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //���ͷ�Ʈ 
  
  Debug_Init();
  GPS_Init();
  
  GPS_Power_On();
  while(1)
  {
    Debug_Rx();
    
    GPS_Rx();
  }
  
}