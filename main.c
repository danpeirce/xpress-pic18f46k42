/**

  File Name:
    main.c
*/

#include "mcc_generated_files/mcc.h"
#include <stdio.h>

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    printf("\r\n\n\n\n\n\n\n\n\n");         // short form feed on terminal window
    printf("\t\tTEST CODE\r\n");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\r\n");        // I see putch() is defined in uart2.c
    printf("\t\tECHO TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU PHYS1600\r\n\n\n\n");
    printf("> ");
    
    while (1)
    {
        char rxData;
            // Logic to echo received data
        if(UART2_is_rx_ready())
        {
            rxData = UART2_Read();
            if(UART2_is_tx_ready()) // for USB echo
            {
                UART2_Write(rxData);
                if(rxData == '\r') UART2_Write('\n'); // add newline to return
            }
        }
    }
}
/**
 End of File
*/