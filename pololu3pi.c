#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include <stdio.h>

void menu(void)
{
    printf(" \r\n");
    printf("\r\n\n\n\n\n\n\n\n");   
               // this should clear the terminal screen mostly
                       // otherwise may display junk from power cycle
    printf("\tKPU APSC1299 3pi-menu-basic3\n\n\r");
    printf("\t\t  Menu\n\r");
    printf("\t\t--------\r\n");  
    printf("\t\t@. Pololu Signature?\r\n"); 
    printf("\t\t1. Display mV reading\r\n"); // sent to PuTTY only
    printf("\t\t2. Display mV reading in LCD\r\n");  // also send to LCD
    printf("\t   ctrl+c. Clear LCD\r\n");
	printf("\t   ctrl+d. Dump sensor 1 values\r\n");
    printf("\t   ctrl+s. Print Sensor Values\r\n");
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
    unsigned int * sensorvalues;
    // calibrate();
    while(1)
    {
        sensorvalues = readsensors();
        // __delay_ms(80);
        printf("\rsensor values = %4u, ", *sensorvalues);
        printf("%4u, ", *(sensorvalues+1));
        printf("%4u, ", *(sensorvalues+2));
        printf("%4u, ", *(sensorvalues+3));
        printf("%4u", *(sensorvalues+4));
        //printf(" | Timer Value = %5u",TMR3_ReadTimer());
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

unsigned int* readsensors(void)
{
    unsigned char lbyte[5], ubyte[5], i;
    static unsigned int values[5];
    
    // printf("\r\n\tSensor Readings =  ");
    
    while(!UART1_is_tx_ready()) continue;
    test2_PORT = 1;
    UART1_Write(0x87);
    for (i=0;i<5;i++)
    {
        while (!UART1_is_rx_ready()) continue;
        lbyte[i] = UART1_Read();
        while (!UART1_is_rx_ready()) continue;
        ubyte[i] = UART1_Read();
        values[i] = ubyte[i]*256 + lbyte[i];
    }
    test2_PORT = 0;

    return values;
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