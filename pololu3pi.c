#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include <stdio.h>

void print_sensors(void)
{
    unsigned int * sensorvalues;
    calibrate();
    while(1)
    {
        sensorvalues = readsensors();
        // __delay_ms(80);
        printf("\rsensor values = %4u, ", *sensorvalues);
        printf("%4u, ", *(sensorvalues+1));
        printf("%4u, ", *(sensorvalues+2));
        printf("%4u, ", *(sensorvalues+3));
        printf("%4u", *(sensorvalues+4));
        printf(" | Timer Value = %5u",TMR3_ReadTimer());
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

void go_pd(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBB);   // start PD control
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(50);   // set speed to 100
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(1);   // set a = 1
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(20);   // set b = 20
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(3);   // set c = 3
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBA);   // set d = 2
}


void stop_pd(void)
{
    while(!UART1_is_tx_ready()) continue;
    UART1_Write(0xBC);   // stop PD control
}