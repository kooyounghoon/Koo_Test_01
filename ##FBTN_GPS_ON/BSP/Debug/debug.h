#ifndef __DEBUG_H
#define __DEBUG_H


#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>



void Debug_Init();
void Debug_ISR();

void Debug_Rx();

#endif





