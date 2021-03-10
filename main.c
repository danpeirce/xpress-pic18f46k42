
#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include <stdio.h>

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
   
    // the delay caused by these printf statements ensures 3Pi has time to
    // be ready for LCD commands
    menu();

    sendbatteryvoltage(); // sends battery voltage to both LCD and USB
	printf("> ");        // print prompt
    LCD_line2();
    if (roam_PORT) LCD_print("Roam", 4);
    else LCD_print("No Roam", 7);
    
    while (1)
    {
        char rxData;
        
        if (roam_PORT) followline(); // only returns if switch changes to no roam
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
            // if(UART1_is_tx_ready()) // out RC6
            process_command(rxData);

            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}

/**
 End of File
*/