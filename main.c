/**
  File Name:
    main.c
**/
//	File has been modified from Code Configurator generated file by Dan Peirce B.Sc. Sept 4, 2019
//     File has undergone more revision to the point that the code here was
//     written by Dan Peirce B.Sc. (comment added June10, 2020)
/**
  Description:
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F46K42
        Driver Version    :  2.00
*/

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
    {
        printf("\tKPU APSC1299\n\n\r");
        printf("\t\t  Menu\n\r");
        printf("\t\t--------\r\n");  
        printf("\t\t@. Pololu Signature?\r\n"); 
        printf("\t\t1. Display mV reading\r\n"); // sent to PuTTY only
        printf("\t\t2. Display mV reading in LCD\r\n");  // also send to LCD
        printf("\t\tc. Clear LCD\r\n");
        printf("\t\ts. Print Sensor Values\r\n");
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
    TMR1_StartTimer();  // short times and clock source for timer 3
    TMR2_StartTimer();  // PWM clock source (not used yet)
    TMR3_StartTimer();  // long times
    //printf("Timer Value = %u\r\n",TMR1_ReadTimer());
    //printf("Timer Value = %u\r\n",TMR1_ReadTimer());
    if (roam_PORT) 
    {
        unsigned int * sensorvalues;
        unsigned int time1, tmr3read;
        const unsigned int time1_inc = 57; // 57 for about 10 seconds
        calibrate();
        time1 = TMR3_ReadTimer()+time1_inc;
        go_pd();    // tell slave to start PID mode
        while(1)
        {
            tmr3read = TMR3_ReadTimer();
            if ((tmr3read>time1)&&((0xFFFF-tmr3read)>time1_inc))
            {
                stop_pd(); // tell slave to stop PID mode
                while(1);
            }
            sensorvalues = readsensors();
            if ((*sensorvalues > 250) && (*(sensorvalues+4)>250))
            {
                stop_pd(); // tell slave to stop PID mode
                while(1);
            }
        }
    }
    
    while (1)
    {
        char rxData;
            // Logic to echo received data
        test1_PORT = 1;
        //printf("Timer Value = %u\r\n",TMR1_ReadTimer());
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
            else if (rxData == 's') print_sensors();     // print values loop
            else if (rxData == '-') send_hyphen();     // send hyphen to LCD
            else if (rxData == '~') send_APSC1299();  // send APSC1299  msg to LCD
            else if (rxData == '\r') LCD_line2();     // move courser to start of line 2
            else if (rxData >= ' ') sendchar(rxData);       // send the character to the display

            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}


/**
 End of File
*/