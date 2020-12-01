#include "i2c-lcd.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "stdout.h"

void (*stdout)(char)= lcd_out; // lcd_out is default function called

void putch(char txData)  // printf results in putch being called
{
    stdout(txData);    // function pointer  redirects to chosen function
}

void uart2_out(char txData)  // renamed to allow for redirection of stdout
{
    UART2_Write(txData);
}

void uart1_out(char txData)  // newly created
{
    UART1_Write(txData);
}
