// details to be filled by the user
const USER_DETAILS_SHEET_ID = 'Place your user details sheet ID here i.e. the users to be authenticated within the system'; 
const LOGS_SHEET_ID = 'The sheet id of the logs sheet shich the iot based device will update';    
const USER_DETAILS_SHEET_NAME = 'Users'; // Name the google docs -->sheet as "user"
const LOGS_SHEET_NAME = 'Logs';// Name the google docs -->sheet as "user"
// details to be filled by the user


function doGet(request) {
  const scannedUID = request.parameter.uid;
  if (!scannedUID) {
    return ContentService.createTextOutput("DENIED (No UID provided)");
  }

  const timestamp = new Date();
  let status = "DENIED";
  
  // 1. Authentication: Check the User Details sheet
  try {
    const userDetailsSs = SpreadsheetApp.openById(USER_DETAILS_SHEET_ID);
    const userDetailsSheet = userDetailsSs.getSheetByName(USER_DETAILS_SHEET_NAME);
    
    // Assuming UID is in column A and Status (e.g., 'Allowed') is in column B
    // We fetch all UIDs to check against
    const dataRange = userDetailsSheet.getRange('A:A');
    const uids = dataRange.getValues().map(row => row[0].toString().toUpperCase());
    
    // Check if the scanned UID exists in the authorized list
    if (uids.includes(scannedUID)) {
      status = "GRANTED";
    }
  } catch (e) {
    // If the authentication sheet fails to open, deny access for security
    Logger.log("Authentication sheet error: " + e.toString());
    status = "DENIED (AUTH ERROR)";
  }

  // 2. Logging: Update the Logs sheet
  try {
    const logsSs = SpreadsheetApp.openById(LOGS_SHEET_ID);
    const logsSheet = logsSs.getSheetByName(LOGS_SHEET_NAME);
    
    // Append a new row to the Logs sheet: Timestamp, UID, Status
    logsSheet.appendRow([
      timestamp.toLocaleString(),
      scannedUID,
      status
    ]);
  } catch (e) {
    Logger.log("Logging sheet error: " + e.toString());
  }

  // 3. Send the final status back to the ESP32
  return ContentService.createTextOutput(status);
}
