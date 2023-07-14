#ifndef __SYS_H_
#define __SYS_H_

#include <stm32f10x.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "usart.h"
#include "usart3.h"
#include "adc.h"
#include "timer.h"
#include "motor.h"
#include "encoder.h"
#include "ioi2c.h"
#include "mpu6050.h"
#include "show.h"
#include "exti.h"
#include "DataScope_DP.h"
#include "control.h"
#include "filter.h"
#include "nrf24l01.h"
#include "sys.h"

// 0,不支持ucos
// 1,支持ucos
#define SYSTEM_SUPPORT_UCOS 0 // 定义系统文件夹是否支持UCOS

// 位带操作,实现51类似的GPIO控制功能
// 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
// IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 12) // 0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 8) // 0x40011E08

// IO口操作,只对单一的IO口!
// 确保n的值小于16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  // 输入

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)  // 输入

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)  // 输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)  // 输入

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)  // 输入
/////////////////////////////////////////////////////////////////
// Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6

#define FTIR 1 // 下降沿触发
#define RTIR 2 // 上升沿触发

#define DEFAULT_KEY 99
#define DEFAULT_AD_KEY 98
#define NO_KEY 0

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

#define KEY_1 1 // 键盘AD方向
#define KEY_2 2 // 键盘右侧上下左右等按键，包括播放暂停
#define KEY_3 3 // 旋钮音量

#define MUSIC_PLAY "A2"                     // 播放
#define MUSIC_PAUSE "A3"                    // 暂停
#define MUSIC_STOP "A4"                     // ֹͣ停止
#define MUSIC_PRE "A5"                      // 上一曲
#define MUSIC_NEXT "A6"                     // 下一曲
#define MUSIC_VOLUME_ADD "B0"               // 音量+
#define MUSIC_VOLUME_SUB "B1"               // 音量-
#define MUSIC_PLAY_MODE_LOOP_ALL "B4:00"    // 循环播放
#define MUSIC_PLAY_MODE_LOOP_SINGLE "B4:01" // 单曲播放
#define MUSIC_PLAY_MODE_RANDOM "B4:03"      // 随机播放

////JTAG模式设置定义
#define JTAG_SWD_DISABLE 0X02
#define SWD_ENABLE 0X01
#define JTAG_SWD_ENABLE 0X00

#define MUSIC_PLAY "A2"                     // 播放
#define MUSIC_PAUSE "A3"                    // 暂停
#define MUSIC_STOP "A4"                     // 停止
#define MUSIC_PRE "A5"                      // 上一曲
#define MUSIC_NEXT "A6"                     // 下一曲
#define MUSIC_VOLUME_ADD "B0"               // 音量+
#define MUSIC_VOLUME_SUB "B1"               // 音量-
#define MUSIC_PLAY_MODE_LOOP_ALL "B4:00"    // 全盘循环(00)：按顺序播放全盘曲目,播放完后循环播放
#define MUSIC_PLAY_MODE_LOOP_SINGLE "B4:01" // 单曲循环(01)：一直循环播放当前曲目
#define MUSIC_PLAY_MODE_RANDOM "B4:03"      // 全盘随机(03)：随机播放盘符内曲目

extern u8 Way_Angle;                                                   // 获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波
extern int Motor_Left, Motor_Right;                                    // 电机PWM变量 应是motor的 向moto致敬
extern u8 Flag_front, Flag_back, Flag_Left, Flag_Right, Flag_velocity; // 蓝牙遥控相关的变量
extern u8 Flag_Stop, Flag_Show;                                        // 停止标志位和 显示标志位 默认停止 显示打开
extern int Voltage;                                                    // 电池电压采样相关的变量
extern float Angle_Balance, Gyro_Balance, Gyro_Turn;                   // 平衡倾角 平衡陀螺仪 转向陀螺仪
extern int Temperature;
extern u32 Distance; // 超声波测距
extern u8 Flag_follow, Flag_avoid, delay_50, delay_flag, PID_Send;
extern float Acceleration_Z; // Z轴加速度计
extern float Balance_Kp, Balance_Kd, Velocity_Kp, Velocity_Ki, Turn_Kp, Turn_Kd;
/////////////////////////////////////////////////////////////////
void Stm32_Clock_Init(u8 PLL);                                                                      // 时钟初始化
void Sys_Soft_Reset(void);                                                                          // 系统软复位
void Sys_Standby(void);                                                                             // 待机模式
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);                                          // 设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);                                                    // 设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group); // 设置中断
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);                                                    // 外部中断配置函数(只对GPIOA~G)
void JTAG_Set(u8 mode);
//////////////////////////////////////////////////////////////////////////////
// 以下为汇编函数
void WFI_SET(void);      // 执行WFI指令
void INTX_DISABLE(void); // 关闭所有中断
void INTX_ENABLE(void);  // 开启所有中断
void MSR_MSP(u32 addr);  // 设置堆栈地址

#endif
