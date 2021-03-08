#define LEFTMOTOR_FORWARD 0xC1
#define LEFTMOTOR_BACKWARD 0XC2
#define RIGHTMOTOR_FORWARD 0xC5
#define RIGHTMOTOR_BACKWARD 0xC6

union word16_u {
    unsigned int word;
    struct {
        unsigned char lower;
        unsigned char upper;
    } ;
};

void menu(void);
unsigned int readbatteryvoltage(void);
unsigned int* readsensors(void);
void sendbatteryvoltage(void);
void send_APSC1299(void);
void display_signature(void);
void LCD_print(char *str, char length);
void forward(unsigned char speed);
void forwardD(int speedL, int speedR);
void backward(unsigned char speed);
void spinleft(unsigned char speed);
void spinright(unsigned char speed);
void LCD_line2(void);
void sendchar(char a_char);
void clearLCD(void);
void calibrate(void);
void go_pd(unsigned char speed);
void stop_pd(void);
void print_sensors(void);

