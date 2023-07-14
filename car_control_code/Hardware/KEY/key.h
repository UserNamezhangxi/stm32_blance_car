#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define DEFAULT_KEY 99
#define DEFAULT_AD_KEY 98
#define NO_KEY 0

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_PRE 5
#define KEY_NEXT 6
#define KEY_PLAY_PAUSE 7
#define KEY_LED 8
#define KEY_BEEP 9
#define KEY_CAR_MODE 10

#define LEFT 20
#define LEFT_UP 21
#define UP 22
#define RIGHT_UP 23
#define RIGHT 24
#define RIGHT_DOWN 25
#define DOWN 26
#define LEFT_DOWN 27

#define KEY_1 1
#define KEY_2 2
#define KEY_3 3

void Key_Init(void);
u8 Key_GetNum(u8 *key, u8 *value);

#endif
