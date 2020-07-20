/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs
*/
//	File has been modified from Code Configurator generated file by Dan Peirce B.Sc. Sept 4, 2019
/**
  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F46K42
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
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
    char active = 0;
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    // uart2 switch to 9600 baud
        // BRGL 225; 
    U2BRGL = 0xE1;
    // BRGH 4; 
    U2BRGH = 0x04;
    printf("\r\n\t\t*** Board Reset ***\r\n");
    printf("\r\n\t\tSet terminal to 115200 baud and hit Enter\r\n");
        // uart2 switch to 115200 baud
    U2BRGL = 0x67;
        // BRGH 0; 
    U2BRGH = 0x00;
    while(!active)
    {
        char rxData;
            // Logic to echo received data
        //test1_PORT = 1;
        if(UART2_is_rx_ready())
        {
            //test2_PORT = 1;
            if ('\r' == UART2_Read())
            {
                active = 1;
            }
        }
    }
    printf("\t\tTEST CODE\n\r");		//Enable redirect STDIO to USART before using printf statements
    printf("\t\t---- ----\n\r");        // I see putch() is defined in uart2.c
    printf("\t\tECHO TEST\n\r");
    printf("\t\t---- ----\n\n\r");
    
    printf("\tKPU APSC1299\n\n\r");
    
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
            //test2_PORT = 0;
        }
        //test1_PORT = 0; 
    }
}
/**
 End of File
*/