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
#include <stdio.h>

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    printf("\t\tTEST CODE\n\r");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\n\r");        // I see putch() is defined in uart2.c
    printf("\t\tDAC1OUT1 TEST\n\r");
    printf("\t\t---- ----\n\n\r");
    
    printf("\tKPU PHYS1600\n\n\r");
    while (1)
    {
        char rxData;
        static unsigned char output_val=0;
        static int out_cycle=0;
            // Logic to echo received data
        test1_PORT = 1;
        out_cycle++;
        if (out_cycle>2200)
        {
            DAC1_SetOutput(output_val);
            output_val++;
            if (output_val > 31) output_val = 0;
            out_cycle = 0;
        }
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