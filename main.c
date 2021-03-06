
#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include <stdio.h>

void dumpSvalues(void);

unsigned char sensor0[600];
unsigned char sensor1[600];
unsigned char sensor2[600];
unsigned char sensor3[600];
unsigned char sensor4[600];

union {
    unsigned int word;
    struct {
        unsigned char lower;
        unsigned char upper;
    } ;
} tmrvalue;

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


    if (roam_PORT) 
    {
        unsigned int * sensorvalues;
        
        unsigned int sensorReadIndex=0;
        unsigned char pd_mode=1;
        calibrate();
        go_pd(50);    // tell slave to start PID mode
        while(roam_PORT)
        {
            if (pd_mode)
            {
                sensorvalues = readsensors();
            
                if ((*sensorvalues > 250) && (*(sensorvalues+4)>250))
                {
                    stop_pd(); // tell slave to stop PID mode
                    while(roam_PORT);
                }
                if ((*(sensorvalues+1) > 200) && (*(sensorvalues+3)>200))
                {
                    int diff;
                    pd_mode = 0;
                    stop_pd();
                    diff = ((int)(*(sensorvalues+3))-(int)(*(sensorvalues+1)))/64;
                    forwardD(50+diff, 50-diff);
                }
                // if ( (sensorReadIndex>0)  || (*(sensorvalues+4)>25))
                {
                    sensor0[sensorReadIndex] = ((*(sensorvalues+0)) >> 2);
                    sensor1[sensorReadIndex] = ((*(sensorvalues+1)) >> 2);
                    sensor2[sensorReadIndex] = ((*(sensorvalues+2)) >> 2);
                    sensor3[sensorReadIndex] = ((*(sensorvalues+3)) >> 2);
                //    sensor4[sensorReadIndex] = ((*(sensorvalues+4)) >> 2);
                    tmrvalue.word = TMR1_ReadTimer();
                    sensor4[sensorReadIndex] = tmrvalue.lower;
                    sensorReadIndex++;
                    if(sensorReadIndex>599) 
                    {
                        stop_pd();
                        while(roam_PORT);
                    }
                }
            }
            else
            {
                int diff;
                static int error=0, lasterror=0;
                sensorvalues = readsensors();
                error = (int)(*(sensorvalues+3))-(int)(*(sensorvalues+1));
                diff = error/64 + (error - lasterror)/4;
                lasterror = error;
                forwardD(50+diff, 50-diff);
                if ((*(sensorvalues+1) < 50) && (*(sensorvalues+3) < 50))
                {
                    pd_mode = 1;
                    forward(0);
                    go_pd(50);
                }
                // if ( (sensorReadIndex>0)  || (*(sensorvalues+4)>25))
                {
                    sensor0[sensorReadIndex] = ((*(sensorvalues+0)) >> 2);
                    sensor1[sensorReadIndex] = ((*(sensorvalues+1)) >> 2);
                    sensor2[sensorReadIndex] = ((*(sensorvalues+2)) >> 2);
                    sensor3[sensorReadIndex] = ((*(sensorvalues+3)) >> 2);
                //    sensor4[sensorReadIndex] = ((*(sensorvalues+4)) >> 2);
                    tmrvalue.word = TMR1_ReadTimer();
                    sensor4[sensorReadIndex] = tmrvalue.lower;
                    sensorReadIndex++;
                    if(sensorReadIndex>599) 
                    {
                        forward(0);
                        while(roam_PORT);
                    }
                }                
            }
        }
    }
    
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
            else if (rxData == 0x03) UART1_Write(0xB7);      // ctrl+c clear LCD on 3Pi
			else if (rxData == 0x04) dumpSvalues();      // ctrl+d clear LCD on 3Pi
            else if (rxData == 0x13) print_sensors();   // ctrl+s print values loop
            else if (rxData == 0x14) printf("\r\nticks1 %u\r\n", (TMR1_ReadTimer() ) );
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
	for (i=0;i<600;i++)
	{
        printf("%4u, ", ((unsigned int)sensor0[i])<<2);
		printf("%4u, ", ((unsigned int)sensor1[i])<<2);
        printf("%4u, ", ((unsigned int)sensor2[i])<<2);
        printf("%4u, ", ((unsigned int)sensor3[i])<<2);
        printf("%4u\r\n", ((unsigned int)sensor4[i]) ); // used for tmr in this branch
	}
}

/**
 End of File
*/