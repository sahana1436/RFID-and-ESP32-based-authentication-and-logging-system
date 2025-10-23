#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>          // NEW: For Wi-Fi connectivity
#include <HTTPClient.h>    // NEW: For making web requests

// --- WIFI CONFIGURATION ---
// !!! IMPORTANT: Update these with your Wi-Fi credentials !!!
const char* ssid = "wifi name";
const char* password = "wifi password";

// --- GOOGLE APPS SCRIPT ENDPOINT ---
// !!! IMPORTANT: REPLACE THIS URL with your deployed Google Apps Script URL !!!
const String APPS_SCRIPT_URL = "WRITE YOUR GOOGLE APP SCRIPT URL HERE";

// --- ESP32 PIN Definitions (Using separated pins) ---
// Define pins for MFRC522 (SPI)
#define RST_PIN 5    // RST (Reset) connected to ESP32 GPIO 5
#define SS_PIN 17    // SDA (Slave Select) connected to ESP32 GPIO 17 

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance

// Define I2C pins for the OLED
#define OLED_SDA 21 // SDA pin for OLED
#define OLED_SCL 22 // SCL pin for OLED

// OLED Display parameters
#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 50
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- AUTHORIZATION DATA (Removed for sheet-based authentication) ---
// Note: Local authorization is now replaced by the check within the Google Apps Script.
// The list below is commented out as it's no longer the source of truth.
/*
byte authorizedUIDs[][4] = {
    {0xB3, 0xBE, 0x68, 0x27} // ONLY Card 1: GRANTED ACCESS
};
const int NUM_AUTHORIZED_CARDS = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);
*/


// --- HELPER FUNCTIONS ---

/**
 * @brief Sends the scanned UID to the Google Apps Script for authentication and logging.
 * @param uid_str The UID of the card as a Hex string (e.g., "B3BE6827").
 * @return The final access status string ("GRANTED" or "DENIED") received from the script.
 */
String sendDataToSheet(String uid_str) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send data.");
        return "DENIED (NO WIFI)";
    }

    HTTPClient http;
    // Construct the full URL with the UID parameter
    String url = APPS_SCRIPT_URL + "?uid=" + uid_str;
    http.begin(url);
    
    Serial.print("[HTTP] Sending UID to Apps Script: ");
    Serial.println(url);

    int httpResponseCode = http.GET();
    String response = "DENIED (SERVER ERROR)"; // Default fallback

    if (httpResponseCode > 0) {
        Serial.print("[HTTP] Response code: ");
        Serial.println(httpResponseCode);
        
        // The script is designed to return "GRANTED" or "DENIED" directly
        response = http.getString();
        response.trim(); // Remove any leading/trailing whitespace
        Serial.print("[HTTP] Server Response: ");
        Serial.println(response);

        // Simple check for access status
        if (response.equals("GRANTED")) {
             return "GRANTED";
        } else {
             return "DENIED";
        }
    } else {
        Serial.print("[HTTP] Error on connection: ");
        Serial.println(httpResponseCode);
    }
    
    http.end();
    return response;
}

/**
 * @brief Displays a single, centered message on the OLED screen (e.g., initial status).
 */
void displayMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(2); // Medium text
  display.setTextColor(WHITE);
  // Center the message
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
  display.println(message);
  display.display();
}

/**
 * @brief Displays the authentication status and the full scanned UID on the OLED.
 */
void showDetailedResult(const char* status, String uid_str) {
    display.clearDisplay();
    
    // Line 1: Access Status (e.g., GRANTED/DENIED) - Big Text
    display.setCursor(0, 0);
    display.setTextSize(2); 
    display.setTextColor(WHITE);
    display.println(status);
    
    // Separator line
    display.drawFastHLine(0, 20, SCREEN_WIDTH, WHITE);

    // Line 3: UID Label - Small Text
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println("UID:");

    // Line 4: The Actual UID in Hex - Medium Text
    display.setTextSize(1); // Use smaller text size to fit the whole UID string
    display.setCursor(0, 40);
    display.println(uid_str);
    
    // Line 5: Instruction
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.println("Remove Card to Reset");

    display.display();
}

/**
 * @brief Connects the ESP32 to Wi-Fi.
 */
void connectToWiFi() {
    displayMessage("Connecting...");
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        displayMessage("WiFi OK");
        delay(1000);
    } else {
        Serial.println("\nWiFi Failed! Rebooting...");
        displayMessage("WiFi Failed");
        delay(3000);
        ESP.restart();
    }
}


void setup() {
  Serial.begin(115200); 
  Serial.println("--- ESP32 RFID IoT System Initializing ---");

  // Initialize I2C bus with specific pins for OLED
  Wire.begin(OLED_SDA, OLED_SCL); 
  
  // Initialize SPI bus for RFID
  SPI.begin();       
  rfid.PCD_Init();    

  // OLED Initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed. Check wiring/address."));
    for(;;); 
  }
  
  connectToWiFi(); // Connect to Wi-Fi immediately
  
  // Initial Display Message
  displayMessage("Scan a Card");
}

void loop() {
  // Ensure Wi-Fi stays connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost WiFi connection. Attempting to reconnect...");
    displayMessage("Reconnecting");
    connectToWiFi();
    displayMessage("Scan a Card");
    return;
  }
  
  // Check for new cards/tags
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50);
    return; // No new card or unable to read serial
  }
  
  // Convert scanned UID to a single Hex string (e.g., "B3BE6827")
  String uid_hex_str = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) { 
      uid_hex_str += "0"; 
    }
    uid_hex_str += String(rfid.uid.uidByte[i], HEX);
  }
  uid_hex_str.toUpperCase();
  
  // --- Serial Monitor Output (The Output Screen) ---
  Serial.print("\n--- NEW SCAN DETECTED ---");
  Serial.print("\nScanned UID (Hex String): ");
  Serial.println(uid_hex_str);
  
  // --- Authentication and Logging Logic (via HTTP) ---
  displayMessage("Checking...");
  
  // Send UID to Google Apps Script and get status back
  String status_result = sendDataToSheet(uid_hex_str);
  
  const char* statusMessage;
  if (status_result.equals("GRANTED")) {
    statusMessage = "GRANTED";
    Serial.println("-> ACCESS GRANTED!");
  } else {
    statusMessage = "DENIED";
    Serial.println("-> ACCESS DENIED!");
  }

  // --- OLED Display Output (Both UID and Status) ---
  showDetailedResult(statusMessage, uid_hex_str);

  // Halt communication with the card and pause
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(3000); // Display result for 3 seconds
  
  // Reset display to waiting message
  displayMessage("Scan a Card");
}
