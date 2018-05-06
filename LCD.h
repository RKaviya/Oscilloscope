#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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



#ifndef __LCD_H__
#define __LCD_H__


extern tContext sContext;
extern tRectangle sRect,Rect_M,Rect_L,Rect_R,Rect_t1,Rect_t2;
extern uint32_t ui32SysClock;

extern uint8_t data_ch[4][40];
extern uint8_t data_ch_prev[4][40];

extern uint8_t Y_max[4];
extern uint8_t Y_min[4];

extern int32_t print_left;
extern int32_t print_right;
extern int32_t print_left_ch2;
extern int32_t print_right_ch2;
extern char str_left[10];
extern char str_right[10];
extern char str_time[10];


extern int32_t prev_print_left, diff;
extern uint32_t time_interval;

void print_time(void);
void print_time_ch2(void);

void clear_screen_grid(void);
void print_waveform_ch1(int32_t time_step, int index, uint8_t *y_axis);
void print_waveform_ch1(int32_t time_step, int index, uint8_t *y_axis);
void print_waveform1_ch1(int32_t time_step, uint8_t *y_axis);
void print_waveform_ch1_black(int32_t time_step, int index, uint8_t *y_axis);
void print_waveform_ch1_black(int32_t time_step, int index, uint8_t *y_axis);
void prev_init(void);
int peak_to_peak ( uint8_t *);
int frequency (uint8_t *);


#endif // __LCD_H__
