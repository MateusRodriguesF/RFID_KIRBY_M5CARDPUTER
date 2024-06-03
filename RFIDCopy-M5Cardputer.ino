/**
 * @file RFIDCopy-M5Cardputer.ino
 * @author Mateus Fonseca
 * @brief https://github.com/MateusRodriguesF/RFID_KIRBY_M5CARDPUTER
 * @version 1.0
 * @date 05-05-2024
 *
 * @Hardwares: M5Cardputer
 * @Platform Version: Arduino M5Stack Board Manager
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * WIRE: 
 * MFRC522_I2C: 
 * M5Cardputer: https://github.com/m5stack/M5Cardputer
 **/

#include <Wire.h>
#include "MFRC522_I2C.h"
#include "kirbybmp.h"
#include <M5Cardputer.h>
#include <SD.h> // Add the SD.h library

MFRC522 mfrc522(0x28); // Create an instance of MFRC522.
enum state {
  read_mode, // Read mode
  write_mode // Write mode
} currentState;

bool readUID = false;

byte UID[20];
uint8_t UIDLength = 0;

void bootScreen(){
  //boot Screen
  M5Cardputer.Lcd.drawBitmap(0, 0, kirbybmpWidth, kirbybmpHeight, kirbybmp); //(kirbybmp, 32400)
  delay(3000);
}
// Function to display read mode
void displayReadMode() {
  M5Cardputer.Display.setTextSize(1.5); // Reduce text size
  M5Cardputer.Display.println(); // Add a line break before the line
  M5Cardputer.Display.println(F("  --- RFID-KIRBY-v1.0---")); // Display the application name and version
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("(TAB to switch to write mode)")); // Instruction to switch to write mode
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("         Ready to READ...")); // Message indicating that the device is ready to read
  M5Cardputer.Display.println(); // Add a line break before the line
}

// Function to display write mode
void displayWriteMode() {
  M5Cardputer.Display.setTextColor(BLUE);
  M5Cardputer.Display.setTextSize(1.5); // Reduce text size
  M5Cardputer.Display.println(); // Add a line break before the line
  M5Cardputer.Display.println(F("  --- RFID-KIRBY-v1.0---")); // Display the application name and version
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("(TAB to switch to read mode)")); // Instruction to switch to read mode
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("        Ready to WRITE...")); // Message indicating that the device is ready to write
  M5Cardputer.Display.println(); // Add a line break
  displayUID(); // Display the Unique Identifier (UID)
}
// Function to display Intro
void displayIntro() {
  // Text scrolls up
  M5Cardputer.Display.setTextColor(GREEN);
  M5Cardputer.Display.setTextSize(2); // Reduce text size
  for (int i = 8; i >= 0; i--) { // Starts from 7 and decrements to 0
    M5Cardputer.Display.fillScreen(BLACK); // Fill the screen with black color
    M5Cardputer.Display.setCursor(0, i * 16); // Set the cursor position for the new line
    M5Cardputer.Display.println(); // Add a line break
    M5Cardputer.Display.println(); // Add a line break
    M5Cardputer.Display.println(F("  RFID-KIRBY V1.0")); // Display the application name and version
    M5Cardputer.Display.println(); // Add a line break
    M5Cardputer.Display.println(F(" Coded by MATEUSRF ")); // Instruction to switch to read mode
    M5Cardputer.Display.println(F("        2024 ")); // Instruction to switch to read mode
    delay(500); // Wait for 0.5 seconds
  }
  delay(500);
  cls();
  //Essa parte não funcionou como eu esperava
  // Text scrolls down
  //   M5Cardputer.Display.setTextColor(GREEN);
  //   M5Cardputer.Display.setTextSize(2); // Reduce text size
  // for (int i = 0; i < 8; i++) {
  //   M5Cardputer.Display.fillScreen(BLACK); // Fill the screen with black color
  //   M5Cardputer.Display.setCursor(0, i * 16); // Set the cursor position for the new line
  //   M5Cardputer.Display.println(); // Add a line break
  //   M5Cardputer.Display.println(); // Add a line break
  //   M5Cardputer.Display.println(F("  RFID-KIRBY V0.1")); // Display the application name and version
  //   M5Cardputer.Display.println(); // Add a line break
  //   M5Cardputer.Display.println(F(" Coded by MATEUSRF ")); // Instruction to switch to read mode
  //   M5Cardputer.Display.println(F("        2024 ")); // Instruction to switch to read mode
  //   delay(500); // Wait for 0.5 seconds
  // }
  // delay(500);
  // cls();
}

// Function to emit an attention beep
void beep_attention() {
  M5Cardputer.Speaker.tone(882, 40);
  delay(100);
}

// Function to emit an error beep
void beep_error() {
  M5Cardputer.Speaker.tone(510, 60);
  delay(60);
  M5Cardputer.Speaker.tone(495, 60);
  delay(60);
  M5Cardputer.Speaker.tone(520, 60);
  delay(60);
}

// Function to emit a successful write beep
void beep_writeOK() {
  M5Cardputer.Speaker.tone(661, 60);
  delay(60);
  M5Cardputer.Speaker.tone(882, 100);
  delay(100);
}

// Function to clear the screen
void cls() {
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.fillScreen(BLACK); // Fill the screen with black color
  M5Cardputer.Display.setCursor(0, 0); // Set the cursor to the initial position
}

void setup() {
  M5Cardputer.begin(); // Initialize the M5Cardputer
  M5Cardputer.Display.setRotation(1); // Rotate the display by 90 degrees
  M5Cardputer.Display.fillScreen(BLACK); // Fill the screen with black color
  M5Cardputer.Display.setCursor(0, 0); // Set the cursor to the initial position
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C communication
  mfrc522.PCD_Init(); // Initialize the MFRC522 module
  currentState = read_mode; // Set the current state to read mode
  bootScreen();
  displayIntro();
  displayReadMode(); // Display the read mode
  if (!SD.begin()) { // Initialize the SD card
    M5Cardputer.Display.println("Failed to initialize SD card");
    while (true); // Halt here if initialization fails
  }
}

  
void loop() {
  static unsigned long lastKeyPressTime = 0; // Stores the last time a key was pressed
  const unsigned long debounceDelay = 200; // Debounce delay in milliseconds
  M5Cardputer.update(); // Update M5Cardputer to handle keyboard inputs

  // Check for keyboard input changes and ensure enough time has passed since the last key press
  if (M5Cardputer.Keyboard.isChange() && (millis() - lastKeyPressTime > debounceDelay)) {
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      // Check if the Tab key is pressed
      if (status.tab) {
        cls(); // Clear the screen
        // Toggle between modes
        switch (currentState) {
          case read_mode:
            currentState = write_mode;
            displayWriteMode();
            break;
          case write_mode:
            currentState = read_mode;
            displayReadMode();
            readUID = false; // Reset UID reading flag
            break;
        }
        lastKeyPressTime = millis(); // Update the last key press time
      }
    }
  }

  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  cls();

  switch (currentState) {
    case read_mode:
      displayReadMode();
      readCard();
      break;
    case write_mode:
      displayWriteMode();
      writeCard();
      break;
  }

  mfrc522.PICC_HaltA();
}

// Function to read the card
void readCard() {
  MFRC522::PICC_Type piccType = (MFRC522::PICC_Type)mfrc522.PICC_GetType(mfrc522.uid.sak);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.print(F("Card Type: "));
  M5Cardputer.Display.println(mfrc522.PICC_GetTypeName(piccType)); // Display the card type

  M5Cardputer.Display.print(F("Serial Number(UID): ")); // Display the serial number (UID)
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    M5Cardputer.Display.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    M5Cardputer.Display.print(mfrc522.uid.uidByte[i], HEX);
  }
  M5Cardputer.Display.println();
  M5Cardputer.Display.print(F("Protocol Version (SAK): ")); // Display the protocol version (SAK)
  M5Cardputer.Display.println(mfrc522.uid.sak);
  // Display the RFID card frequency
  M5Cardputer.Display.print(F("Card Frequency (MHz): "));
  M5Cardputer.Display.println(mfrc522.PCD_GetAntennaGain());
  // Display other additional information as needed...
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.setTextSize(1.5); // Reduce text size
    M5Cardputer.Display.setCursor(0, 120); // Move the error message down
    M5Cardputer.Display.println(F("Not a MIFARE Classic"));
    M5Cardputer.Display.setCursor(0, 0); // Reset the cursor
    M5Cardputer.Display.setTextColor(YELLOW);
    beep_error();
    delay(1000);
  } else {
    M5Cardputer.Display.println("");
    readUID = true;
    UIDLength = mfrc522.uid.size;
    for (byte i = 0; i < UIDLength; i++) {
      UID[i] = mfrc522.uid.uidByte[i];
    }
    displayUID();
    beep_attention();
    delay(1000);
    saveInfoToSD(mfrc522.uid.uidByte, mfrc522.uid.size, piccType, mfrc522.uid.sak); // Call the function to save to SD card
    M5Cardputer.Display.println(); // Add a line break before the line
    M5Cardputer.Display.println(); // Add a line break before the line
    M5Cardputer.Display.println(F("File saved to SD card!"));
    M5Cardputer.Speaker.tone(661, 100);
    delay(100);
  }
}

// Function to display the UID
void displayUID() {
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("User ID:"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  for (byte i = 0; i < UIDLength; i++) {
    M5Cardputer.Display.print(UID[i] < 0x10 ? " 0" : " ");
    M5Cardputer.Display.print(UID[i], HEX);
  }
}

// Function to write to the card
void writeCard() {
  if (mfrc522.MIFARE_SetUid(UID, (byte)UIDLength, true)) {
    M5Cardputer.Display.println();
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.println(F("UID written."));
    beep_writeOK();
  } else {
    // cls();
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.setTextSize(1.5); // Reduce text size
    M5Cardputer.Display.setCursor(0, 120); // Move the error message down
    M5Cardputer.Display.println(F("   Write failed!!"));
    // M5Cardputer.Display.setCursor(0, 0); // Reset the cursor
    // M5Cardputer.Display.setTextColor(YELLOW);
    beep_error();
  }

  mfrc522.PICC_HaltA();
  delay(1000);
}
// Function to save information to the SD card
void saveInfoToSD(byte* UID, uint8_t UIDLength, MFRC522::PICC_Type piccType, byte sak) {
  // Check if the folder exists, if not, create it
  if (!SD.exists("/SavedCards")) {
    if (!SD.mkdir("/SavedCards")) {
      M5Cardputer.Display.println("Error creating folder on SD card");
      return;
    }
  }

  // Generate the file name based on the card UID
  char fileName[25]; // Define size as needed
  sprintf(fileName, "/SavedCards/%02X%02X%02X%02X.txt", UID[0], UID[1], UID[2], UID[3]);

  // Open the file for writing
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    // Write the information to the file
    dataFile.println("RFID Card Information:");
    dataFile.print("Card Type: ");
    dataFile.println(mfrc522.PICC_GetTypeName(piccType));
    dataFile.print("Serial Number (UID): ");
    for (byte i = 0; i < UIDLength; i++) {
      dataFile.print(UID[i] < 0x10 ? " 0" : " ");
      dataFile.print(UID[i], HEX);
    }
    dataFile.println();
    dataFile.print("Protocol Version (SAK): ");
    dataFile.println(sak);
    dataFile.println("------------------------------");
    dataFile.close();
  } else {
    M5Cardputer.Display.println("Error opening file on SD card");
  }
}
