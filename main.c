#include "mcc_generated_files/mcc.h"
#include <stdio.h>

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    printf("\t\tTEST CODE\n\r");		
    printf("\t\t---- ----\n\r");      
    printf("\t\tUART1 Buffer TEST\n\r");
    printf("\t\t---- ----\n\n\r");
    
    // printf("\tKPU APSC1299\n\n\r");
    
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
            if (rxData == '-')
            {
                printf("\r\n ");
                while (UART1_is_rx_ready()) printf("\r\n>%c, ", UART1_Read());
                printf(" - done\r\n");
            }
            else if(UART1_is_tx_ready()) // out RC6
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