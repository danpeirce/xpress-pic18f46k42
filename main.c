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

void dumpSvalues(void);

unsigned char sensor1[1000];
unsigned char sensor2[1000];
unsigned char sensor3[1000];


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
    TMR1_StartTimer();  // short times and clock source for timer 3
    TMR2_StartTimer();  // PWM clock source (not used yet)
    TMR3_StartTimer();  // long times

    if (roam_PORT) 
    {
        unsigned int * sensorvalues;
        
        unsigned int sensorReadIndex=0;
        unsigned int time1, tmr3read;
        const unsigned int time1_inc = 57; // 57 for about 10 seconds
        calibrate();
        time1 = TMR3_ReadTimer()+time1_inc;
        go_pd(50);    // tell slave to start PID mode
        while(roam_PORT)
        {
            tmr3read = TMR3_ReadTimer();
            if ((tmr3read>time1)&&((0xFFFF-tmr3read)>time1_inc))
            {
                stop_pd(); // tell slave to stop PID mode
                while(roam_PORT);
            }
            sensorvalues = readsensors();
            // if ( (sensorReadIndex>0)  || (*(sensorvalues+4)>25))
            {
                sensor1[sensorReadIndex] = ((*(sensorvalues+1)) >> 2);
                sensor2[sensorReadIndex] = ((*(sensorvalues+2)) >> 2);
                sensor3[sensorReadIndex] = ((*(sensorvalues+3)) >> 2);
                sensorReadIndex++;
                if(sensorReadIndex>999) 
                {
                    stop_pd();
                    while(roam_PORT);
                }
            }            
            if ((*sensorvalues > 250) && (*(sensorvalues+4)>250))
            {
                stop_pd(); // tell slave to stop PID mode
                while(roam_PORT);
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
            else if (rxData == 0x03) UART1_Write(0xB7);      // ctrl+c clear LCD on 3Pi
			else if (rxData == 0x04) dumpSvalues();      // ctrl+d clear LCD on 3Pi
            else if (rxData == 0x13) print_sensors();   // ctrl+s print values loop
            else if (rxData == '~') send_APSC1299();  // send APSC1299  msg to LCD
            else if (rxData == '\r') LCD_line2();     // move courser to start of line 2
            else if (rxData == '<') spinleft(50);
            else if (rxData == '>') spinright(50);
            else if (rxData == '|') forward(0);
            else if (rxData == 0x1B) menu();
            else if (rxData >= ' ') sendchar(rxData);       // send the character to the display

            test2_PORT = 0;
        }
        test1_PORT = 0; 
    }
}


void dumpSvalues(void)
{
	unsigned int i;
	for (i=0;i<1000;i++)
	{
		printf("%4u, ", ((unsigned int)sensor1[i])<<2);
        printf("%4u, ", ((unsigned int)sensor2[i])<<2);
        printf("%4u\r\n", ((unsigned int)sensor3[i])<<2);
	}
}

/**
 End of File
*/