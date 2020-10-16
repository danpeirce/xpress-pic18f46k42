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
    printf("\t\tDEBUGGer TEST\n\r");
    printf("\t\t---- ----\n\n\r");
    
    printf("\tKPU PHYS1600\n\n\r");
    
    while (1)
    {
        test1_PORT = 0;
        LED1_PORT = 0;
        printf("LED OFF\r\n");
        LED1_PORT = 1;
        printf("LED On\r\n");
        test1_PORT = 1;
    }
}
/**
 End of File
*/

// note add MyConfig.mc3 to Important Files so that I will not have to 
//      start from scratch next time I update something on the MCC