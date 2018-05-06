/*
 * diaplya_lcd.c
 *
 *  Created on: 01-May-2018
 *      Author: kaviya
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "inc/tm4c123gh6pm.h"
#include "Kentec320x240x16_ssd2119_spi.h"
#include "driverlib/sysctl.h"
#include "driverlib/fpu.h"
#include "LCD.h"
#include "grlib/grlib.h"
extern const tDisplay g_sKentec320x240x16_SSD2119;
tRectangle sRect,Rect_M,Rect_L,Rect_R,Rect_t1,Rect_t2;
tContext sContext;

int main (void){
    int32_t x_axis[]={0,10, 20, 30 ,40, 50, 60, 70, 80,90, 100, 110, 120, 130};
//    int32_t y_axis[10], y_axis1[]={4095, 3754, 3413, 3072, 2731,2390, 2049,1026, 344, 000};
    int32_t y_axis[12], y_axis1[]={2049, 2731, 3072, 3413, 3754, 4095,3754, 3413, 3072, 2731, 2049};
    float tt;
//    int32_t y_axis[]={0,10 ,20 30, 40, 50}; //{10,30,9,13,18,24,28};{ 120,  110, 96, 70, 55, 40};
    int i;
   FPUEnable();
   FPUStackingDisable();
   SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
   GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_DIR_MODE_IN);
   GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
   prev_init();
//   IntMasterEnable();
   ui32SysClock = SysCtlClockGet();
   Kentec320x240x16_SSD2119Init(ui32SysClock);
   GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);

//   Rect_M.i16XMin = 99;
//   Rect_M.i16YMin = 0;
//   Rect_M.i16XMax = 219;
//   Rect_M.i16YMax = 19;
   GrContextForegroundSet(&sContext, ClrCyan);
//   GrRectFill(&sContext, &Rect_M);

//   extern void GrLineDraw(const tContext *psContext, int32_t i32X1, int32_t i32Y1,
//                          int32_t i32X2, int32_t i32Y2);
   tt = (float)96/ (float)4095;
   for (i=0;i <12; i++){
//   y_axis[i] = 120-(96*(y_axis1[i]/4095));
       y_axis[i] = (float)120-((tt)*((float)y_axis1[i]));
       printf("%d \n",y_axis[i]);
   }

   GrLineDraw(&sContext,x_axis[0], y_axis[0], x_axis[1], y_axis[1] );
   GrLineDraw(&sContext,x_axis[1], y_axis[1], x_axis[2], y_axis[2] );
   GrLineDraw(&sContext,x_axis[2], y_axis[2], x_axis[3], y_axis[3] );
   GrLineDraw(&sContext,x_axis[3], y_axis[3], x_axis[4], y_axis[4] );
   GrLineDraw(&sContext,x_axis[4], y_axis[4], x_axis[5], y_axis[5] );
   GrLineDraw(&sContext,x_axis[5], y_axis[5], x_axis[6], y_axis[6] );
   GrLineDraw(&sContext,x_axis[6], y_axis[6], x_axis[7], y_axis[7] );
   GrLineDraw(&sContext,x_axis[7], y_axis[7], x_axis[8], y_axis[8] );
   GrLineDraw(&sContext,x_axis[8], y_axis[8], x_axis[9], y_axis[9] );
   GrLineDraw(&sContext,x_axis[9], y_axis[9], x_axis[10], y_axis[10] );
//   GrLineDraw(&sContext,x_axis[10], y_axis[10], x_axis[11], y_axis[11] );
//   GrLineDraw(&sContext,x_axis[11], y_axis[11], x_axis[12], y_axis[11] );
//   GrLineDraw(&sContext,x_axis[11], y_axis[12], x_axis[13], y_axis[11] );

   while(1){

   }
}


