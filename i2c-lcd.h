/**
  Section: Macro Declarations
 */
#define LCD_ADDRESS             0X3E  // I2C address of this display
#define LCD_DATA                0x40
#define LCD_COMMAND             0x80
#define LINE1_START_ADDRESS     0x80
#define LINE2_START_ADDRESS     0xC0
#define FUNCTION_SET            0x28  // "Function Set"  001X NFXX  or 0x28
#define DISPLAY_ON              0x0E  // "Display ON/OFF Control"  0000 1DCB or 0x0E
#define LCD_CLEAR               0X01 // "Display Clear"  0000 0001  or 0x01
#define ENTRY_MODE              0X02 // "Entry Mode Set"  0000 0010   or 0x02
