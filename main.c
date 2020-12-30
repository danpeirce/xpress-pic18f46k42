/**
  File Name: main.c
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
void print_time(void);
void i2c_lcd_initialize(void);

i2c1_address_t lcd_address = LCD_ADDRESS; 

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
    printf("\t\tTEST CODE\r\n");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\r\n");        // I see putch() is defined in uart2.c
    printf("\t\tI2C LCD TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU APSC1299\r\n\n");
    print_time();
    // address, buffer, number of bytes

    while (1)
    {
        char rxData;
        static unsigned char cursor = LINE1_START_ADDRESS; // local cursor counter
		                                              // static so only initialized once 
            // Logic to echo received data
        test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            test2_PORT = 1;
            rxData = UART2_Read();
            if(rxData != '\r') 
            {
                I2C1_Write1ByteRegister(lcd_address, LCD_DATA, rxData);
                cursor++;
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
            if(rxData == 0x13)  // use ctrl s in terminal for read time 
            {
                /*
                I2C1_Write1ByteRegister(0X68, 0X00, (3*16+0));  //second 0
                I2C1_Write1ByteRegister(0X68, 0X01, (2*16+9));  //minute
                //I2C1_Write1ByteRegister(0X68, 0X02, (4*16+2*16+1*16+0));  //hour
                
                I2C1_Write1ByteRegister(0X68, 0X03, 0x07);
                I2C1_Write1ByteRegister(0X68, 0X04, (2*16+7));  //27 day
                I2C1_Write1ByteRegister(0X68, 0X05, (1*16+2));  // 12 month
                I2C1_Write1ByteRegister(0X68, 0X06, (2*16+0));  // 20 year */
            }
            if(rxData == 0x14) print_time(); // use ctrl t in terminal for read and print time 

            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}

void print_time(void)
{
    const char *days[8];
    days[0] = "\0";
    days[1] = "Monday";
    days[2] = "Tuesday";
    days[3] = "Wednesday";
    days[4] = "Thursday";
    days[5] = "Friday";
    days[6] = "Saturday";
    days[7] = "Sunday";
    
    char timeh;
    printf(" 20%x/", I2C1_Read1ByteRegister(0X68, 0X06)); // year
    printf("%x/", I2C1_Read1ByteRegister(0X68, 0X05)); // month
    printf("%x,", I2C1_Read1ByteRegister(0X68, 0X04)); // day of month
    printf(" day %s,", days[I2C1_Read1ByteRegister(0X68, 0X03)]); // day of week
    printf(" time %02x:", 0x1F&(timeh=I2C1_Read1ByteRegister(0X68, 0X02)) );
    printf("%02x:", I2C1_Read1ByteRegister(0X68, 0X01));
    printf("%02x", I2C1_Read1ByteRegister(0X68, 0X00));
    if (timeh/32 & 0x01) puts(" PM\r" );
    else puts(" AM\r");
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