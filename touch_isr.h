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



#ifndef __TOUCHISR_H__
#define __TOUCHISR_H__

extern tPushButtonWidget wave_print;
extern tPushButtonWidget increase;
extern tPushButtonWidget decrease;
extern tPushButtonWidget new;
extern tPushButtonWidget left;
extern tPushButtonWidget right;
extern tPushButtonWidget start;
extern tPushButtonWidget plus;
extern tPushButtonWidget minus;

extern uint32_t time_interval_step[];
extern uint32_t time_interval_ptr;
extern uint32_t print_interval_step[];
extern uint8_t print_int_ptr;
extern uint8_t time_print_flag;
extern uint8_t time_print_flag_ch2;

extern uint8_t SUT1_data,SUT2_data,SUT3_data,SUT4_data;

void move_slider_press_ch1(tWidget *psWidget, int32_t i32Value);
void move_slider_press_ch2(tWidget *psWidget, int32_t i32Value);
void start_press(tWidget *psWidget);

void increase_press(tWidget *psWidget);
void decrease_press(tWidget *psWidget);
void new_press(tWidget *psWidget);

void plus_press(tWidget *psWidget);
void minus_press(tWidget *psWidget);

#endif // __TOUCHISR_H__
