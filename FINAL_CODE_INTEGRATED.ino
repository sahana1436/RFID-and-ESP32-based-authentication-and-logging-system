#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>          
#include <HTTPClient.h>    

// --- WIFI CONFIGURATION ---
// !!! IMPORTANT: Update these with your Wi-Fi credentials !!!
const char* ssid = "wifi name";
const char* password = "wifi password";

// --- GOOGLE APPS SCRIPT ENDPOINT ---
// !!! IMPORTANT: REPLACE THIS URL with your deployed Google Apps Script URL !!!
const String APPS_SCRIPT_URL = "WRITE YOUR GOOGLE APP SCRIPT URL HERE";

// --- ESP32 PIN 
#define RST_PIN 5    // RST (Reset) connected to ESP32 GPIO 5
#define SS_PIN 17    // SDA (Slave Select) connected to ESP32 GPIO 17 

MFRC522 rfid(SS_PIN, RST_PIN); 

#define OLED_SDA 21 // SDA pin for OLED
#define OLED_SCL 22 // SCL pin for OLED

#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 50
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String sendDataToSheet(String uid_str) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send data.");
        return "DENIED (NO WIFI)";
    }

    HTTPClient http;
    String url = APPS_SCRIPT_URL + "?uid=" + uid_str;
    http.begin(url);
    
    Serial.print("[HTTP] Sending UID to Apps Script: ");
    Serial.println(url);

    int httpResponseCode = http.GET();
    String response = "DENIED (SERVER ERROR)"; 

    if (httpResponseCode > 0) {
        Serial.print("[HTTP] Response code: ");
        Serial.println(httpResponseCode);
        
        response = http.getString();
        response.trim(); 
        Serial.print("[HTTP] Server Response: ");
        Serial.println(response);

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

void displayMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(2); 
  display.setTextColor(WHITE);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
  display.println(message);
  display.display();
}


void showDetailedResult(const char* status, String uid_str) {
    display.clearDisplay();
    
    display.setCursor(0, 0);
    display.setTextSize(2); 
    display.setTextColor(WHITE);
    display.println(status);
    
    display.drawFastHLine(0, 20, SCREEN_WIDTH, WHITE);

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println("UID:");

    display.setTextSize(1); 
    display.setCursor(0, 40);
    display.println(uid_str);
    
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.println("Remove Card to Reset");

    display.display();
}

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

  Wire.begin(OLED_SDA, OLED_SCL); 
  
  SPI.begin();       
  rfid.PCD_Init();    
    
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed. Check wiring/address."));
    for(;;); 
  }
  
  connectToWiFi(); 
  displayMessage("Scan a Card");
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost WiFi connection. Attempting to reconnect...");
    displayMessage("Reconnecting");
    connectToWiFi();
    displayMessage("Scan a Card");
    return;
  }
  

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50);
    return; 
  }
  
  String uid_hex_str = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) { 
      uid_hex_str += "0"; 
    }
    uid_hex_str += String(rfid.uid.uidByte[i], HEX);
  }
  uid_hex_str.toUpperCase();
  
  Serial.print("\n--- NEW SCAN DETECTED ---");
  Serial.print("\nScanned UID (Hex String): ");
  Serial.println(uid_hex_str);
  
  displayMessage("Checking...");
  
  String status_result = sendDataToSheet(uid_hex_str);
  
  const char* statusMessage;
  if (status_result.equals("GRANTED")) {
    statusMessage = "GRANTED";
    Serial.println("-> ACCESS GRANTED!");
  } else {
    statusMessage = "DENIED";
    Serial.println("-> ACCESS DENIED!");
  }

  showDetailedResult(statusMessage, uid_hex_str);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(3000); 
  

  displayMessage("Scan a Card");
}
