

<div class="p-6 md:p-10 lg:p-12">

<h1 class="text-4xl font-extrabold text-gray-900 mb-2 flex items-center">
<span class="mr-3">💳</span> ESP32 RFID IoT Access & Logging System
</h1>
<p class="text-xl text-gray-600 mb-8">
A real-time access control solution built with an ESP32 microcontroller, an MFRC522 RFID reader, and a Google Sheets backend for centralized authentication and logging via Wi-Fi.
</p>

<!-- Overview -->

<div class="card bg-white p-6 mb-8">
<h2 class="text-2xl font-bold text-gray-800 mb-3 border-b pb-2">💡 Project Overview</h2>
<p class="text-gray-700 leading-relaxed">
This system provides a robust, cloud-integrated method for authenticating users. When an RFID tag is scanned, the ESP32 connects via Wi-Fi to a Google Apps Script endpoint. The script checks the tag's UID against an authorized list in a User Details Sheet and then logs the entire transaction (timestamp, UID, status) in a separate Logs Sheet. The outcome ("GRANTED" or "DENIED") is instantly displayed on the attached OLED screen.
</p>
</div>

<!-- Components -->

<h2 class="text-2xl font-bold text-gray-800 mb-4 mt-8">🛠️ Components Required</h2>
<div class="overflow-x-auto card">
<table class="min-w-full text-left text-sm font-light shadow-md">
<thead class="table-header">
<tr>
<th scope="col" class="px-6 py-3">Component</th>
<th scope="col" class="px-6 py-3">Description</th>
<th scope="col" class="px-6 py-3">Role</th>
</tr>
</thead>
<tbody>
<tr class="table-row-odd">
<td class="px-6 py-4 font-medium">ESP32 Development Board</td>
<td class="px-6 py-4">Microcontroller with integrated Wi-Fi.</td>
<td class="px-6 py-4">Reads data, connects to internet, controls display.</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-4 font-medium">MFRC522 RFID Reader</td>
<td class="px-6 py-4">Reads the unique ID (UID) from tags.</td>
<td class="px-6 py-4">Interface for physical authentication.</td>
</tr>
<tr class="table-row-odd">
<td class="px-6 py-4 font-medium">0.96" OLED Display (I2C)</td>
<td class="px-6 py-4">Small display module.</td>
<td class="px-6 py-4">Displays status (Connecting, Checking, GRANTED/DENIED).</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-4 font-medium">Breadboard & Wires</td>
<td class="px-6 py-4">Prototyping tools.</td>
<td class="px-6 py-4">Connects the components.</td>
</tr>
</tbody>
</table>
</div>

<div class="h-8"></div>

<!-- Wiring Guide -->

<h2 class="text-2xl font-bold text-gray-800 mb-4 mt-8">🔌 Wiring and Connections (Updated Pins)</h2>
<div class="alert-warning mb-4">
<p>&#9888; Note: The pins used in the <code class="inline-code">FINAL_CODE_INTEGRATED.ino</code> are specific and differ from some common schematics. Please follow the table below exactly.</p>
</div>

<!-- Physical Wiring Diagram Integration -->

<div class="my-6 p-4 border rounded-lg bg-gray-50">
<p class="text-center font-semibold mb-2 text-xl">1. Physical Setup and Wiring Connections</p>
<p>"https://drive.google.com/file/d/1CjbkilMulPmuYmdqzegZh-EatnOVB7-A/view?usp=sharing"</p>
</div>

<!-- MFRC522 Table -->

<h3 class="text-xl font-semibold text-gray-700 mb-2 mt-6">MFRC522 RFID Reader (SPI)</h3>
<div class="overflow-x-auto card mb-6">
<table class="min-w-full text-left text-sm font-light shadow-md">
<thead class="table-header bg-blue-700">
<tr>
<th scope="col" class="px-6 py-3">Component Pin</th>
<th scope="col" class="px-6 py-3">ESP32 Pin (GPIO)</th>
<th scope="col" class="px-6 py-3">Protocol</th>
<th scope="col" class="px-6 py-3">Note</th>
</tr>
</thead>
<tbody>
<tr class="table-row-odd">
<td class="px-6 py-2 font-medium">RST</td>
<td class="px-6 py-2">GPIO 5</td>
<td class="px-6 py-2">SPI</td>
<td class="px-6 py-2">Reset Pin (<code class="inline-code">RST_PIN</code>)</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-2 font-medium">SDA (SS)</td>
<td class="px-6 py-2">GPIO 17</td>
<td class="px-6 py-2">SPI</td>
<td class="px-6 py-2">Slave Select (<code class="inline-code">SS_PIN</code>)</td>
</tr>
<tr class="table-row-odd">
<td class="px-6 py-2">MOSI</td>
<td class="px-6 py-2">GPIO 23</td>
<td class="px-6 py-2">SPI</td>
<td class="px-6 py-2">Default SPI MOSI</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-2">MISO</td>
<td class="px-6 py-2">GPIO 19</td>
<td class="px-6 py-2">SPI</td>
<td class="px-6 py-2">Default SPI MISO</td>
</tr>
<tr class="table-row-odd">
<td class="px-6 py-2">SCK</td>
<td class="px-6 py-2">GPIO 18</td>
<td class="px-6 py-2">SPI</td>
<td class="px-6 py-2">Default SPI SCK</td>
</tr>
<tr class="table-row-even bg-red-100">
<td class="px-6 py-2 font-medium text-red-700">3.3V</td>
<td class="px-6 py-2 text-red-700">3.3V</td>
<td class="px-6 py-2">Power</td>
<td class="px-6 py-2 text-red-700 font-bold">MUST use 3.3V</td>
</tr>
<tr class="table-row-odd">
<td class="px-6 py-2">GND</td>
<td class="px-6 py-2">GND</td>
<td class="px-6 py-2">Ground</td>
<td class="px-6 py-2"></td>
</tr>
</tbody>
</table>
</div>

<!-- OLED Table -->

<h3 class="text-xl font-semibold text-gray-700 mb-2 mt-6">OLED Display (I2C)</h3>
<div class="overflow-x-auto card mb-6">
<table class="min-w-full text-left text-sm font-light shadow-md">
<thead class="table-header bg-green-700">
<tr>
<th scope="col" class="px-6 py-3">Component Pin</th>
<th scope="col" class="px-6 py-3">ESP32 Pin (GPIO)</th>
<th scope="col" class="px-6 py-3">Protocol</th>
<th scope="col" class="px-6 py-3">Note</th>
</tr>
</thead>
<tbody>
<tr class="table-row-odd">
<td class="px-6 py-2">SCL</td>
<td class="px-6 py-2">GPIO 22</td>
<td class="px-6 py-2">I2C</td>
<td class="px-6 py-2">I2C Clock</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-2">SDA</td>
<td class="px-6 py-2">GPIO 21</td>
<td class="px-6 py-2">I2C</td>
<td class="px-6 py-2">I2C Data</td>
</tr>
<tr class="table-row-odd">
<td class="px-6 py-2">VCC</td>
<td class="px-6 py-2">3.3V or 5V</td>
<td class="px-6 py-2">Power</td>
<td class="px-6 py-2">Check module specs</td>
</tr>
<tr class="table-row-even">
<td class="px-6 py-2">GND</td>
<td class="px-6 py-2">GND</td>
<td class="px-6 py-2">Ground</td>
<td class="px-6 py-2"></td>
</tr>
</tbody>
</table>
</div>

<!-- Backend Setup -->

<h2 class="text-2xl font-bold text-gray-800 mb-4 mt-8">☁️ Google Sheets & Apps Script Backend Setup</h2>

<h3 class="text-xl font-semibold text-gray-700 mb-2">Step 1: Create Google Sheets</h3>
<p class="text-gray-700 mb-4">You need two separate Google Sheets documents in your Drive:</p>
<ul class="list-disc list-inside ml-4 text-gray-700 space-y-2">
<li>
User Details Sheet (Authentication Source):
<ul class="list-circle list-inside ml-6">
<li>Create a sheet named <code class="inline-code">Users</code>.</li>
<li>Column A must contain the Hex UID of all authorized RFID tags (e.g., <code class="inline-code">B3BE6827</code>).</li>
</ul>
</li>
<li>
Logs Sheet (Activity Log):
<ul class="list-circle list-inside ml-6">
<li>Create a sheet named <code class="inline-code">Logs</code>.</li>
<li>This sheet will automatically receive data (Timestamp, UID, Status).</li>
</ul>
</li>
</ul>

<!-- Google Sheet Log Example -->

<div class="my-6 p-4 border rounded-lg bg-gray-50">
<p class="text-center font-semibold mb-2 text-xl">2. Example of Automated Log Entry in Google Sheet</p>
<p>"https://drive.google.com/file/d/1EUEJeJ81H8Gz1kxVjTnK9uXzTkJhzBQr/view?usp=sharing"</p>
</div>

<h3 class="text-xl font-semibold text-gray-700 mb-2 mt-6">Step 2: Deploy Google Apps Script (Code 3)</h3>
<ol class="list-decimal list-inside ml-4 space-y-3 text-gray-700">
<li>Open either of your Google Sheets, go to Extensions > Apps Script.</li>
<li>Copy the Apps Script code (provided in your project) into the <code class="inline-code">Code.gs</code> file.</li>
<li>Update the configuration constants in the script with the IDs of your two sheets (the ID is the long string in the Sheet's URL).
<div class="code-block mt-2">
<pre>// Details to be filled by the user
const USER_DETAILS_SHEET_ID = 'YOUR_USER_DETAILS_SHEET_ID_HERE';
const LOGS_SHEET_ID = 'YOUR_LOGS_SHEET_ID_HERE';
const USER_DETAILS_SHEET_NAME = 'Users';
const LOGS_SHEET_NAME = 'Logs';</pre>
</div>
</li>
<li>Click Deploy > New Deployment.</li>
<li>Set Type to Web app.</li>
<li>Set Execute as to Me.</li>
<li>Set Who has access to Anyone.</li>
<li>Click Deploy and copy the Web App URL. This is your <code class="inline-code">APPS_SCRIPT_URL</code> needed for the ESP32 code.</li>
</ol>

<!-- Arduino IDE Setup -->

<h2 class="text-2xl font-bold text-gray-800 mb-4 mt-8">💻 Arduino IDE Setup</h2>

<h3 class="text-xl font-semibold text-gray-700 mb-2">Step 1: Install Libraries</h3>
<p class="text-gray-700 mb-4">Install the following libraries using the Arduino Library Manager (Sketch > Include Library > Manage Libraries):</p>
<ul class="list-disc list-inside ml-4 text-gray-700">
<li>MFRC522 by Udo Klein</li>
<li>Adafruit GFX Library by Adafruit</li>
<li>Adafruit SSD1306 by Adafruit</li>
</ul>

<h3 class="text-xl font-semibold text-gray-700 mb-2 mt-6">Step 2: Configure and Upload Code</h3>
<ol class="list-decimal list-inside ml-4 space-y-3 text-gray-700">
<li>Ensure the file is saved correctly: <code class="inline-code">FINAL_CODE_INTEGRATED.ino</code> must be inside a folder named <code class="inline-code">FINAL_CODE_INTEGRATED</code> (matching the file name).</li>
<li>Open the <code class="inline-code">.ino</code> file and update the three configuration constants at the top with your credentials and deployed URL:
<div class="code-block mt-2">
<pre>// --- WIFI CONFIGURATION ---
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// --- GOOGLE APPS SCRIPT ENDPOINT ---
const String APPS_SCRIPT_URL = "PASTE_YOUR_DEPLOYED_GOOGLE_APP_SCRIPT_URL_HERE";</pre>

</div>
</li>
<li>Select your board (ESP32 Dev Module) and the correct Port.</li>
<li>Click Upload.</li>

</ol>

<!-- Execution Flow -->

<h2 class="text-2xl font-bold text-gray-800 mb-4 mt-8">🚀 System Execution Flow (Pattern)</h2>

<div class="flex flex-wrap lg:flex-nowrap gap-6">
<div class="w-full lg:w-1/2">
<h3 class="text-xl font-semibold text-gray-700 mb-2">Device Side</h3>
<ol class="list-decimal list-inside ml-4 space-y-3 text-gray-700 card p-4 bg-blue-50">
<li>Initialization: ESP32 connects to Wi-Fi. OLED shows "WiFi OK" then "Scan a Card".</li>
<li>Scan: User taps an RFID tag. MFRC522 reads the Hex UID (e.g., <code class="inline-code">B3BE6827</code>).</li>
<li>Request: ESP32 constructs an HTTP request: <code class="inline-code">APPS_SCRIPT_URL?uid=B3BE6827</code>. OLED shows "Checking...".</li>
<li>Response: ESP32 receives the status back from the script ("GRANTED" or "DENIED").</li>
<li>Display: OLED shows the final status and the UID.</li>
</ol>
</div>
<div class="w-full lg:w-1/2">
<h3 class="text-xl font-semibold text-gray-700 mb-2">Backend Side (Apps Script)</h3>
<ol class="list-decimal list-inside ml-4 space-y-3 text-gray-700 card p-4 bg-green-50">
<li>Input: Script receives the UID parameter from the ESP32.</li>
<li>Authentication: Checks the UID against the <code class="inline-code">Users</code> sheet. Determines if access is GRANTED or DENIED.</li>
<li>Logging: Appends a new row to the <code class="inline-code">Logs</code> sheet with the timestamp, UID, and determined status.</li>
<li>Output: Returns the final status string ("GRANTED" or "DENIED") as plain text to the ESP32.</li>
</ol>
</div>
</div>

