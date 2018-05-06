#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "utils/ustdlib.h"
#include "Kentec320x240x16_ssd2119_spi.h"
#include "touch.h"
#include "utils/ustdlib.h"

#include "UART.h"
#include "LCD.h"

tContext sContext;
extern tRectangle Rect_M,Rect_L,Rect_R;
tRectangle sRect,Rect_t1,Rect_t2, RectL_ch2, RectR_ch2;
uint32_t ui32SysClock;


int32_t print_left = 0;
int32_t print_left_ch2 = 0;
int32_t print_right = 320;
int32_t print_right_ch2 = 320;
char str_left[10];
char str_right[10];
char str_time[10];
float prev_time_left = 0.0;
float prev_time_left_2 = 0.0;
int32_t prev_print_left;
int32_t diff;
extern uint32_t print_interval_step[];
extern int step;
extern uint32_t time_interval_step;
extern uint32_t time_interval_ptr;
int prev_step = 1;
int prev_step_ch2 = 1;
extern uint8_t print_int_ptr;
uint32_t time_interval = 320;

extern tContext sContext;
extern tRectangle  Rect_ch1, Rect_ch2;

tRectangle Rect_vpp_ch1,Rect_f_ch1;
tRectangle Rect_vpp_ch2,Rect_f_ch2;

extern uint8_t * data1;  //channel1
extern uint8_t * data2;  //channel2

int max_index = 0;
int min_index = 0;

void print_time(void)
{
    float time_left = 0, time_right=0;
    time_interval = print_interval_step[print_int_ptr];

    time_left = print_left;
    time_right = print_left + time_interval;

    int32_t int_part, frac_part;

    int_part = (int32_t) time_left;
    frac_part = (int32_t) (time_left * 1000.0f);
    frac_part = frac_part - (int_part * 1000);
    if(frac_part < 0)
        frac_part *= -1;

    Rect_L.i16XMin = 0;
    Rect_L.i16YMin = 0;
    Rect_L.i16XMax = 99;
    Rect_L.i16YMax = 19;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &Rect_L);

    Rect_R.i16XMin = 219;
    Rect_R.i16YMin = 0;
    Rect_R.i16XMax = 319;
    Rect_R.i16YMax = 19;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &Rect_R);

    usprintf(str_left, "%3d.%03dus", int_part, frac_part);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &Rect_L);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm12);
    GrStringDrawCentered(&sContext, str_left, -1,39, 9, 0);

    int_part = (int32_t) time_right;
    frac_part = (int32_t) (time_right * 1000.0f);
    frac_part = frac_part - (int_part * 1000);
    if(frac_part < 0)
        frac_part *= -1;

    usprintf(str_right, "%3d.%03dus", int_part, frac_part);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &Rect_R);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm12);
    GrStringDrawCentered(&sContext, str_right, -1,279, 9, 0);

    prev_time_left = time_left;
    prev_step = step;

}


//////////////////////////
void print_time_ch2(void)
{
    float time_left =0, time_right =0;
    time_interval = print_interval_step[print_int_ptr];

    time_left = print_left_ch2;
    time_right = print_left_ch2 + time_interval;

    int32_t int_part, frac_part;

    int_part = (int32_t) time_left;
    frac_part = (int32_t) (time_left * 1000.0f);
    frac_part = frac_part - (int_part * 1000);
    if(frac_part < 0)
        frac_part *= -1;

    RectL_ch2.i16XMin = 0;
    RectL_ch2.i16YMin = 220;
    RectL_ch2.i16XMax = 99;
    RectL_ch2.i16YMax = 239;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &RectL_ch2);

    RectR_ch2.i16XMin = 219;
    RectR_ch2.i16YMin = 220;
    RectR_ch2.i16XMax = 319;
    RectR_ch2.i16YMax = 239;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &RectR_ch2);

    usprintf(str_left, "%3d.%03dus", int_part, frac_part);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &RectL_ch2);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm12);
    GrStringDrawCentered(&sContext, str_left, -1,39, 229, 0);

    int_part = (int32_t) time_right;
    frac_part = (int32_t) (time_right * 1000.0f);
    frac_part = frac_part - (int_part * 1000);
    if(frac_part < 0)
        frac_part *= -1;

    usprintf(str_right, "%3d.%03dus", int_part, frac_part);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &RectR_ch2);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm12);
    GrStringDrawCentered(&sContext, str_right, -1,279, 229, 0);

    prev_time_left_2 = time_left;
    prev_step_ch2 = step;
}

void clear_screen_grid(void)
{
    sRect.i16XMin = 0;
    sRect.i16YMin = 20;
    sRect.i16XMax = 319;
    sRect.i16YMax = 239;
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &sRect);

    GrContextForegroundSet(&sContext, ClrDimGray);
    GrLineDrawV(&sContext, 159, 20, 239);
    GrLineDrawV(&sContext, 79, 20, 239);
    GrLineDrawV(&sContext, 239, 20, 239);

    GrLineDrawH(&sContext, 0, 319, 74);
    GrLineDrawH(&sContext, 0, 319, 129);
    GrLineDrawH(&sContext, 0, 319, 184);
    GrLineDrawH(&sContext, 0, 319, 239);

    Rect_ch1.i16XMin = 0;
    Rect_ch1.i16YMin = 20;
    Rect_ch1.i16XMax = 10;
    Rect_ch1.i16YMax = 30;
    GrContextForegroundSet(&sContext, ClrCyan);
    GrRectFill(&sContext, &Rect_ch1);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrContextFontSet(&sContext, &g_sFontCm14);
    GrStringDrawCentered(&sContext, "1", -1,5, 25 , 0);

    Rect_ch2.i16XMin = 0;
    Rect_ch2.i16YMin = 131;
    Rect_ch2.i16XMax = 10;
    Rect_ch2.i16YMax = 141;
    GrContextForegroundSet(&sContext, ClrYellow);
    GrRectFill(&sContext, &Rect_ch2);
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrContextFontSet(&sContext, &g_sFontCm14);
    GrStringDrawCentered(&sContext, "2", -1,5, 136 , 0);
}


void print_waveform1_ch1(int32_t time_step, uint8_t *y_axis)
{
        float scale_factor;
        int i;
        uint32_t  t = 0, y_origin = 120, y_axis1[320];
//        uint8_t y_extract[320];

        y_axis1[0] = y_axis[0];
        scale_factor = (float)72/ (float)255;

        y_axis1[0] = (float)y_origin-((scale_factor)*((float)y_axis1[0]));

        GrContextForegroundSet(&sContext, ClrCyan);

            for (i= 1; i < 319; i++){
                t = t+time_step;
                y_axis1[i] = y_axis[i];
                y_axis1[i] = (float)y_origin-((scale_factor)*((float)y_axis1[i]));

                GrLineDrawV(&sContext, t, y_axis1[i-1],y_axis1[i]);
                GrLineDrawH(&sContext, t, t+time_step,y_axis1[i]);
                }
            t = t+time_step;



}


void print_waveform_ch1(int32_t time_step, int index, uint8_t *y_axis){
        float scale_factor;
        int i, length = 0;
        uint32_t  t = 0, y_origin = 120, y_axis1[640];
        uint8_t y_extract[640];

        y_extract[index] =y_axis[index];
        y_axis1[index] =y_extract[index];

        i = index;
        scale_factor = (float)72/ (float)255;

        length = (320)/time_step;

        y_axis1[index] = (float)y_origin-((scale_factor)*((float)y_axis1[index]));

        GrContextForegroundSet(&sContext, ClrCyan);
        GrLineDrawH(&sContext, t, t+time_step,y_axis1[index]);

            for (i=index+1; i < index+length; i++){
                t = t+time_step;
                y_axis1[i] = y_axis[i];
                y_axis1[i] = (float)y_origin-((scale_factor)*((float)y_axis1[i]));

                GrLineDrawV(&sContext, t, y_axis1[i-1],y_axis1[i]);
                GrLineDrawH(&sContext, t, t+time_step,y_axis1[i]);
                }
            t = t+time_step;

            int peak_topeak =0, freq =0;
            peak_topeak = peak_to_peak (y_axis);
            freq= frequency (y_axis);

            int h = 0,tens = 0,u = 0;
            u = peak_topeak%10;
            peak_topeak = peak_topeak/10;
            tens = peak_topeak%10;
            peak_topeak = peak_topeak/10;
            h = peak_topeak%10;
            peak_topeak = peak_topeak/10;


            usprintf(str_left, "Vp-p : %d.%d%dV", h,tens,u);

            Rect_vpp_ch1.i16XMin = 250;
            Rect_vpp_ch1.i16YMin = 20;
            Rect_vpp_ch1.i16XMax = 319;
            Rect_vpp_ch1.i16YMax = 35;

            GrContextForegroundSet(&sContext, ClrDarkBlue);
            GrRectFill(&sContext, &Rect_vpp_ch1);
            GrContextForegroundSet(&sContext, ClrWhite);
            GrContextFontSet(&sContext, &g_sFontCm12);
            GrStringDrawCentered(&sContext, str_left, -1,280,25 , 0);

            usprintf(str_left, "T : %3dus", freq);  //// needs to be changed
            Rect_f_ch1.i16XMin = 250;
            Rect_f_ch1.i16YMin = 35;
            Rect_f_ch1.i16XMax = 319;
            Rect_f_ch1.i16YMax = 50;

            GrContextForegroundSet(&sContext, ClrDarkBlue);
            GrRectFill(&sContext, &Rect_f_ch1);
            GrContextForegroundSet(&sContext, ClrWhite);
            GrContextFontSet(&sContext, &g_sFontCm12);
            GrStringDrawCentered(&sContext, str_left, -1,285,42 , 0);

//            peak_topeak = 0, freq =0;
//            peak_topeak = peak_to_peak (data2);
//            freq= frequency (data2);
//            h = 0,tens = 0,u = 0;
//            u = peak_topeak%10;
//            peak_topeak = peak_topeak/10;
//            tens = peak_topeak%10;
//            peak_topeak = peak_topeak/10;
//            h = peak_topeak%10;
//            peak_topeak = peak_topeak/10;
//
//            usprintf(str_left, "Vp-p : %d.%d%dV", h,tens,u);
//
//            Rect_vpp_ch2.i16XMin = 250;
//            Rect_vpp_ch2.i16YMin = 121;
//            Rect_vpp_ch2.i16XMax = 319;
//            Rect_vpp_ch2.i16YMax = 136;
//
//            GrContextForegroundSet(&sContext, ClrDarkBlue);
//            GrRectFill(&sContext, &Rect_vpp_ch2);
//            GrContextForegroundSet(&sContext, ClrWhite);
//            GrContextFontSet(&sContext, &g_sFontCm12);
//            GrStringDrawCentered(&sContext, str_left, -1,285,126 , 0);
//
//            usprintf(str_left, "T : %3dus", freq);  //// needs to be changed
//            Rect_f_ch2.i16XMin = 250;
//            Rect_f_ch2.i16YMin = 136;
//            Rect_f_ch2.i16XMax = 319;
//            Rect_f_ch2.i16YMax = 151;
//
//            GrContextForegroundSet(&sContext, ClrDarkBlue);
//            GrRectFill(&sContext, &Rect_f_ch2);
//            GrContextForegroundSet(&sContext, ClrWhite);
//            GrContextFontSet(&sContext, &g_sFontCm12);
//            GrStringDrawCentered(&sContext, str_left, -1,285,143 , 0);
}

void print_waveform_ch2(int32_t time_step, int index, uint8_t *y_axis){
        float scale_factor;
        int i, length = 0;
        uint32_t  t = 0, y_origin = 216,y_axis1[640];
        y_axis1[index] = y_axis[index];

        i = index;
        scale_factor = (float)72/ (float)255;
        length = 320/time_step;

        y_axis1[index] = (float)y_origin-((scale_factor)*((float)y_axis1[index]));


        GrContextForegroundSet(&sContext, ClrYellow);

        GrLineDrawH(&sContext, t, t+time_step,y_axis1[index]);

            for (i=index+1; i < index+length; i++){
                t = t+time_step;
                y_axis1[i] = y_axis[i];
                y_axis1[i] = (float)y_origin-((scale_factor)*((float)y_axis1[i]));

                GrLineDrawV(&sContext, t, y_axis1[i-1],y_axis1[i]);
                GrLineDrawH(&sContext, t, t+time_step,y_axis1[i]);
                }
            t = t+time_step;

         int peak_topeak = 0, freq =0;
         peak_topeak = peak_to_peak (y_axis);
         freq= frequency (y_axis);

         int h = 0,tens = 0,u = 0;
         u = peak_topeak % 10;
         peak_topeak = peak_topeak/10;
         tens = peak_topeak%10;
         peak_topeak = peak_topeak/10;
         h = peak_topeak%10;
         peak_topeak = peak_topeak/10;

         usprintf(str_left, "Vp-p : %d.%d%dV", h,tens,u);

         Rect_vpp_ch2.i16XMin = 250;
         Rect_vpp_ch2.i16YMin = 121;
         Rect_vpp_ch2.i16XMax = 319;
         Rect_vpp_ch2.i16YMax = 136;

         GrContextForegroundSet(&sContext, ClrDarkBlue);
         GrRectFill(&sContext, &Rect_vpp_ch2);
         GrContextForegroundSet(&sContext, ClrWhite);
         GrContextFontSet(&sContext, &g_sFontCm12);
         GrStringDrawCentered(&sContext, str_left, -1,285,126 , 0);

         usprintf(str_left, "T : %3dus", freq);  //// needs to be changed
         Rect_f_ch2.i16XMin = 250;
         Rect_f_ch2.i16YMin = 136;
         Rect_f_ch2.i16XMax = 319;
         Rect_f_ch2.i16YMax = 151;

         GrContextForegroundSet(&sContext, ClrDarkBlue);
         GrRectFill(&sContext, &Rect_f_ch2);
         GrContextForegroundSet(&sContext, ClrWhite);
         GrContextFontSet(&sContext, &g_sFontCm12);
         GrStringDrawCentered(&sContext, str_left, -1,285,143 , 0);

}



int peak_to_peak ( uint8_t * data){
    uint32_t max = 0;
    uint32_t min =0;
    int i =0, j= 0;
    int32_t res1 = 0;
    uint32_t res = 0;
    min_index = 0;
    min_index = 0;
    max = data[0];

    for (i = 1; i< 320; i++){
        if (data[i] > max)
        {
            max = data[i];
            max_index = i;
        }
    }
    min = data[0];
    for (i =1; i< 320; i++){
        if (data[i] < min)
            {
            min = data[i];
            min_index = i;
            }
    }

    res1 = max - min;
    res = res1 * 330;

    while (res >= 255)
    {
    res = res -255;
    j++;
    }

    return j;
}

int frequency (uint8_t * data){

    int i =0, j= 0;

    i = max_index;
    while(data[i] - data[min_index] > 3)
    {
        i++;
//        printf("data %d  error %d \n", data[i],data[i] - data[min_index] );
     }
    j = (i - max_index)*9;
    max_index = 0;
    min_index = 0;
    return j;

       }
