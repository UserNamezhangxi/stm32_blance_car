#ifndef __AD_H
#define __AD_H

#include "key.h"

extern uint16_t AD_Value[2];

void AD_Init(void);
uint16_t Vertical(void);
uint16_t Horizontal(void);
uint16_t Get_AD_Key(void);
#endif
