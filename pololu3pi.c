#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include <stdio.h>

unsigned char sensor0[600];
unsigned char sensor1[600];
unsigned char sensor2[600];
unsigned char sensor3[600];
unsigned char sensor4[600];

union word16_u tmrvalue;

void menu(void)
{
    printf(" \r\n");
    printf("\r\n\n\n\n\n\n\n\n");   
               // this should clear the terminal screen mostly
                       // otherwise may display junk from power cycle
    printf("\tKPU APSC1299 3pi-state-2-1\n\n\r");
    printf("\t\t  Menu\n\r");
    printf("\t\t--------\r\n");  
    printf("\t\t@. Pololu Signature?\r\n"); 
    printf("\t\t1. Display mV reading\r\n"); // sent to PuTTY only
    printf("\t\t2. Display mV reading in LCD\r\n");  // also send to LCD
    printf("\t   ctrl+c. Clear LCD\r\n");
	printf("\t   ctrl+d. Dump sensor 1 values\r\n");
    printf("\t   ctrl+s. Print Sensor Values\r\n");
    printf("\t   ctrl+t. Print TMR1 Value\r\n");
    printf("\t\t~. LCD message APSC1299\r\n");
    printf("\t\treturn. LCD go to start of line two\r\n");
    printf("\t\t<, robot spin left\r\n");
    printf("\t\t>, robot spin right\r\n");
    printf("\t\t|, robot stop\r\n");
    printf("\t     Esc, Print Menu\r\n");
    printf("\t\t--------\r\n\n");
}

void print_sensors(void)
{
    struct sensorval_s  sensorvalues;
    // calibrate();
    while(1)
    {
        sensorvalues = readsensors();
        // __delay_ms(80);
        printf("\rsensor values = %4u, ", sensorvalues.s0.word);
        printf("%4u, ", sensorvalues.s1.word);
        printf("%4u, ", sensorvalues.s2.word);
        printf("%4u, ", sensorvalues.s3.word);
        printf("%4u", sensorvalues.s4.word);
        //printf(" | Timer Value = %5u",TMR3_ReadTimer());
    }
    
}

void followline(void)
{
    struct sensorval_s sensorvalues;

    unsigned int sensorReadIndex=0;
    unsigned char pd_mode=1;
    calibrate();
    go_pd(50);    // tell slave to start PID mode
    while(roam_PORT) 
    {
        if (pd_mode)
        {
            sensorvalues = readsensors();

            if ((sensorvalues.s0.word > 250) && (sensorvalues.s4.word>250))
            {
                stop_pd(); // tell slave to stop PID mode
                while(roam_PORT);
            }
            if ((sensorvalues.s1.word > 200) && (sensorvalues.s3.word>200))
            {
                pd_mode = 0;
                stop_pd();
                steer_diff(sensorvalues);
            }

            {
                save_data(sensorvalues, sensorReadIndex);
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
            sensorvalues = readsensors();
            steer_diff(sensorvalues);  
            if (((sensorvalues.s1.word) < 50) && ((sensorvalues.s3.word) < 50))
            {
                pd_mode = 1;
                forward(0);
                go_pd(50);
            }

            {
                save_data(sensorvalues, sensorReadIndex);
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

void forward(unsigned char speed)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC1);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC5);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
}

void forwardD(int speedL, int speedR)
{
    if (speedL >= 0)
    {
        while(!UART1_is_tx_ready()) continue;
        UART1_Write(LEFTMOTOR_FORWARD);
        while(!UART1_is_tx_ready()) continue;
        UART1_Write((char)speedL);
    }
    else
    {
        while(!UART1_is_tx_ready()) continue;
        UART1_Write(LEFTMOTOR_BACKWARD);
        while(!UART1_is_tx_ready()) continue;
        UART1_Write((char)-speedL);
    }
    if (speedR >= 0)
    {
        while(!UART1_is_tx_ready()) continue;
        UART1_Write(RIGHTMOTOR_FORWARD);
        while(!UART1_is_tx_ready()) continue;
        UART1_Write((char)speedR);
    }
    else
    {
        while(!UART1_is_tx_ready()) continue;
        UART1_Write(RIGHTMOTOR_BACKWARD);
        while(!UART1_is_tx_ready()) continue;
        UART1_Write((char)-speedR);
    }        
}

void backward(unsigned char speed)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC2);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC6);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
}

void spinleft(unsigned char speed)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC2);      // left motor backwards
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC5);     // right motor forward
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
}

void spinright(unsigned char speed)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC1);      // left motor forward
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xC6);     // right motor backward
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);
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

struct sensorval_s readsensors(void)
{
    //unsigned char lbyte[5], ubyte[5], i;
    //static unsigned int values[5];
    static struct sensorval_s  value ;
    
    while(!UART1_is_tx_ready()) continue;
    test2_PORT = 1;
    UART1_Write(0x87);
    {
        while (!UART1_is_rx_ready()) continue;
        value.s0.lower = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s0.upper = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s1.lower = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s1.upper = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s2.lower = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s2.upper = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s3.lower = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s3.upper = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s4.lower = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        value.s4.upper = UART1_Read();
    }
    test2_PORT = 0;

    return value;
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

void clearLCD(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB7);   // clear LCD
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

void send_APSC1299(void)
{
    char  msg_length=8; // to send 8 characters
    char msg[] = "APSC1299";
    LCD_print(msg, msg_length); 
}

void calibrate(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBA);   // autocalibration command to slave
    while(!UART1_is_rx_ready()) continue;
    while(UART1_Read() != 'c')
    {
        while(!UART1_is_rx_ready()) continue;
    }
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xB7);   // clear LCD
    LCD_print("Cal Done", 8); // LCD msg
}

void go_pd(unsigned char speed)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBB);   // start PD control
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(speed);   // set speed 
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(1);   // set a = 1
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(20);   // set b = 20
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(3);   // set c = 3
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0x02);   // set d = 2
}


void stop_pd(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBC);   // stop PD control
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
        printf("%4u\r\n", ((unsigned int)sensor4[i]) <<2 ); 
	}
}

void steer_diff(struct sensorval_s sensorvalues)
{
    int diff;
    static int error=0, lasterror=0;

    error = (int)((sensorvalues.s3.word))-(int)((sensorvalues.s1.word));
    diff = error/64 + (error - lasterror)/4;
    lasterror = error;
    forwardD(50+diff, 50-diff);
}

void save_data(struct sensorval_s sensorvalues, unsigned int sensorReadIndex)
{
    sensor0[sensorReadIndex] = ((sensorvalues.s0.word) >> 2);
    sensor1[sensorReadIndex] = ((sensorvalues.s1.word) >> 2);
    sensor2[sensorReadIndex] = ((sensorvalues.s2.word) >> 2);
    sensor3[sensorReadIndex] = ((sensorvalues.s3.word) >> 2);
    sensor4[sensorReadIndex] = ((sensorvalues.s4.word) >> 2);
//    tmrvalue.word = TMR1_ReadTimer();
//    sensor4[sensorReadIndex] = tmrvalue.lower;
}

void process_command(char rxData)
{
    if (rxData == '1') readbatteryvoltage();   // read battery voltage 
                                               //  and send to PuTTY
    else if (rxData == '2') sendbatteryvoltage();   // send battery voltage to LCD
                                               //  and send to PuTTY
    else if (rxData == '@') display_signature();
    else if (rxData == 0x03) UART1_Write(0xB7);      // ctrl+c clear LCD on 3Pi
    else if (rxData == 0x04) dumpSvalues();      // ctrl+d clear LCD on 3Pi
    else if (rxData == 0x13) print_sensors();   // ctrl+s print values loop
    else if (rxData == 0x14) printf("\r\nticks1 %u\r\n", (TMR1_ReadTimer() ) ); // ctrl+t
    else if (rxData == '~') send_APSC1299();  // send APSC1299  msg to LCD
    else if (rxData == '\r') LCD_line2();     // move courser to start of line 2
    else if (rxData == '<') spinleft(50);
    else if (rxData == '>') spinright(50);
    else if (rxData == '|') forward(0);
    else if (rxData == 0x1B) menu();
    else if (rxData >= ' ') sendchar(rxData);       // send the character to the display
}