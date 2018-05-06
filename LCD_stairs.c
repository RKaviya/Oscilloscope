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
#include "touch.h"
#include "touch_isr.h"

#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

tContext sContext;

tRectangle Rect_M,Rect_L,Rect_R;

extern const tDisplay g_sKentec320x240x16_SSD2119;
extern uint8_t SUT1_data,SUT2_data;
uint8_t * data1 = &SUT1_data;  //channel1
uint8_t * data2 = &SUT2_data;  //channel2

void ADCgetdata(void)
{
      uint32_t bytes=0;
      uint32_t pui32ADC1Value[1];
      ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
      ADCSequenceEnable(ADC1_BASE, 3);
      ADCIntClear(ADC1_BASE, 3);
      while(bytes<=4096)
          {

              ADCProcessorTrigger(ADC1_BASE, 3);
              while(!ADCIntStatus(ADC1_BASE, 3, false));
              ADCIntClear(ADC1_BASE, 3);
              ADCSequenceDataGet(ADC1_BASE, 3, pui32ADC1Value);
              data1[bytes]=(uint8_t)(pui32ADC1Value[0]>>4);
              bytes++;
          }

      bytes=0;
      ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
      ADCSequenceEnable(ADC1_BASE, 3);
      ADCIntClear(ADC1_BASE, 3);
      while(bytes<=4096)
          {

              ADCProcessorTrigger(ADC1_BASE, 3);
              while(!ADCIntStatus(ADC1_BASE, 3, false));
              ADCIntClear(ADC1_BASE, 3);
              ADCSequenceDataGet(ADC1_BASE, 3, pui32ADC1Value);
              data2[bytes]=(uint8_t)(pui32ADC1Value[0]>>4);
              bytes++;
          }
}
void ADCEnable(void)
{
       SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
       GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
       ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
       ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |ADC_CTL_END);
       ADCSequenceEnable(ADC1_BASE, 3);
       ADCIntClear(ADC1_BASE, 3);
       ADCgetdata();

}




int main (void){

    FPUEnable();
    FPUStackingDisable();
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ui32SysClock = SysCtlClockGet();
    Kentec320x240x16_SSD2119Init(ui32SysClock);
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);
    TouchScreenInit(ui32SysClock);
    TouchScreenCallbackSet(WidgetPointerMessage);

    Rect_M.i16XMin = 99;
    Rect_M.i16YMin = 0;
    Rect_M.i16XMax = 219;
    Rect_M.i16YMax = 19;
    GrContextForegroundSet(&sContext, ClrGreenYellow);
    GrRectFill(&sContext, &Rect_M);

    Rect_L.i16XMin = 0;
    Rect_L.i16YMin = 0;
    Rect_L.i16XMax = 99;
    Rect_L.i16YMax = 19;
    GrContextForegroundSet(&sContext, ClrGreenYellow);
    GrRectFill(&sContext, &Rect_L);

    Rect_R.i16XMin = 219;
    Rect_R.i16YMin = 0;
    Rect_R.i16XMax = 319;
    Rect_R.i16YMax = 19;
    GrContextForegroundSet(&sContext, ClrGreenYellow);
    GrRectFill(&sContext, &Rect_R);

    GrContextForegroundSet(&sContext, ClrBlack);
    GrContextFontSet(&sContext, &g_sFontCm16);
    GrStringDrawCentered(&sContext, "Oscilloscope", -1,159, 7, 0);


    Rect_t1.i16XMin = 99;
    Rect_t1.i16YMin = 29;
    Rect_t1.i16XMax = 219;
    Rect_t1.i16YMax = 89;

    Rect_t2.i16XMin = 99;
    Rect_t2.i16YMin = 89;
    Rect_t2.i16XMax = 219;
    Rect_t2.i16YMax = 109;

    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm14);
    GrStringDrawCentered(&sContext, "Time Period", -1,159, 55 , 0);

    usprintf(str_time, "%3dus",time_interval_step[time_interval_ptr]);
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &Rect_t2);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm14);
    GrStringDrawCentered(&sContext, str_time, -1, 159, 99, 0);

    WidgetAdd(WIDGET_ROOT, (tWidget *)&plus);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&minus);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&start);

    WidgetPaint((tWidget *)&plus);
    WidgetPaint((tWidget *)&minus);
    WidgetPaint((tWidget *)&start);

   while(1){
       WidgetMessageQueueProcess();
       if(time_print_flag == 1)
        {
            time_print_flag = 0;
            print_time();

        }
       if (time_print_flag_ch2 ==1){
           time_print_flag_ch2 = 0;
           print_time_ch2();
       }
   }
}





