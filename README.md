# RFID-KIRBY

This is a project involving reading and writing RFID cards using the M5Cardputer and the RFID2 module.

## About

This code was created by Mateus Fonseca in 2024 for the RFID-KIRBY project.

## Dependencies

- **Wire.h**: Library for I2C communication.
- **MFRC522_I2C.h**: Library for the RFID2 module.
- **kirbybmp.h**: Bitmap file for the boot screen.
- **M5Cardputer.h**: Library for the M5Cardputer.
- **SD.h**: Library for communication with the SD card.

## Features

- **Read Mode**: Allows reading information from RFID cards.
- **Write Mode**: Allows writing information to RFID cards.
- **TAB Button**: Toggles between read and write modes.

## Initial Setup

- The screen is initialized with a boot image of Kirby.
- The program displays the version and name of the application.
- Initializes the RFID2 module.
- Initializes the SD card.

## How to Use

- In read mode, hold an RFID card near the reader to view the card's information.
- In write mode, the previously read information will be written to the card, hold the card near the reader to write it.
- Use the TAB button to toggle between read and write modes.

## Notes

- Make sure the RFID2 module is properly inserted.
- This code is specific to the M5Cardputer and the RFID2 module.
