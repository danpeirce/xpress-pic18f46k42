-   [Testing PIC18F46K42 XPRESS
    Board](#testing-pic18f46k42-xpress-board)
    -   [Pins Used](#pins-used)
    -   [Variants of Sensor Board](#variants-of-sensor-board)
    -   [Working with PuTTY and
        limitations](#working-with-putty-and-limitations)
    -   [After Build Copy](#after-build-copy)

<!---
use 
pandoc -s --toc -t html5 -c pandocbd.css README.pandoc.md -o index.html

pandoc -s --toc -t gfm README.pandoc.md -o README.md
-->

Testing PIC18F46K42 XPRESS Board
================================

This branch of the project was set up to test a reflective sensor
connected to the PIC18F46K42 XPRESS board.

Pins Used
---------

![](images/pins.png)

-   The reflective sensor is connected to R\_SENSOR (RD1).
-   UART2 is connected to the XPRESS boards USB interface PIC.
    -   Communication between UART2 and the interface IC is at 9600
        baud.

Variants of Sensor Board
------------------------

There are two variants of the QTR-1xx reflective sensor.

1.  QTR-1A is intended to be an analogue board. On left in image below.
2.  QTR-1RC is intended connected to a general purpose I/O pin.

![](images/compare-ccts.png)

Working with PuTTY and limitations
----------------------------------

One can use a PuTTY terminal with the virtual serial port of the Xpress
board. This works fine when one is typing into the terminal. There is an
issue though if one attempts pasting into the PuTTY terminal (using a
right mouse click). In that case only the first character is sent. This
is an issue of the USB to serial bridge on the Xpress board and not the
PIC code!

Others have commented on the limitation of the USART to USB bridge on
the Xpress board:

-   [Xpress PIC18F46K42 board virtual COM port bridge to UART receive
    limitations](https://www.microchip.com/forums/m1097510.aspx)

After Build Copy
----------------

Added after build execute option.

Note the path varies depending on where the project is located.

    copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y

-   the output path will depend on the computer and operating system

![](images/after-build.png)
