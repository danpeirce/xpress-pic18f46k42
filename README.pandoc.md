% Testing PIC18F46K42 XPRESS Board
% by Dan Peirce B.Sc.

<!---
use 
pandoc -s --toc -t html5 -c pandocbd.css README.pandoc.md -o index.html

pandoc -s --toc -t gfm README.pandoc.md -o README.md
-->

# Testing PIC18F46K42 XPRESS Board

This branch of the project was set up to to allow the on board UART to serial bridge to be used by a separate circuit. To 
facilitate this both RC6 abd RC7 are set as inputs and the program then just goes into an enless loop.

![](DIP-PIC-Xpress.jpg)

## Board Features

This board has a USB microB connector. It has a PIC MCU on board configured to act as an interface and it enumerates as 
a multifunction device. This allows one to both download programs to it as a mass storage device and use the interface as 
a virtual com port. 

### Pins Used

![](images/pins.png)

* UART1 is at 115200 baud. Tx1 is on RC6.
* UART2 is connected to the XPRESS boards USB interface PIC. 
    * Communication between UART2 and the interface IC is at 9600 baud.

### Solder Bumps

Solder Bump for 3.3 volts removed.

![](images/solder-bump-removed.jpg)

Solder Bump for 5 volts added.

![](images/solder-bump-added.jpg)



