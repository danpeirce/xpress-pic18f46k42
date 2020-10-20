% Testing PIC18F46K42 XPRESS Board
% by Dan Peirce B.Sc.

<!---
use 
pandoc -s --toc -t html5 -c pandocbd.css README.pandoc.md -o index.html

pandoc -s --toc -t gfm README.pandoc.md -o README.md
-->

## Testing programs in the DIP PIC18F46K42 IC

This branch of the project was set up to test the feasibility of using a DIP version of the PIC18F46K42 as the target device
and connecting to the Xpress board as a USB to serial adaptor. The reasons for doing this:
 
* to allow one to try features of the PIC18F46K42 that are incompatible with the Xpress board. The incompatibility arises if a feature 
	requires RB7 to be no-load. On the Xpress board the output from the UART to serial bridge is connected to RB7 through a 220 Ω resistor.

**To see an unloaded output the programmer must be removed!**

![](images/dip-pic-dac1.jpg)

![](images/dac1-output.png)

## Pins Used

### DIP PIC18F46K42

![](images/dip-pins.png)

* DAC1OUT2 is at pin 40. **To see an unloaded output the programmer must be removed!**
* UART1 is at 115200 baud. 
    * Tx1 is on RC6. 
	* Rx1 is on RC7
* UART2 is connected to the XPRESS boards USB interface PIC. 
    * Communication between UART2 and the interface IC is at 9600 baud.
	* Tx2 is on RD6
	* Rx2 is on RD7

### Xpress Board PIC

More information on the setup of the Xpress board can be found in the [branch dummy-xpress](https://github.com/danpeirce/xpress-pic18f46k42/tree/dummy-xpress)

![](images/pins.png)

#### Solder Bumps

Solder Bump for 3.3 volts removed.

![](images/solder-bump-removed.jpg)

Solder Bump for 5 volts added.

![](images/solder-bump-added.jpg)


## Working with PuTTY and limitations

One can use a PuTTY terminal with the virtual serial port of the Xpress board. This works fine when one is typing into the 
terminal. There is an issue though if one attempts pasting into the PuTTY terminal (using a right mouse click). In that case
only the first character is sent. This is an issue of the USB to serial bridge on the Xpress board and not the PIC code!
This was verified by using a USB to serial bridge on a different board (TTLyFTDI USB-to-TTL Cable Adapter) fed into RB7. In this case the code worked as expected
and all pasted characters appeared in the PuTTY terminal and were correctly sent out uart1 TX.

![uart1-uart2.jpg](images/uart1-uart2.jpg)

Others have commented on the limitation of the USART to USB bridge on the Xpress board:

* [Xpress PIC18F46K42 board virtual COM port bridge to UART receive limitations](https://www.microchip.com/forums/m1097510.aspx)

## After Build Copy

Added after build execute option.

~~~~
copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y
~~~~

* the output path will depend on the computer and operating system

![](images/after-build.png)