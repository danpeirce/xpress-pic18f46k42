
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

const unsigned int FullOn = 0x037F; //383?

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

	printf("> ");        // print prompt
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
    }
}


void process_command(char rxData)
{
    if (rxData == 0x14) printf("\r\nticks1 %u\r\n", (TMR1_ReadTimer() ) );
    else if (rxData == '0') 
    {
        PWM5_LoadDutyValue(0);
        PWM6_LoadDutyValue(0);
        PWM7_LoadDutyValue(0);
    }
    else if (rxData == 'R') PWM5_LoadDutyValue(FullOn);
    else if (rxData == 'r') PWM5_LoadDutyValue(FullOn/2);
    else if (rxData == 'R') PWM5_LoadDutyValue(FullOn);
    else if (rxData == 'G') PWM6_LoadDutyValue(FullOn);
    else if (rxData == 'g') PWM6_LoadDutyValue(FullOn/2);
    else if (rxData == 'B') PWM7_LoadDutyValue(FullOn);
    else if (rxData == 'b') PWM7_LoadDutyValue(FullOn/2);
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

    printf("\t0. All Off\r\n");
    printf("\tB. Blue Full On\r\n");
    printf("\tb. Blue Half On\r\n");
    printf("\tG. Green Full On\r\n");
    printf("\tg. Green Half On\r\n");
    printf("\tR. Red Full On\r\n");
    printf("\tr. Red Half On\r\n");
    printf("\t   ctrl+t. Print TMR1 Value\r\n");
    printf("\t     Esc, Print Menu\r\n");
    printf("\t\t--------\r\n\n");
}
/**
 End of File
*/