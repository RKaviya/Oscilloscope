#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


#ifndef UART_H_
#define UART_H_

extern float temp_set;

void UART_init(void);
void UARTIntHandler(void);
void UART_string(char string[]);
void UART_integer_digit (unsigned long int value, int digits);
void UART_integer (unsigned long int value);
void UART_newline(void);
void UART_float(double number,int digits);
void UART_Htab(void);
int UARTwrite(const char *pcBuf, uint32_t ui32Len);
void UARTprintf(const char *pcString, ...);
void UARTvprintf(const char *pcString, va_list vaArgP);
void UART_get_string(char *s);


#endif /* UART_H_ */
