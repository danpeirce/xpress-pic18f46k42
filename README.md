# Testing PIC18F46K42 XPRESS Board

Initial testing of Microchip XPRESS board with the PIC18F46K42 target MCU.

The example Microchip Code Configurator was used to generate most of the code in the initial commit of this project.
It was my first attempt at using the Code Configurator tool. Example code for main was found in UART.h.

So far no interrupts have been used.

A branch has been added to the project for basic testing of the XPRESS board. See 
[https://github.com/danpeirce/xpress-pic18f46k43/tree/test-Xpress](https://github.com/danpeirce/xpress-pic18f46k43/tree/test-Xpress).

## Tags

The tags section has been moved to a new file **tags-readme-April2020.md**

## Board Features

This board has a USB microB connector. It has a PIC MCU on board configured to act as an interface and it enumerates as 
a multifunction device. This allows one to both download programs to it and use the interface as a virtual com port. The board
is also less expensive than the USB to serial board we have used in the past with the Microstick II or PIC18F4525.

### Pins Used

![](images/pins.png)

* UART1 is at 115200 baud. Tx1 is on RC6.
* UART2 is connected to the XPRESS boards USB interface PIC. 
    * Communication between UART2 and the interface IC is at 9600 baud.
* roam input used to determine if robot should
    1. Stay in place (perhaps for programming)
	2. Roam (perhaps following a line)

## Pololu 3Pi robot

The robot is running the serial slave program from Pololu. This will allow the 3Pi robot to be 
controlled from a XPRESS board.

* [10.a. Serial slave program](https://www.pololu.com/docs/0J21/all#10.a)

More information on the Pololu 3Pi robot

* [https://www.pololu.com/product/975](https://www.pololu.com/product/975)

## Mounting PIC XPRESS board on 3Pi Expansion board

### Mount 1

The first XPRESS board mount was forward on the 3Pi expansion board. This appeared to have a negative effect on the balance and stability of
the robot. For a second mount position was tested.

![](images/xpress-mount1.jpg)

### Mount 2

For better stability the XPRESS board was moved over the wheels.

![](images/expansion-mounted-2-s.jpg)
  
Wiring under the XPRESS board.

![](images/expansion-wire-s.jpg)

## Roam and No Roam Shunt Jumper

The image below shows the jumper in the no roam possition.
  
![](images/no_roam_s.jpg)

## Autocalibrates when in Roam mode

Will autocalibrate the sensors if in Roam mode. 
For meaningful results the robot should be sitting over a black tape line. The robot will spin to the left and right sweeping 
the sensors over the line so that normalized readings can be calculated.

## Read Sensors

Code was added to read the sensors while the **slave** is executing proportional derivative control. 

* see **Proportional Derivative Control in Roam mode** section down below.

Unlike the **test-read-sensors** branch the sensor values are not sent to PuTTY.

Currently in the main branch far left and far right sensor are checked to see if they are greater than 500. If either are greater than
500 it is assumed a line was seen on the far left or far right and this causes the robot to exit proportional derivative control. It just
stops and does nothing in that case. 

Note that this is simply a test to ensure that the slave can successfully both run proportional derivative control and report sensor 
readings back to the PIC on the Xpress board.


In this branch the test2_PORT signals are ignored! The following was used only in the **test-read-sensors** branch:

**test2_PORT** was used to provide timing to a DSO so that the time required to read all 5 sensors and load that data into the
PIC18F46K42 could be measured. The time was **2.46 mS**.

The test points are shown in this image:

![](images/time-test-point.jpg)

The timing signal from test2_PORT on the Digital Storage Oscilloscope:

![](images/time-read-sensors.jpg)

## Proportional Derivative Control in Roam mode

Another function has been added to allow the MCU on the main board of the 3Pi robot to run proportional derivative code 
when the robot is in Roam mode. The robot follows sharp turns and curves but knows nothing of gaps and other APSC1299 
special obstacles. This could possibly work fine for demos. 

## Pull up on RX2/RB7

A 10 Kohm pull up resistor was added to RX2 for better reliability when USB cable is not attached.  
  
## Charging Circuit

A charging circuit has been added to the expantion board. The charging circuit is just a 75 ohm resistor in series with the four AAA
NiMH cells on the robot. A 9 volt external AC-DC adaptor provides power to the board.

![](images/charging-sm.jpg)

Showing the 75 ohm 2 watt resistor (located under the expansion board.

![](images/charging-75ohm-sm.jpg)

**Will update photos. Had to move one jumper to top side of board as it caused rubbing on right wheel. Also bend tab on barrow connector
so it is flush on board before soldering.**
  
## Added Print Sensor Values to menu

Can now print sensor values in PuTTY when in No Roam mode. Can move robot over track by hand.  
  
## Code Configurator settings

The code in this project was primarily generated by the Code Configurator. The settings used may not be optimal at all. 
The initial goal was just to get the UART of the target PIC to talk to the UART of the interface PIC.

## Working with PuTTY and issues

One can use a PuTTY terminal with the virtual serial port of the Xpress board. This works fine when one is typing into the 
terminal. There is an issue though if one attempts pasting into the PuTTY terminal (using a right mouse click). In that case
only the first character is sent. This is an issue of the USB to serial bridge on the Xpress board and not the PIC code!
This was verified by using a USB to serial bridge on a different board (TTLyFTDI USB-to-TTL Cable Adapter) fed into RB7. In this case the code worked as expected
and all pasted characters appeared in the PuTTY terminal and were correctly sent out uart1 TX.

![uart1-uart2.jpg](images/uart1-uart2.jpg)

Others have commented on the limitation of the USART to USB bridge on the Xpress board:

* [Xpress PIC18F46K42 board virtual COM port bridge to UART receive limitations](https://www.microchip.com/forums/m1097510.aspx)

## Test of Expansion Board

Initial testing of expansion board for 3PI.

![](images/test-expansion-b.jpg)
