extern void (*state)(void);

//used in main.c and i2c-rtc.c
void uart2_time(void);
void lcd_time(void);
void read_rtc(void);

void echo(void);
