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
#include "touch_isr.h"
#include "images.h"

extern void ADCEnable(void);
extern void print_waveform_ch1(int32_t time_step, int index, uint8_t *y_axis);
extern void print_waveform_ch2(int32_t time_step, int index, uint8_t *y_axis);

extern uint32_t byte_count;
extern uint8_t flag_intr;
int32_t prev_print_left = 0;
int32_t prev_print_left_ch2 = 0;
extern uint8_t * data1;  //channel1
extern uint8_t * data2;  //channel2

uint32_t time_interval_step[] = {4000,2000,1000,500,350};
uint32_t time_interval_ptr = 0;
uint32_t print_interval_step[] = {4000,2000,1000,500,350};
uint8_t print_int_ptr = 0;
uint8_t time_print_flag = 100;
uint8_t time_print_flag_ch2 = 100;

int step =1;
extern tContext sContext;
tRectangle Rect_TB1, Rect_TB2, Rect_ch1, Rect_ch2;

RectangularButton(increase, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 0,
                  80, 40, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0,
				  increase_press);
RectangularButton(decrease, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 199,
                  319, 40, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0,
				  decrease_press);

RectangularButton(new, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 99, 0,
                  120, 19, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0,
                  new_press);

RectangularButton(start, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 179,
                  319, 60, PB_STYLE_FILL | PB_STYLE_TEXT, ClrGreenYellow, ClrGreenYellow, 0, ClrBlack,
				  &g_sFontCm20, "START", 0, 0, 0, 0,
				  start_press);
RectangularButton(plus, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 251, 92,
                  70, 70, PB_STYLE_IMG | PB_STYLE_TEXT, ClrBlack, ClrBlack, 0, ClrSilver,
                  &g_sFontCm20, "+", g_pui8Blue50x50, g_pui8Blue50x50Press, 0, 0,
				  plus_press);
RectangularButton(minus, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 90,
                  70, 70, PB_STYLE_IMG | PB_STYLE_TEXT, ClrBlack, ClrBlack, 0,
                  ClrSilver, &g_sFontCm20, "-", g_pui8Blue50x50,
                  g_pui8Blue50x50Press, 0, 0, minus_press);


Slider(move_slider_ch1, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 24, 319, 76, 0, 100, 50,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, move_slider_press_ch1);

Slider(move_slider_ch2, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 120, 319, 196, 0, 100, 50,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, move_slider_press_ch2);


int32_t pre_value = 50;

void new_press(tWidget *psWidget)
{

    time_interval = time_interval_step[time_interval_ptr];
    ADCEnable();

    switch (print_int_ptr){
       case 0: clear_screen_grid();
               print_waveform_ch1( 1, 0, data1);
               print_waveform_ch2( 1, 0, data2);
               break;

       case 1: clear_screen_grid();
               print_waveform_ch1( 2, 0, data1);
               print_waveform_ch2( 2, 0, data2);
               break;

       case 2: clear_screen_grid();
               print_waveform_ch1( 4, 0, data1);
               print_waveform_ch2( 4, 0, data2);
               break;

       case 3: clear_screen_grid();
               print_waveform_ch1( 8, 0, data1);
               print_waveform_ch2( 8, 0, data2);
               break;

       case 4: clear_screen_grid();
               print_waveform_ch1( 12, 0, data1);
               print_waveform_ch2( 12, 0, data2);
               break;
       }
}

void move_slider_press_ch1(tWidget *psWidget, int32_t value)
{

    int32_t diff_value = value - pre_value;
    prev_print_left = print_left;

    if(abs(diff_value) > 3)
    {
        pre_value = value;

        if(diff_value > 0)
        {

            if(print_interval_step[print_int_ptr] == 500)
            {
                print_left = print_left - 10;
                print_right = print_right - 10;
            }
            else if(print_interval_step[print_int_ptr] == 350)
            {
                print_left = print_left - 10;
                print_right = print_right - 10;
            }
            else
            {
                print_left = print_left - 10;
                print_right = print_right - 10;
            }

            if(print_left <= 0)
            {
                print_left = 0;
                print_right = print_left + print_interval_step[print_int_ptr];
            }


        }

        if(diff_value < 0)
        {

            if(print_interval_step[print_int_ptr] == 500)
            {
                print_left = print_left + 10;
                print_right = print_right + 10;
            }
            else if(print_interval_step[print_int_ptr] == 350)
            {
                print_left = print_left + 10;
                print_right = print_right + 10;
            }
            else
            {
                print_left = print_left + 10;
                print_right = print_right + 10;
            }

            if(print_right >= 4000)
            {
                print_right = 4000;
                print_left = print_right - print_interval_step[print_int_ptr];

            }

        }

        Rect_TB1.i16XMin = 0;
        Rect_TB1.i16YMin = 24;
        Rect_TB1.i16XMax = 319;
        Rect_TB1.i16YMax = 121;
        GrContextForegroundSet(&sContext, ClrBlack);
        GrRectFill(&sContext, &Rect_TB1);

        GrContextForegroundSet(&sContext, ClrDimGray);
        GrLineDrawV(&sContext, 159, 20, 121);
        GrLineDrawV(&sContext, 79, 20, 121);
        GrLineDrawV(&sContext, 239, 20, 121);

        GrLineDrawH(&sContext, 0, 319, 74);
        GrLineDrawH(&sContext, 0, 319, 129);

        Rect_ch1.i16XMin = 0;
        Rect_ch1.i16YMin = 20;
        Rect_ch1.i16XMax = 10;
        Rect_ch1.i16YMax = 30;
        GrContextForegroundSet(&sContext, ClrCyan);
        GrRectFill(&sContext, &Rect_ch1);
        GrContextForegroundSet(&sContext, ClrDarkBlue);
        GrContextFontSet(&sContext, &g_sFontCm14);
        GrStringDrawCentered(&sContext, "1", -1,5, 25 , 0);

        switch (step) {
        case 1: if ( print_left > 3775)
                print_left = 3775;
                break;

        case 2:if ( print_left > 160)
               print_left = 160;
               break;

        case 4:if ( print_left > 240)
               print_left = 240;
               break;

        case 8:if ( print_left > 280)
               print_left = 280;
               break;
        }

        if (print_left <0)
            print_left = 0;

        print_waveform_ch1( step, print_left, data1);
        time_print_flag = 1;
    }
}


int32_t pre_value_ch2 = 50;

void move_slider_press_ch2(tWidget *psWidget, int32_t value)
{
    int32_t diff_value1 = value - pre_value_ch2;
    prev_print_left_ch2 = print_left_ch2;
    if(abs(diff_value1) > 3)
    {
        pre_value_ch2 = value;

        if(diff_value1 > 0)
        {


            if(print_interval_step[print_int_ptr] == 500)
            {
                print_left_ch2 = print_left_ch2 - 10;
                print_right_ch2 = print_right_ch2 - 10;
            }
            else if(print_interval_step[print_int_ptr] == 350)
            {
                print_left_ch2 = print_left_ch2 - 10;
                print_right_ch2 = print_right_ch2 - 10;
            }
            else
            {
                print_left_ch2 = print_left_ch2 - 10;
                print_right_ch2 = print_right_ch2 - 10;
            }


        }

        if(diff_value1 < 0)
        {

            if(print_interval_step[print_int_ptr] == 500)
            {
                print_left_ch2 = print_left_ch2 + 10;
                print_right_ch2 = print_right_ch2 + 10;
            }
            else if(print_interval_step[print_int_ptr] == 350)
            {
                print_left_ch2 = print_left_ch2 + 10;
                print_right_ch2 = print_right_ch2 + 10 ;
            }
            else
            {
                print_left_ch2 = print_left_ch2 + 10;
                print_right_ch2 = print_right_ch2 + 10;
            }

            if(print_right >= 4000)
            {
                print_right_ch2 = 4000;
                print_left_ch2 = print_right_ch2 - print_interval_step[print_int_ptr];
            }
        }


        Rect_TB2.i16XMin = 0;
        Rect_TB2.i16YMin = 121;
        Rect_TB2.i16XMax = 319;
        Rect_TB2.i16YMax = 220;
        GrContextForegroundSet(&sContext, ClrBlack);
        GrRectFill(&sContext, &Rect_TB2);

        GrContextForegroundSet(&sContext, ClrDimGray);
        GrLineDrawV(&sContext, 159, 121, 239);
        GrLineDrawV(&sContext, 79, 121, 239);
        GrLineDrawV(&sContext, 239, 121, 239);

        GrLineDrawH(&sContext, 0, 319, 184);
        GrLineDrawH(&sContext, 0, 319, 239);

        Rect_ch2.i16XMin = 0;
        Rect_ch2.i16YMin = 131;
            Rect_ch2.i16XMax = 10;
            Rect_ch2.i16YMax = 141;
            GrContextForegroundSet(&sContext, ClrYellow);
            GrRectFill(&sContext, &Rect_ch2);
            GrContextForegroundSet(&sContext, ClrDarkBlue);
            GrContextFontSet(&sContext, &g_sFontCm14);
            GrStringDrawCentered(&sContext, "2", -1,5, 136 , 0);

            switch (step) {
            case 1: if ( print_left_ch2 > 0)
                print_left_ch2 = 0;
                    break;

            case 2:if ( print_left_ch2 > 160)
                print_left_ch2 = 160;
                   break;

            case 4:if ( print_left_ch2 > 240)
                print_left_ch2 = 240;
                   break;

            case 8:if ( print_left_ch2 > 280)
                print_left_ch2 = 280;
                   break;
            }
//
            if (print_left_ch2 <0)
                print_left_ch2 = 0;

        print_waveform_ch2( step, print_left_ch2, data2);
        time_print_flag_ch2 = 1;
    }
}



void start_press(tWidget *psWidget)
{


	time_interval = time_interval_step[time_interval_ptr];


	WidgetRemove((tWidget *)&start);
	WidgetRemove((tWidget *)&minus);
	WidgetRemove((tWidget *)&plus);
	ADCEnable();


	switch (print_int_ptr){
       case 0: clear_screen_grid();
               print_waveform_ch1( 1, 0, data1);
               print_waveform_ch2( 1, 0, data2);
               break;

       case 1: clear_screen_grid();
               print_waveform_ch1( 2, 0, data1);
               print_waveform_ch2( 2, 0, data2);
               break;

       case 2: clear_screen_grid();
               print_waveform_ch1( 4, 0, data1);
               print_waveform_ch2( 4, 0, data2);
               break;

       case 3: clear_screen_grid();
               print_waveform_ch1( 8, 0, data1);
               print_waveform_ch2( 8, 0, data2);
               break;

       case 4: clear_screen_grid();
               print_waveform_ch1( 12, 0, data1);
               print_waveform_ch2( 12, 0, data2);
               break;
       }
//
//	time_print_flag = 1;
//	time_print_flag_ch2 = 1;
	WidgetAdd(WIDGET_ROOT, (tWidget *)&increase);
	WidgetAdd(WIDGET_ROOT, (tWidget *)&decrease);
	WidgetAdd(WIDGET_ROOT, (tWidget *)&new);
	WidgetAdd(WIDGET_ROOT, (tWidget *)&move_slider_ch1);
	WidgetAdd(WIDGET_ROOT, (tWidget *)&move_slider_ch2);
}

void plus_press(tWidget *psWidget)
{
    if(time_interval_ptr == 0)
        time_interval_ptr = 0;
    else
        time_interval_ptr--;

    if(print_int_ptr == 0)
          print_int_ptr = 0;
      else
          print_int_ptr--;

    usprintf(str_time, "%3dus",time_interval_step[time_interval_ptr]);
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &Rect_t2);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextFontSet(&sContext, &g_sFontCm14);
    GrStringDrawCentered(&sContext, str_time, -1, 159, 99, 0);

}

void minus_press(tWidget *psWidget)
{
    time_interval_ptr++;
        if(time_interval_ptr > 4)
            time_interval_ptr = 4;

        print_int_ptr++;
            if(print_int_ptr > 4)
                   print_int_ptr =4 ;

        usprintf(str_time, "%3dus",time_interval_step[time_interval_ptr]);
        GrContextForegroundSet(&sContext, ClrBlack);
        GrRectFill(&sContext, &Rect_t2);
        GrContextForegroundSet(&sContext, ClrWhite);
        GrContextFontSet(&sContext, &g_sFontCm14);
        GrStringDrawCentered(&sContext, str_time, -1, 159, 99, 0);
}




void increase_press(tWidget *psWidget)
{
    print_int_ptr++;
     if(print_int_ptr > 4)
            print_int_ptr =4 ;
    switch (print_int_ptr){
    case 0: clear_screen_grid();
            print_waveform_ch1( 1, 0, data1);
            print_waveform_ch2( 1, 0, data2);
            step = 1;
            break;

    case 1: clear_screen_grid();
            print_waveform_ch1( 2, 0, data1);
            print_waveform_ch2( 2, 0, data2);
            step = 2;
            break;

    case 2: clear_screen_grid();
            print_waveform_ch1( 4, 0, data1);
            print_waveform_ch2( 4, 0, data2);
            step = 4;
            break;

    case 3: clear_screen_grid();
            print_waveform_ch1( 8, 0, data1);
            print_waveform_ch2( 8, 0, data2);
            step = 8;
            break;

    case 4: clear_screen_grid();
            print_waveform_ch1( 12, 0, data1);
            print_waveform_ch2( 12, 0, data2);
            step = 12;
            break;
    }
    print_left = 0;
    print_left_ch2 = 0;
    time_print_flag = 1;
    time_print_flag_ch2 = 1;
}
void decrease_press(tWidget *psWidget)
{
    if(print_int_ptr == 0)
        print_int_ptr = 0;
    else
        print_int_ptr--;

    switch (print_int_ptr){
    case 0: clear_screen_grid();
            print_waveform_ch1( 1, 0, data1);
            print_waveform_ch2( 1, 0, data2);
            step = 1;
            break;

    case 1: clear_screen_grid();
            print_waveform_ch1( 2, 0, data1);
            print_waveform_ch2( 2, 0, data2);
            step = 2;
            break;

    case 2: clear_screen_grid();
            print_waveform_ch1( 4, 0, data1);
            print_waveform_ch2( 4, 0, data2);
            step = 4;
            break;

    case 3: clear_screen_grid();
            print_waveform_ch1( 8, 0, data1);
            print_waveform_ch2( 8, 0, data2);
            step = 8;
            break;

    case 4: clear_screen_grid();
            print_waveform_ch1( 12, 0, data1);
            print_waveform_ch2( 12, 0, data2);
            step = 12;
            break;
    }

    print_left = 0;
    print_left_ch2 = 0;
    time_print_flag = 1;
    time_print_flag_ch2 = 1;
}




