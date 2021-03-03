/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.80.0
        Device            :  PIC18F46K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB 	          :  MPLAB X 5.30	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set roam aliases
#define roam_TRIS                 TRISBbits.TRISB5
#define roam_LAT                  LATBbits.LATB5
#define roam_PORT                 PORTBbits.RB5
#define roam_WPU                  WPUBbits.WPUB5
#define roam_OD                   ODCONBbits.ODCB5
#define roam_ANS                  ANSELBbits.ANSELB5
#define roam_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define roam_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define roam_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define roam_GetValue()           PORTBbits.RB5
#define roam_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define roam_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define roam_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define roam_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define roam_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define roam_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define roam_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define roam_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)
#define RB6_SetAnalogMode()         do { ANSELBbits.ANSELB6 = 1; } while(0)
#define RB6_SetDigitalMode()        do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)
#define RB7_SetAnalogMode()         do { ANSELBbits.ANSELB7 = 1; } while(0)
#define RB7_SetDigitalMode()        do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set test1 aliases
#define test1_TRIS                 TRISDbits.TRISD0
#define test1_LAT                  LATDbits.LATD0
#define test1_PORT                 PORTDbits.RD0
#define test1_WPU                  WPUDbits.WPUD0
#define test1_OD                   ODCONDbits.ODCD0
#define test1_ANS                  ANSELDbits.ANSELD0
#define test1_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define test1_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define test1_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define test1_GetValue()           PORTDbits.RD0
#define test1_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define test1_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define test1_SetPullup()          do { WPUDbits.WPUD0 = 1; } while(0)
#define test1_ResetPullup()        do { WPUDbits.WPUD0 = 0; } while(0)
#define test1_SetPushPull()        do { ODCONDbits.ODCD0 = 0; } while(0)
#define test1_SetOpenDrain()       do { ODCONDbits.ODCD0 = 1; } while(0)
#define test1_SetAnalogMode()      do { ANSELDbits.ANSELD0 = 1; } while(0)
#define test1_SetDigitalMode()     do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set test2 aliases
#define test2_TRIS                 TRISDbits.TRISD1
#define test2_LAT                  LATDbits.LATD1
#define test2_PORT                 PORTDbits.RD1
#define test2_WPU                  WPUDbits.WPUD1
#define test2_OD                   ODCONDbits.ODCD1
#define test2_ANS                  ANSELDbits.ANSELD1
#define test2_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define test2_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define test2_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define test2_GetValue()           PORTDbits.RD1
#define test2_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define test2_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define test2_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define test2_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define test2_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define test2_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define test2_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define test2_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)

// get/set Tick aliases
#define Tick_TRIS                 TRISDbits.TRISD2
#define Tick_LAT                  LATDbits.LATD2
#define Tick_PORT                 PORTDbits.RD2
#define Tick_WPU                  WPUDbits.WPUD2
#define Tick_OD                   ODCONDbits.ODCD2
#define Tick_ANS                  ANSELDbits.ANSELD2
#define Tick_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define Tick_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define Tick_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define Tick_GetValue()           PORTDbits.RD2
#define Tick_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define Tick_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define Tick_SetPullup()          do { WPUDbits.WPUD2 = 1; } while(0)
#define Tick_ResetPullup()        do { WPUDbits.WPUD2 = 0; } while(0)
#define Tick_SetPushPull()        do { ODCONDbits.ODCD2 = 0; } while(0)
#define Tick_SetOpenDrain()       do { ODCONDbits.ODCD2 = 1; } while(0)
#define Tick_SetAnalogMode()      do { ANSELDbits.ANSELD2 = 1; } while(0)
#define Tick_SetDigitalMode()     do { ANSELDbits.ANSELD2 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/