/*
  File Name:
    main.c
*/
//	File has been modified from Code Configurator generated file by Dan Peirce B.Sc. Sept 4, 2019
/**
  Description:
        Device            :  PIC18F46K42
*/


//  execute this command after build (project properties build)
//  copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y
//  output path depends on computer

#include "mcc_generated_files/mcc.h"
#include <stdio.h>

#define _XTAL_FREQ 48000000  // for delay functions

void baud9600(void);
void baud115200(void);
void waitforreturn(void);
void UART1_Write_st(char * string_p);

/*
                         Main application
 */
void main(void)
{
   
    // Initialize the device
    SYSTEM_Initialize();
 
    baud9600();
    printf("\r\n\t\t*** Board Reset ***");
    printf("\r\n\t\tThis is the UART2 window***\r\n");
    printf("\r\n\t\t*** each Reset bumps UART2 terminal to 9600 baud***\r\n");
    UART1_Write_st("\r\n\n**This is the UART1 window");
    UART1_Write_st("\r\nLook at the UART2 window\r\n");
    printf("\r\n\t\tManually Set UART2 terminal back to 115200 baud and \r\n\t\tthen hit Enter\r\n");
    __delay_ms(8); // allow buffer to empty before changing baud
    baud115200();
    waitforreturn();
    printf("\r\n\n\t\tTEST CODE\r\n");		
    printf("\t\t---- ----\r\n");        // putch() is defined in uart2.c
    printf("\t\tECHO TEST\r\n");
    printf("\t\t---- ----\r\n\n");
    
    printf("\tKPU APSC1299 or PHYS1600\r\n\n");
    printf("text typed in this window will be also print in UART1 window\r\n\n");
    UART1_Write_st("\r\nAnything typed in UART2 window should now be copied here\r\n");
    
    while (1)
    {
        char rxData;
            // Logic to echo received data
        //test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            //test2_PORT = 1;
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
        }
    }
}

void baud9600(void)
{
     // uart2 switch to 9600 baud
    // these values will depend on the oscillator set up
    // BRGL 225; 
    U2BRGL = 0xE1;
    // BRGH 4; 
    U2BRGH = 0x04;
}

void baud115200(void)
{
    // uart2 switch to 115200 baud
    // these values will depend on the oscillator set up
    U2BRGL = 0x67;
        // BRGH 0; 
    U2BRGH = 0x00;
}

void waitforreturn(void)
{
    char active = 0;
    while(!active)
    {
        char rxData;
            // Logic to echo received data
        //test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            if ('\r' == UART2_Read())
            {
                active = 1;
            }
        }
    }
}


void UART1_Write_st(char * string_p)
{
    while(*string_p!='\0')
    {
        UART1_Write(*string_p);
        string_p++;
    }
}
/**
 End of File
*/