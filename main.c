
#include "mcc_generated_files/mcc.h"
#include <stdio.h>

void process_command(char rxData);
void menu(void);

union word16_u {
    unsigned int word;
    struct {
        unsigned char lower;
        unsigned char upper;
    } ;
};

union word16_u tmrvalue;

/*
                         Main application
 */
void main(void)
{
    static unsigned int ticks, oldticks, nbase=179, ncycles=179;

    // Initialize the device
    SYSTEM_Initialize();
   
    // the delay caused by these printf statements ensures 3Pi has time to
    // be ready for LCD commands
    menu();

	printf("> ");        // print prompt
    oldticks = TMR1_ReadTimer();
    ticks = oldticks + 10000*2;    // intervale in ms times 2
    while (1)
    {
        char rxData;
      
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
        if (TMR1_ReadTimer() == ticks)
        {
            oldticks = ticks;
            ticks = oldticks + 10000 * 2;
            ncycles = ncycles + nbase;
            if (ncycles > (5 * nbase) ) ncycles = nbase;
            //if (ncycles > (4 * nbase) ) ncycles =  ncycles -20;
            PWM5_LoadDutyValue(ncycles);
        }
    }
}


void process_command(char rxData)
{
    if (rxData == 0x14) printf("\r\nticks1 %u\r\n", (TMR1_ReadTimer() ) );
    else if (rxData == 0x1B) menu();
}

void menu(void)
{
    printf(" \r\n");
    printf("\r\n\n\n\n\n\n\n\n");   
               // this should clear the terminal screen mostly
                       // otherwise may display junk from power cycle
    printf("\tKPU PHYS1600 CLC TMR0 TMR1\n\n\r");
    printf("\t\t  Menu\n\r");
    printf("\t\t--------\r\n");  

    printf("\t   ctrl+t. Print TMR1 Value\r\n");
    printf("\t     Esc, Print Menu\r\n");
    printf("\t\t--------\r\n\n");
}
/**
 End of File
*/