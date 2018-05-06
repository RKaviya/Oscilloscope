

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/inc/hw_ints.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/inc/hw_memmap.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/inc/hw_types.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/gpio.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/interrupt.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/pin_map.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/sysctl.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/uart.h"
#include "/home/kaviya/ti/TivaWare_C_Series-2.1.4.178/driverlib/pwm.h"


#include "UART.h"



#ifndef UART_C_
#define UART_C_

static const char * const g_pcHex = "0123456789abcdef";
float temp_set = 3.3;
bool get_string_flag = false;
char string[100];
uint8_t i = 0;

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    char ch;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status

    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
    	ch = UARTCharGetNonBlocking(UART0_BASE);

    	if(ch == 13)
    	{
    		UARTCharPutNonBlocking(UART0_BASE, 10);
    		get_string_flag = false;
    	}
		UARTCharPutNonBlocking(UART0_BASE, ch); //echo character

		if(ch == 8)
		{
			if(i > 0)
			{
				i--;
			}
			continue;
		}

		if(get_string_flag == true)
		{
			string[i++] = ch;
		}

    }
}

void UART_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

	UARTIntRegister(UART0_BASE, UARTIntHandler);
}


void UART_get_string(char *s)
{
	get_string_flag = true;

	while(get_string_flag == true);

	string[i] = '\0';
	i = 0;
	//mystrcpy(s,string);
}


void UART_string(char string[])
{
	while(*(string)!= '\0')
	UARTCharPut(UART0_BASE, *(string++));
}

void UART_integer_digit (unsigned long int value, int digits)
{
	char buffer[20],l = 0;
	__itoa(value,buffer,10);
	l = strlen(buffer);
	while (l < digits--)
	{
		UARTCharPut(UART0_BASE, '0');
	}
	UART_string(buffer);
}

void UART_integer (unsigned long int value)
{
	char buffer[20] ;
	__itoa(value,buffer,10);

	UART_string(buffer);
}

void UART_newline(void)
{
	UARTCharPut(UART0_BASE, 10);
	UARTCharPut(UART0_BASE, 13);
}

void UART_Htab(void)
{
	UARTCharPut(UART0_BASE, 9);
}

void UART_float(double number,int digits)
{
	// Handle negative numbers
	if (number < 0.0)
	{
		UARTCharPut(UART0_BASE, '-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	long int_part = (long)number;
	double remainder = number - (double)int_part;

	UART_integer((long)int_part);

	// CLCD the decimal point, but only if there are digits beyond
	if (digits > 0) {
		UARTCharPut(UART0_BASE, '.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		int toCLCD = (int)(remainder);

		UART_integer(toCLCD);
		remainder -= toCLCD;
	}

}







//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ui32Len is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ui32Len parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ui32Len parameter to stop transmitting the string, if
//! a null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
int UARTwrite(const char *pcBuf, uint32_t ui32Len)
{

    unsigned int uIdx;

    //
    // Check for valid UART base address, and valid arguments.
    //
    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if(pcBuf[uIdx] == '\n')
        {
            UARTCharPut(UART0_BASE, '\r');
        }

        //
        // Send the character to the UART output.
        //
        UARTCharPut(UART0_BASE, pcBuf[uIdx]);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);

}


//*****************************************************************************
//
//! A simple UART based vprintf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param vaArgP is a variable argument list pointer whose content will depend
//! upon the format string passed in \e pcString.
//!
//! This function is very similar to the C library <tt>vprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments in the variable arguments list must match the
//! requirements of the format string.  For example, if an integer was passed
//! where a string was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void UARTvprintf(const char *pcString, va_list vaArgP)
{
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;
    double temp;

    //
    // Check the arguments.
    //


    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui32Idx = 0;
            (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        UARTwrite(pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ui32Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Print out the character.
                    //
                    UARTwrite((char *)&ui32Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int32_t)ui32Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui32Value = -(int32_t)ui32Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui32Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }
                case 'f':
                {
                	temp = va_arg(vaArgP, double);
                	UART_float(temp,6);
                	break;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcStr, ui32Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui32Count > ui32Idx)
                    {
                        ui32Count -= ui32Idx;
                        while(ui32Count--)
                        {
                            UARTwrite(" ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui32Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui32Neg)
                    {
                        ui32Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui32Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui32Count > 1) && (ui32Count < 16))
                    {
                        for(ui32Count--; ui32Count; ui32Count--)
                        {
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui32Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui32Idx; ui32Idx /= ui32Base)
                    {
                        pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcBuf, ui32Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    UARTwrite(pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    UARTwrite("ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void UARTprintf(const char *pcString, ...)
{
    va_list vaArgP;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    UARTvprintf(pcString, vaArgP);

    //
    // We're finished with the varargs now.
    //
    va_end(vaArgP);
}

#endif /* UART_C_ */



