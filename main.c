/**

  File Name:
    main.c
*/
//	File has been modified from Code Configurator generated file by Dan Peirce B.Sc. Sept 4, 2019
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

/*
                         Main application
 */
void main(void)
{
    static unsigned char data_buf1[3]= {0x00,0x00,0x00};
    static unsigned char data_buf2[3]= {0x0A,0xFF,0x00};
    // Initialize the device
    SYSTEM_Initialize();
    
    printf("\t\tTEST CODE\n\r");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\n\r");        // I see putch() is defined in uart2.c
    printf("\t\tI2C TEST\n\r");
    printf("\t\t---- ----\n\n\r");
    
    printf("\tKPU APSC1299\n\n\r");
    // address, buffer, number of bytes
    I2C1_WriteNBytes(0x40, data_buf1, 2 );
    printf("I2C data sent\r\n");
    I2C1_WriteNBytes(0x40, data_buf2, 2 );
    printf("I2C data sent\r\n");
    while (1)
    {
        char rxData;
            // Logic to echo received data
        test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            test2_PORT = 1;
            rxData = UART2_Read();
            if(UART2_is_tx_ready()) // for USB echo
            {
                UART2_Write(rxData);
                if(rxData == '\r') UART2_Write('\n'); // add newline to return
            }
            if(UART1_is_tx_ready()) // out RC6
            {
                UART1_Write(rxData);
                if(rxData == '\r') UART1_Write('\n'); // add newline to return
            }
            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}
/**
 End of File
*/