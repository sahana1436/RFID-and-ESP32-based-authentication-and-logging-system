💳 ESP32 RFID IoT Access & Logging System

A real-time access control solution built with an ESP32 microcontroller, an MFRC522 RFID reader, and a Google Sheets backend for centralized authentication and logging via Wi-Fi.

💡 Project Overview

This system provides a robust, cloud-integrated method for authenticating users. When an RFID tag is scanned, the ESP32 connects via Wi-Fi to a Google Apps Script endpoint. The script checks the tag's UID against an authorized list in a User Details Sheet and then logs the entire transaction (timestamp, UID, status) in a separate Logs Sheet. The outcome ("GRANTED" or "DENIED") is instantly displayed on the attached OLED screen.

🛠️ Components Required

Component

Description

Role

ESP32 Development Board

Microcontroller with integrated Wi-Fi.

Reads data, connects to internet, controls display.

MFRC522 RFID Reader

Reads the unique ID (UID) from tags.

Interface for physical authentication.

0.96" OLED Display (I2C)

Small display module.

Displays status (Connecting, Checking, GRANTED/DENIED).

Breadboard & Wires

Prototyping tools.

Connects the components.

🔌 Wiring and Connections (Updated Pins)

⚠️ Note: The pins used in the FINAL_CODE_INTEGRATED.ino are specific and differ from some common schematics. Please follow the table below exactly.

MFRC522 RFID Reader (SPI)

Component Pin

ESP32 Pin (GPIO)

Protocol

Note

RST

GPIO 5

SPI

Reset Pin (RST_PIN)

SDA (SS)

GPIO 17

SPI

Slave Select (SS_PIN)

MOSI

GPIO 23

SPI

Default SPI MOSI

MISO

GPIO 19

SPI

Default SPI MISO

SCK

GPIO 18

SPI

Default SPI SCK

3.3V

3.3V

Power

MUST use 3.3V

GND

GND

Ground



OLED Display (I2C)

Component Pin

ESP32 Pin (GPIO)

Protocol

Note

SCL

GPIO 22

I2C

I2C Clock

SDA

GPIO 21

I2C

I2C Data

VCC

3.3V or 5V

Power

Check module specs

GND

GND

Ground



☁️ Google Sheets & Apps Script Backend Setup

Step 1: Create Google Sheets

You need two separate Google Sheets documents in your Drive:

User Details Sheet (Authentication Source):

Create a sheet named Users.

Column A must contain the Hex UID of all authorized RFID tags (e.g., B3BE6827).

Logs Sheet (Activity Log):

Create a sheet named Logs.

This sheet will automatically receive data (Timestamp, UID, Status).

Step 2: Deploy Google Apps Script (Code 3)

Open either of your Google Sheets, go to Extensions > Apps Script.

Copy the Apps Script code (provided in your project) into the Code.gs file.

Update the configuration constants in the script with the IDs of your two sheets (the ID is the long string in the Sheet's URL).

// Details to be filled by the user
const USER_DETAILS_SHEET_ID = 'YOUR_USER_DETAILS_SHEET_ID_HERE';
const LOGS_SHEET_ID = 'YOUR_LOGS_SHEET_ID_HERE';
const USER_DETAILS_SHEET_NAME = 'Users';
const LOGS_SHEET_NAME = 'Logs';


Click Deploy > New Deployment.

Set Type to Web app.

Set Execute as to Me.

Set Who has access to Anyone.

Click Deploy and copy the Web App URL. This is your APPS_SCRIPT_URL needed for the ESP32 code.

💻 Arduino IDE Setup

Step 1: Install Libraries

Install the following libraries using the Arduino Library Manager (Sketch > Include Library > Manage Libraries):

MFRC522 by Udo Klein

Adafruit GFX Library by Adafruit

Adafruit SSD1306 by Adafruit

Step 2: Configure and Upload Code

Ensure the file is saved correctly: FINAL_CODE_INTEGRATED.ino must be inside a folder named FINAL_CODE_INTEGRATED (matching the file name).

Open the .ino file and update the three configuration constants at the top with your credentials and deployed URL:

// --- WIFI CONFIGURATION ---
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// --- GOOGLE APPS SCRIPT ENDPOINT ---
const String APPS_SCRIPT_URL = "PASTE_YOUR_DEPLOYED_GOOGLE_APP_SCRIPT_URL_HERE";


Select your board (ESP32 Dev Module) and the correct Port.

Click Upload.

🚀 System Execution Flow (Pattern)

Device Side

Initialization: ESP32 connects to Wi-Fi. OLED shows "WiFi OK" then "Scan a Card".

Scan: User taps an RFID tag. MFRC522 reads the Hex UID (e.g., B3BE6827).

Request: ESP32 constructs an HTTP request: APPS_SCRIPT_URL?uid=B3BE6827. OLED shows "Checking...".

Response: ESP32 receives the status back from the script ("GRANTED" or "DENIED").

Display: OLED shows the final status and the UID.

Backend Side (Apps Script)

Input: Script receives the UID parameter from the ESP32.

Authentication: Checks the UID against the Users sheet. Determines if access is GRANTED or DENIED.

Logging: Appends a new row to the Logs sheet with the timestamp, UID, and determined status.

Output: Returns the final status string ("GRANTED" or "DENIED") as plain text to the ESP32.
