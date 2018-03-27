#include <Debug.h>
#include <stdio.h>


uint8_t Debug_Rx_Buffer[256] = {0x00, };
uint8_t Debug_Rx_Point = 0;

void Debug_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE); //
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    
    //UART Pin Setting
    GPIO_InitTypeDef gpio_init; //구조체 재정의 gpio_init으로 사용하겠다
    
    //USART1 Tx Pin Setting
    gpio_init.GPIO_Pin   = GPIO_Pin_6; // GPIO_Pin 매칭
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz; // 속도 설정
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP; // 모드 설정
    GPIO_Init(GPIOB, &gpio_init); // ?
    
    //USART1 Rx Pin Setting
    gpio_init.GPIO_Pin   = GPIO_Pin_7;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio_init);
    
    //USART Connection Information Setting
    USART_InitTypeDef  usart_init;
    usart_init.USART_BaudRate             = 115200;
    usart_init.USART_WordLength           = USART_WordLength_8b;
    usart_init.USART_StopBits             = USART_StopBits_1;
    usart_init.USART_Parity               = USART_Parity_No;
    usart_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usart_init);
    
    //UART Interrupt Setting
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 9;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

//UART1 Interrupt Service Routine
void Debug_ISR()
{
    Debug_Rx_Buffer[Debug_Rx_Point++] = USART_ReceiveData(USART1);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


void Debug_Rx()
{
    if(Debug_Rx_Point == 0) return; //저장된 데이터가 없으면 for문 실행 안함
    
    for(int i = 0; i < Debug_Rx_Point ; i++)
    {
        USART_SendData(USART1, Debug_Rx_Buffer[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
    }
    
    memset(Debug_Rx_Buffer, 0x00, Debug_Rx_Point); //버퍼에 저장된 데이터 초기화
    Debug_Rx_Point = 0;
}
//
//void Debug_Rx()
//{
//    if(Debug_Rx_Point == 0) return; //저장된 데이터가 없으면 for문 실행 안함
//    
//    for(int i = 0; i < Debug_Rx_Point ; i++)
//    {
//        USART_SendData(USART1, Debug_Rx_Buffer[i]);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
//    }
//    
//    memset(Debug_Rx_Buffer, 0x00, Debug_Rx_Point); //버퍼에 저장된 데이터 초기화
//    Debug_Rx_Point = 0;
//}