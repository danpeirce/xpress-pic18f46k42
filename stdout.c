#include "i2c-lcd.h"
#include "stdout.h"

void (*stdout)(char)= lcd_out; // lcd_out is default function called

void putch(char txData)  // printf results in putch being called
{
    stdout(txData);    // function pointer  redirects to chosen function
}
