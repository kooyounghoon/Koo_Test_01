#ifndef __GPS_H
#define __GPS_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include <stdlib.h>

void GPS_Init();
void GPS_Power_On();
void GPS_Power_Off();
void GPS_ISR();
void GPS_Rx();
void GPS_Data_Parsing(char* data);

#endif



