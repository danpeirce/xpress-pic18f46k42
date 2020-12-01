#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-lcd.h"


void lcd_out(char txData)
{
   I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_DATA, txData);
}

/*
### Initialization Sequence

* Power On
* *Wait for more than 15 ms after VDD rises to 4.5V*
* "Function Set"  001X NFXX  or 0x28
    * where X is don't care 
	* N is 0 1-line mode
	* **N is 1 2-line mode**  
	* **F is 0 5*8 dots**    
	* F is 1 5*11 dots
* *Wait for more than 39 µs*
* "Display ON/OFF Control"  0000 1DCB or 0x0E
    * where D 0 is display off
	* *D 1 is display on*
	* C 0 is Cursor off
	* *C 1 is Cursor On*
	* *B 0 is blink off*
	* *B 1 is blink on
* *Wait for more than 39 µs*
* "Display Clear"  0000 0001  or 0x01
* *Wait for more than 1.53 ms*
* "Entry Mode Set"  0000 0010   or 0x02
*/

void i2c_lcd_initialize(void)
{
    //static uint8_t setup_buf[] = { FUNCTION_SET, DISPLAY_ON_OFF, LCD_CLEAR, ENTRY_MODE};
    static uint8_t data[] =     "@KPU APSC1299 Int"; // 0x40 to send data;
    static uint8_t name_msg[] = "@Microcontrollers"; // @ is same as 0x40 same as LCD_DATA
    unsigned char count;
   
    // Initialization Sequence
    __delay_ms(16); 
	I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, FUNCTION_SET);  // 001X NFXX  or 0x28
    __delay_us(41);
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, DISPLAY_ON);   // 0000 1DCB or 0x0E
    __delay_us(41);
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, LCD_CLEAR);   // 0000 0001  or 0x01
    __delay_ms(2); 
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, ENTRY_MODE);  // 0000 0010   or 0x02
    __delay_us(41);

    // send messages
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, LINE1_START_ADDRESS); // set to row 0 col 0
    __delay_us(41);
    I2C1_WriteNBytes(LCD_ADDRESS, data, 17 ); // array data[] contains first 
                                              // string
                                              // 17 is data code plus 16 characters
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, LINE2_START_ADDRESS); // set to row 1 col 0
    __delay_us(41);
    
    I2C1_WriteNBytes(LCD_ADDRESS, name_msg, 17); // array name_msg[] contains
                                                 // second string
                                              // string
                                              // 17 is data code plus 16 characters												 
    for(count=0;count<40;count++)    // 2 second delay so message displayed for
    {                                //    2 seconds as a splash screen
        __delay_ms(50);
    }
    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, LCD_CLEAR); // clear display
    __delay_ms(50);
}
