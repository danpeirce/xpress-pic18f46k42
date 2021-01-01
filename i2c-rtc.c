#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "i2c-rtc.h"
#include <stdio.h>

uint8_t data[7];
const char * days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", 
                        "Saturday", "Sunday"
                      };

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