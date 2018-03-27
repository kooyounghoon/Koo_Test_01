#include <gps.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



uint8_t GPS_Rx_Buffer[256] = {0x00, };
uint8_t GPS_Rx_Point = 0;

uint8_t GPS_Parsing_Data[256] = {0x00, };
uint8_t GPS_Parsing_Point = 0;
//uint8_t koo[256] = {0x00,};

//uint8_t GPS_Data[256] = {0x00, };
void GPS_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    
    GPIO_InitTypeDef  gpio_init;
    gpio_init.GPIO_Pin   = GPIO_Pin_2;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpio_init);
	
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    
    
    gpio_init.GPIO_Pin   = GPIO_Pin_12;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &gpio_init);
    
    gpio_init.GPIO_Pin   = GPIO_Pin_2;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &gpio_init);
    
    //////////////////////UART Setting
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_InitTypeDef  usart_init;
    usart_init.USART_BaudRate             = 9600;
    usart_init.USART_WordLength           = USART_WordLength_8b;
    usart_init.USART_StopBits             = USART_StopBits_1;
    usart_init.USART_Parity               = USART_Parity_No;
    usart_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &usart_init);
    
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART5, ENABLE);
}


void GPS_Power_On()
{
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
}

void GPS_Power_Off()
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
}

void GPS_ISR()
{
    GPS_Rx_Buffer[GPS_Rx_Point++] = USART_ReceiveData(UART5);
    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
}

void GPS_Data_Parsing(char* data)
{
    char* tmp;
    tmp = strtok(data, ",");
    while(tmp != NULL)
    
      printf("    [Message ID : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [UTC Time : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [Status : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [Lititude : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [N/S Indicator : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [DATE : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
      printf("    [Checksum : %s] \r\n", tmp);
      tmp = strtok(NULL, ",");
    
       
}
void GPS_Rx()
{
    if(GPS_Rx_Point == 0) return;
    
    for(int i = 0; i < GPS_Rx_Point ; i++)
    {
        GPS_Parsing_Data[GPS_Parsing_Point ++] = GPS_Rx_Buffer[i];
        
        if(GPS_Rx_Buffer[i] == '\n')
        {
            if(strncmp((char*)GPS_Parsing_Data, "$GPRMC", 6) == 0)
            {
                GPS_Data_Parsing((char*)GPS_Parsing_Data);
            }
            
            memset(GPS_Parsing_Data, 0x00, GPS_Parsing_Point);
            GPS_Parsing_Point = 0;
        }

    }
    
    memset(GPS_Rx_Buffer, 0x00, GPS_Rx_Point);
    GPS_Rx_Point = 0;
}




