  - [Testing PIC18F46K42 XPRESS
    Board](#testing-pic18f46k42-xpress-board)
      - [Board Features](#board-features)
          - [Pins Used](#pins-used)
          - [Solder Bumps](#solder-bumps)
      - [Connecting UART1 Tx to USB to Serial Adaptor to Test
        Board](#connecting-uart1-tx-to-usb-to-serial-adaptor-to-test-board)
      - [Working with PuTTY and
        limitations](#working-with-putty-and-limitations)
      - [After Build Copy](#after-build-copy)

<!---
use 
pandoc -s --toc -t html5 -c pandocbd.css README.pandoc.md -o index.html

pandoc -s --toc -t gfm README.pandoc.md -o README.md
-->

# Testing PIC18F46K42 XPRESS Board

This branch of the project was set up to do a simple UART test of the
two UARTs on the PIC18F46K42 XPRESS board.

## Board Features

This board has a USB microB connector. It has a PIC MCU on board
configured to act as an interface and it enumerates as a multi-function
device. This allows one to both download programs to it as a mass
storage device and use the interface as a virtual com port.

### Pins Used

![](images/pins.png)

## Connecting UART1 Tx to USB to Serial Adaptor to Test Board

![](images/uart-forward-2-1.jpg)

  - UART1 is at 115200 baud. Tx1 is on RC6.
  - UART2 is connected to the XPRESS boards USB interface PIC.
      - Communication between UART2 and the interface IC is at 115200
        baud but the interface PIC has a default speed of 9600 baud.
        The baud rate must be reset to 115200:
          - every time the target is programmed and
          - every time the reset button on the Xpress board is pressed.

![](images/terminal-uart2-115200-1.png)

![](images/terminal-uart2-115200-2.png)

![](images/terminal-uart2-115200-3.png)

![](images/terminal-uart2-115200-4.png)

### Solder Bumps

Solder Bump for 3.3 volts removed.

![](images/solder-bump-removed.jpg)

Solder Bump for 5 volts added.

![](images/solder-bump-added.jpg)

## Working with PuTTY and limitations

One can use a PuTTY terminal with the virtual serial port of the Xpress
board. This works fine when one is typing into the terminal. There is an
issue though if one attempts pasting into the PuTTY terminal (using a
right mouse click). In that case only the first character is sent. This
is an issue of the USB to serial bridge on the Xpress board and not the
PIC code\! This was verified by using a USB to serial bridge on a
different board (TTLyFTDI USB-to-TTL Cable Adapter) fed into RB7. In
this case the code worked as expected and all pasted characters appeared
in the PuTTY terminal and were correctly sent out uart1 TX.

![uart1-uart2.jpg](images/uart1-uart2.jpg)

Others have commented on the limitation of the USART to USB bridge on
the Xpress board:

  - [Xpress PIC18F46K42 board virtual COM port bridge to UART receive
    limitations](https://www.microchip.com/forums/m1097510.aspx)

## After Build Copy

Added after build execute option.

    copy C:\Users\danp\MPLABXProjects\xpress-pic18f46k42\dist\default\production\xpress-pic18f46k42.production.hex E:\output.hex /y

  - the output path will depend on the computer and operating system

![](images/after-build.png)
