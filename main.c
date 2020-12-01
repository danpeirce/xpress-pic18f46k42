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
#include "i2c-lcd.h"
#include "stdout.h"
#include <stdio.h>
#include <string.h>

void header_uart2(void); 

// i2c1_address_t lcd_address = LCD_ADDRESS; 

const char apsc_msg[] = "KPU APSC1299"; 

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    i2c_lcd_initialize();
	stdout = uart2_out;
    header_uart2();
	stdout = lcd_out;
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
                I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_DATA, rxData);
                cursor++;
            }
            if (rxData == '\t')  // use tab to clear LCD screen
            {
                I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, LCD_CLEAR); // clear display
                cursor = LINE1_START_ADDRESS; // reset cursor counter variable 
            }
            if(rxData == '\\') printf(" ~ backslash");
            if(UART2_is_tx_ready()) // for USB echo
            {
                if (rxData == '\t') 
                {
                    UART2_Write('\r');
                    UART2_Write('\n');
                    UART2_Write('\n');
                }
                else UART2_Write(rxData);
                if(rxData == '\r') 
                {
                    UART2_Write('\n'); // add newline to return
                    if (cursor >= LINE2_START_ADDRESS) cursor = LINE1_START_ADDRESS; // move to other line
                    else cursor = LINE2_START_ADDRESS;
                    I2C1_Write1ByteRegister(LCD_ADDRESS, LCD_COMMAND, cursor);
                }
            }
            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}

void header_uart2(void)
{
	printf("\t\tTEST CODE\r\n");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\r\n");        // I see putch() is defined in uart2.c
    printf("\t\tI2C LCD TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU APSC1299\r\n\n");
	
}


/**
 End of File
*/