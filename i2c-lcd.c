#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-lcd.h"


void putch(char txData)
{
   I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_DATA, txData);
}
