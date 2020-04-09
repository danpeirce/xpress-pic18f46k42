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

#include "mcc_generated_files/mcc.h"
#include <stdio.h>

unsigned int readbatteryvoltage(void);
void sendbatteryvoltage(void);
void send_hyphen(void);
void send_APSC1299(void);
void display_signature(void);
void LCD_print(char *str, char length);
void LCD_line2(void);
void sendchar(char a_char);


/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
   
    // the delay caused by these printf statements ensures 3Pi has time to
    // be ready for LCD commands
    {
        printf("\tKPU APSC1299\n\n\r");
        printf("\t\t  Menu\n\r");
        printf("\t\t--------\r\n");  
        printf("\t\t@. Pololu Signature?\r\n"); 
        printf("\t\t1. Display mV reading\r\n"); // sent to PuTTY only
        printf("\t\t2. Display mV reading in LCD\r\n");  // also send to LCD
        printf("\t\tc. Clear LCD\r\n");
        printf("\t\t-. Send hyphen to LCD\r\n");
        printf("\t\t~. LCD message APSC1299\r\n");
        printf("\t\treturn. LCD go to start of line two\r\n");
        printf("\t\t?. LCD display error and hex 3F\r\n");
        printf("\t\t' '. LCD display error and hex 20\r\n");
        printf("\t\t--------\r\n\n");
    }

    sendbatteryvoltage(); // sends battery voltage to both LCD and USB
    LCD_line2();
    if (roam_PORT) LCD_print("Roam", 4);
    else LCD_print("No Roam", 7);
    
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
            // if(UART1_is_tx_ready()) // out RC6
            if (rxData == '1') readbatteryvoltage();   // read battery voltage 
                                                       //  and send to PuTTY
            else if (rxData == '2') sendbatteryvoltage();   // send battery voltage to LCD
                                                       //  and send to PuTTY
            else if (rxData == '@') display_signature();
            else if (rxData == 'c') UART1_Write(0xB7);      // clear LCD on 3Pi
            else if (rxData == '-') send_hyphen();     // send hyphen to LCD
            else if (rxData == '~') send_APSC1299();  // send APSC1299  msg to LCD
            else if (rxData == '\r') LCD_line2();     // move courser to start of line 2
            else if (rxData >= ' ') sendchar(rxData);       // send the character to the display

            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}

unsigned int readbatteryvoltage(void)
{
    unsigned char lbyte, ubyte;
    
    printf("\r\n\tBattery Voltage = ");
    
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB1);
    while (!UART1_is_rx_ready()) continue;
    lbyte = UART1_Read();
    while (!UART1_is_rx_ready()) continue;
    ubyte = UART1_Read();
    printf("%d mV\r\n", ubyte*256 + lbyte);
    return (unsigned int)(ubyte*256 + lbyte);
}

// sends battery voltage to LCD
void sendbatteryvoltage(void)
{
    unsigned int voltage;
    char bat_volt[9];
    unsigned char msg_length, i=0;
    
    voltage = readbatteryvoltage();
    msg_length = sprintf(bat_volt, "%u mV", voltage);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB8);   // print LCD command to slave
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(msg_length);     // send eight characters
    while (i<msg_length)
    {
        if(UART1_is_tx_ready())
        {
            UART1_Write(bat_volt[i]);
            i++;
        }
    }
    
}

void sendchar(char a_char)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB8);   // print LCD command to slave
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(1);     // send one character
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(a_char);     // send one character
}

void LCD_print(char *str, char length)
{
    char i=0;
    
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB8);   // print LCD command to slave
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(length);     // send eight characters
    while (i<length)
    {
        if(UART1_is_tx_ready())
        {
            UART1_Write(str[i]);
            i++;
        }
    }
}

void LCD_line2(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB9);   // goto LCD position
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0x00);   // column 0
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0x01);   // row 1
}

void display_signature(void)
{
    char signature[7], i = 0;
    int sig_length = 6;
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0x81);
    while (i < sig_length)
    {
        if (UART1_is_rx_ready())
        {
            signature[i] = UART1_Read();
            i++;
        }
    }
    signature[sig_length] = '\0';  // terminate string
    printf("\r\n\tThe Signature from 3Pi is: %s\r\n", signature);
    LCD_print(signature, sig_length);
}

// just to test that printing to the LCD is working
void send_hyphen(void)
{
    // see https://www.pololu.com/docs/0J21/10.a 
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB8);   // print LCD command to slave
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(1);     // send one character
    while(!UART1_is_tx_ready()) continue;
    UART1_Write('-');   // send a hyphen
}

void send_APSC1299(void)
{
    char  msg_length=8; // to send 8 characters
    char msg[] = "APSC1299";
    LCD_print(msg, msg_length); 
}

/**
 End of File
*/