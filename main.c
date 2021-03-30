/**

  File Name:
    main.c
*/

//  execute this command after build (project properties build)
//  copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y
//  output path depends on computer

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-lcd.h"
#include <stdio.h>
#include <string.h>


// #define _XTAL_FREQ 48000000 note already defined in mcc_generated_files/device_config.h

void i2c_lcd_initialize(void);
void processData(char rxData);

i2c1_address_t lcd_address = LCD_ADDRESS; 
static unsigned char cursor = LINE1_START_ADDRESS; // local cursor counter
		                                              // static so only initialized once 
const char apsc_msg[] = "KPU APSC1299"; 

/*
                         Main application
 */
void main(void)
{
	// uint8_t setup_buf[] = { ***just starting to work on this****
    // Initialize the device
    SYSTEM_Initialize();
    i2c_lcd_initialize();
    printf("\f\t\tTEST CODE\r\n");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\r\n");        // I see putch() is defined in uart2.c
    printf("\t\tI2C LCD miniKB TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU APSC1299\r\n\n");
    // address, buffer, number of bytes

    while (1)
    {
        char rxData;
        //unsigned char *miniData;

            // Logic to echo received data
        test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            test2_PORT = 1;
            rxData = UART2_Read();
            processData(rxData);
            
            test2_PORT = 0;
        }
        // void I2C1_ReadNBytes(i2c1_address_t address, uint8_t *data, size_t len)
        I2C1_ReadNBytes(0x5F, &rxData, 1);
        if(rxData!=0x00) 
        {
            // printf("%c", rxData);
            processData(rxData);
        }
        test1_PORT = 0; 
    }
}

void processData(char rxData)
{
    if((rxData >= 0xB4)&&(rxData <= 0xB7))
    {
        if( (rxData == 0xB4)  ) // cursor left
        {
            if ((cursor != LINE1_START_ADDRESS) && (cursor != LINE2_START_ADDRESS )  ) 
            {
                cursor--;
                I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, cursor);
            }
        }
        if( (rxData == 0xB7)  ) //cursor right
        {
            if ((cursor != LINE1_START_ADDRESS+15) && (cursor != LINE2_START_ADDRESS+15 )  ) 
            {
                cursor++;
                I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, cursor);
            }
        }
        if( (rxData == 0xB5) || (rxData == 0xB6) ) //cursor up
        {
            if ( (cursor >= LINE2_START_ADDRESS )  ) cursor = cursor-0x40;
            else cursor = cursor+0x40;
            I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, cursor);
        }
    }
    else if(rxData != '\r') // most characters
    {
        if ((cursor != LINE1_START_ADDRESS+16) && (cursor != LINE2_START_ADDRESS+16 )  )
        {  // only if not just past end of row on LCD
            I2C1_Write1ByteRegister(lcd_address, LCD_DATA, rxData);
            cursor++;
        }
    }
    if (rxData == '\t')  // use tab to clear LCD screen
    {
        I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, LCD_CLEAR); // clear display
        cursor = LINE1_START_ADDRESS; // reset cursor counter variable 
    }
    if(UART2_is_tx_ready()) // for USB echo
    {
        if (rxData == '\t') printf("\r\n\n\n");
        else UART2_Write(rxData);
        if(rxData == '\r') 
        {
            UART2_Write('\n'); // add newline to return
            if (cursor >= LINE2_START_ADDRESS) cursor = LINE1_START_ADDRESS; // move to other line
            else cursor = LINE2_START_ADDRESS;
            I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, cursor);
        }
    }
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
	I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, FUNCTION_SET);  // 001X NFXX  or 0x28
    __delay_us(41);
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, DISPLAY_ON);   // 0000 1DCB or 0x0E
    __delay_us(41);
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, LCD_CLEAR);   // 0000 0001  or 0x01
    __delay_ms(2); 
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, ENTRY_MODE);  // 0000 0010   or 0x02
    __delay_us(41);

    // send messages
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, LINE1_START_ADDRESS); // set to row 0 col 0
    __delay_us(41);
    I2C1_WriteNBytes(lcd_address, data, 17 ); // array data[] contains first 
                                              // string
                                              // 17 is data code plus 16 characters
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, LINE2_START_ADDRESS); // set to row 1 col 0
    __delay_us(41);
    
    I2C1_WriteNBytes(lcd_address, name_msg, 17); // array name_msg[] contains
                                                 // second string
                                              // string
                                              // 17 is data code plus 16 characters												 
    for(count=0;count<40;count++)    // 2 second delay so message displayed for
    {                                //    2 seconds as a splash screen
        __delay_ms(50);
    }
    I2C1_Write1ByteRegister(lcd_address, LCD_COMMAND, LCD_CLEAR); // clear display
    __delay_ms(50);
}
/**
 End of File
*/