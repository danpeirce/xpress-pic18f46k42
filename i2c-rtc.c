#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-lcd.h"
#include "i2c-rtc.h"
#include <stdio.h>

// state functions
void set_time(void); 
void set_minutes10(void);
void set_minutes1(void);

// other functions used only in i2c-rtc.c
void lcd_date(void);


// variables
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
    if((rxData != '\r')&&(rxData>=' ')) 
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
    if(rxData == 0x13) 
    {
        puts("\r\n\n**Set Time**\r\n m for minute\r\n s for second\r\n\n");
        state =  set_time; // use ctrl s in terminal for set time
    } 
    if(rxData == 0x14) 
    {
        read_rtc();
        uart2_time(); // use ctrl t in terminal for read and print time
        lcd_time();
        cursor = cursor + 11;
    } 
    if(rxData == 0x04) 
    {
        read_rtc();
        uart2_time(); // use ctrl t in terminal for read and print time
        lcd_date();
        cursor = cursor + 10;
    }
    test2_PORT = 0;
}

void read_rtc(void)
{
    I2C1_Read1ByteRegister(0X68, 0X12); // read last register so loops to first
    I2C1_ReadNBytes(0X68, data, 0x07);
}

void uart2_time(void)
{    
    printf(" 20%x/", data[0X06]); // year
    printf("%02x/", data[0X05]); // month
    printf("%02x,", data[0X04]); // day of month
    printf(" day %s,", days[data[0X03]-1u]); // day of week
    printf(" time %02x:", 0x1F&(data[0X02]) ); // hour
    printf("%02x:", data[0X01]); //minute
    printf("%02x", data[0X00]);  // seconds
    if (data[0X02]/32 & 0x01) puts(" PM\r" );
    else puts(" AM\r");
}

union 
{
    struct
    {
        uint8_t lower: 4;
        uint8_t upper: 1;
        uint8_t   pm:  1;
        uint8_t  mode: 1;  // am/pm
        uint8_t unnused: 1;
    };
    uint8_t byte;
} hour_parts;

union 
{
    struct
    {
        uint8_t lower: 4;
        uint8_t upper: 4;
    };
    uint8_t byte;
} byte_nibbles;

void lcd_time(void)
{
    char ones, tens;
    hour_parts.byte = data[0x02]; // hour
    ones = hour_parts.lower + '0'; // convert to ascii
    tens = hour_parts.upper + '0';
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, tens);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ones);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ':');
    byte_nibbles.byte = data[0X01]; // minutes
    ones = byte_nibbles.lower + '0'; // convert to ascii
    tens = byte_nibbles.upper + '0';
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, tens);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ones);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ':');
    byte_nibbles.byte = data[0X00]; // minutes
    ones = byte_nibbles.lower + '0'; // convert to ascii
    tens = byte_nibbles.upper + '0';
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, tens);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ones);
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ' ');
    if (hour_parts.pm) I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, 'P');
    else I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, 'A');
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, 'M');
    I2C1_Write1ByteRegister( LCD_ADDRESS , LCD_DATA, ' ');
}

void lcd_date(void)
{
    char str[] = "@2000/00/00"; // some characters modified below
    int index = 3;
    byte_nibbles.byte = data[0x06]; // year
    str[index] = byte_nibbles.upper + '0'; // convert to ascii digit
    index++;
    str[index] = byte_nibbles.lower + '0';
    index = index + 2;                     // skip over '/'
    byte_nibbles.byte = data[0x05]; // month
    str[index] = (0x01 & byte_nibbles.upper) + '0';
    index++;
    str[index] = byte_nibbles.lower + '0';
    index = index + 2;
    byte_nibbles.byte = data[0x04]; // month
    str[index] = byte_nibbles.upper + '0';
    index++;
    str[index] = byte_nibbles.lower + '0';
    I2C1_WriteNBytes( LCD_ADDRESS, str, 11 );
}


void set_time(void)
{
    char rxData; 
    
    test2_PORT = 1;
    rxData = UART2_Read();

    if(UART2_is_tx_ready()) // for USB echo
    {
        if (rxData == '\t') printf("\r\n\n\n");
        else UART2_Write(rxData);
        if(rxData == '\r') 
        {
            UART2_Write('\n'); // add newline to return
        }
        if ((rxData == 'm') || (rxData == 'M'))
        {
            printf("\r\nEnter minutes xx ");
            state = set_minutes10;
        }
        if (rxData == 0x11)   // ctrl+q
        {
            puts("\r\nCancel time set\r\n");
            state = echo;
        }
    }

    test2_PORT = 0;
}

void set_minutes10(void)
{
    char rxData; 
    
    test2_PORT = 1;
    rxData = UART2_Read();

    if(UART2_is_tx_ready()) // for USB echo
    {
        if ((rxData >= '0') && (rxData <= '9'))
        {
            UART2_Write(rxData);
            byte_nibbles.upper = rxData - '0';
            state = set_minutes1;  // next state
        }
        if (rxData == 0x11)   // ctrl+q
        {
            puts("\r\nCancel time set\r\n");
            state = echo;
        }
    }

    test2_PORT = 0;
}

void set_minutes1(void)
{
    char rxData; 
    
    test2_PORT = 1;
    rxData = UART2_Read();

    if(UART2_is_tx_ready()) // for USB echo
    {
        if ((rxData >= '0') && (rxData <= '9'))
        {
            UART2_Write(rxData);
            byte_nibbles.lower = rxData - '0';
            I2C1_Write1ByteRegister(0X68, 0X01, byte_nibbles.byte);  //minute
            puts(" set\r\n");
            state = echo;  // next state
        }
        if (rxData == 0x11)   // ctrl+q
        {
            puts("\r\nCancel time set\r\n");
            state = echo;
        }
    }

    test2_PORT = 0;
}

/* uncomment and manually setup current time and date
{
                
    I2C1_Write1ByteRegister(0X68, 0X00, (3*16+0));  //second 0
    I2C1_Write1ByteRegister(0X68, 0X01, (2*16+9));  //minute
    //I2C1_Write1ByteRegister(0X68, 0X02, (4*16+2*16+1*16+0));  //hour

    I2C1_Write1ByteRegister(0X68, 0X03, 0x07);
    I2C1_Write1ByteRegister(0X68, 0X04, (2*16+7));  //27 day
    I2C1_Write1ByteRegister(0X68, 0X05, (1*16+2));  // 12 month
    I2C1_Write1ByteRegister(0X68, 0X06, (2*16+0));  // 20 year 
}
*/