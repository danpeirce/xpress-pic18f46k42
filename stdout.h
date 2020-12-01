void uart2_out(char txData);
void uart1_out(char txData);
// the prototype for lcd_out() is in i2c.h

extern void (*stdout)(char);  // declaration of function pointer for consistency
