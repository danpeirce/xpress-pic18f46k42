#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-lcd.h"
#include "i2c-rtc.h"
#include <stdio.h>

void (*state)(void) = echo; // state used here and used as extern in main.c
static uint8_t data[7];     // static to limit scope to this file
const char * days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", 
                        "Saturday", "Sunday"
                      };

void echo(void)
{
    char rxData; 
    static unsigned char cursor = LINE1_START_ADDRESS; // local cursor counter
		                                              // static so only initialized once     
    test2_PORT = 1;
    rxData = UART2_Read();
    if(rxData != '\r') 
    {
        I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, rxData);
        cursor++;
    }
    if (rxData == '\t')  // use tab to clear LCD screen
    {
        I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_COMMAND, LCD_CLEAR); // clear display
        cursor = LINE1_START_ADDRESS; // reset cursor counter variable 
    }
    if(UART2_is_tx_ready()) // for USB echo
    {
        if (rxData == '\t') printf("\r\n\n\n");
        else UART2_Write(rxData);
        if(rxData == '\r') 
        {
            UART2_Write('\n'); // add newline to return
            if (cursor >= LINE2_START_ADDRESS) cursor = LINE1_START_ADDRESS; // move to other line
            else cursor = LINE2_START_ADDRESS;
            I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_COMMAND, cursor);
        }
    }
    if(rxData == 0x13) set_time(); // use ctrl s in terminal for set time 
    if(rxData == 0x14) print_time(); // use ctrl t in terminal for read and print time 

    test2_PORT = 0;
}

void print_time(void)
{
    I2C1_Read1ByteRegister(0X68, 0X12); // read last register so loops to first
    I2C1_ReadNBytes(0X68, data, 0x07);

    printf(" 20%x/", data[0X06]); // year
    printf("%x/", data[0X05]); // month
    printf("%x,", data[0X04]); // day of month
    printf(" day %s,", days[data[0X03]-1u]); // day of week
    printf(" time %02x:", 0x1F&(data[0X02]) );
    printf("%02x:", data[0X01]);
    printf("%02x", data[0X00]);
    if (data[0X02]/32 & 0x01) puts(" PM\r" );
    else puts(" AM\r");
}

void set_seconds(unsigned char seconds)
{
    
}

void set_time(void)
{
                /* uncomment and manually setup current time and date
    I2C1_Write1ByteRegister(0X68, 0X00, (3*16+0));  //second 0
    I2C1_Write1ByteRegister(0X68, 0X01, (2*16+9));  //minute
    //I2C1_Write1ByteRegister(0X68, 0X02, (4*16+2*16+1*16+0));  //hour

    I2C1_Write1ByteRegister(0X68, 0X03, 0x07);
    I2C1_Write1ByteRegister(0X68, 0X04, (2*16+7));  //27 day
    I2C1_Write1ByteRegister(0X68, 0X05, (1*16+2));  // 12 month
    I2C1_Write1ByteRegister(0X68, 0X06, (2*16+0));  // 20 year */
}