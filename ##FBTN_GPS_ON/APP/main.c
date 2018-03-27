#include "main.h"


void main()
{
    SystemInit();
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); 
    
    
    Debug_Init();
    
    
    while(TRUE)
    {
        Debug_Rx();
    }
}
