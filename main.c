/**

  File Name:
    main.c
*/
//   set up for Grove I2C LCD Nov 20, 2020 by Dan Peirce B.Sc.
/**
  Description:
    Generation Information :
        Product Revision  :  PIC18 MCUs - 1.77
        Device            :  PIC18F46K42
        Driver Version    :  2.00
*/

//  execute this command after build (project properties build)
//  copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y
//  output path depends on computer

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include <stdio.h>
#include <string.h>


#define _XTAL_FREQ 48000000

void i2c_lcd_initialize(void);

i2c1_address_t lcd_address = 0X3E; 

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
    printf("\t\tLCD TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU APSC1299\r\n\n");
    // address, buffer, number of bytes

    while (1)
    {
        char rxData;
        static unsigned char cursor = 0x80; // local cursor counter
            // Logic to echo received data
        test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            test2_PORT = 1;
            rxData = UART2_Read();
            if(rxData != '\r') 
            {
                I2C1_Write1ByteRegister(lcd_address, 0x40, rxData);
                cursor++;
            }
            if (rxData == '\t')  // use tab to clear LCD screen
            {
                I2C1_Write1ByteRegister(lcd_address, 0x80, 0x01); // clear display
                cursor = 0x80; // reset cursor counter variable 
            }
            if(UART2_is_tx_ready()) // for USB echo
            {
                if (rxData == '\t') printf("\r\n\n\n");
                else UART2_Write(rxData);
                if(rxData == '\r') 
                {
                    UART2_Write('\n'); // add newline to return
                    if (cursor >= 0xC0) cursor = 0x80; // move to other line
                    else cursor = 0xC0;
                    I2C1_Write1ByteRegister(lcd_address, 0x80, cursor);
                }
            }
            test2_PORT = 0;
        }
        test1_PORT = 0; 
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
    //static uint8_t setup_buf[] = { 0x28, 0x0E, 0x01, 0x02};
    static uint8_t data[] =     "@KPU APSC1299 Int"; // 0x40 to send data;
    static uint8_t name_msg[] = "@Microcontrollers"; // @ is same as 0x40
    unsigned char count;
   
    // Initialization Sequence
    __delay_ms(16); 
	I2C1_Write1ByteRegister(lcd_address, 0x80, 0x28);
    __delay_us(41);
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0x0E);
    __delay_us(41);
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0x01);
    __delay_ms(2); 
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0x02);
    __delay_us(41);

    // send messages
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0x80); // set to row 0 col 0
    __delay_us(41);
    I2C1_WriteNBytes(lcd_address, data, 17 ); // array data contains first 
                                              // string
                                              // 
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0xC0); // set to row 1 col 0
    __delay_us(41);
    
    I2C1_WriteNBytes(lcd_address, name_msg, 17); // array name_msg contains
                                                 // second string
    for(count=0;count<40;count++)    // 2 second delay
    {
        __delay_ms(50);
    }
    I2C1_Write1ByteRegister(lcd_address, 0x80, 0x01); // clear display
    __delay_ms(50);
}
/**
 End of File
*/