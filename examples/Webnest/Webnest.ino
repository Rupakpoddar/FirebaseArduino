/*
  ---------------------------------
  IMPORTANT: CONFIGURATION REQUIRED
  ---------------------------------
  
  Before running this code, make sure to check the "secrets.h" file
  for important configuration details such as WiFi credentials and 
  Firebase settings.

  The "secrets.h" file should include:
  - Your WiFi SSID and Password
  - Your Firebase Realtime Database URL

  Ensure that "secrets.h" is properly configured and includes the correct
  information for your project. Failure to do so may result in connection
  errors or incorrect behavior of your application.
*/

/*
  ---------------------------------
  INFO: Webnest Setup & Usage Guide  
  ---------------------------------

  Webnest is a modern home automation dashboard that allows for real-time
  device control using Firebase. It provides an intuitive interface for
  managing smart home devices with support for on/off control and slider
  functionality.

  To get started with Webnest, follow these steps:

  1. Visit the official Webnest site:
     Go to https://rupakpoddar.github.io/Webnest/

  2. Configure Firebase URL:
     - Enter your Firebase Realtime Database URL in the header input field
     - The input border will turn blue when connected successfully

  3. Add Devices:
     - Click the "Add Device" button
     - Enter a device name (e.g., "Living Room Light")
     - Select an icon (icon1.png through icon5.png)
     - Enable slider if you want brightness/speed control (0-100%)
     - Click "Add Device" to save

  4. Firebase Configuration:
     - Ensure your Firebase Realtime Database is in "Test Mode" for proper
       read/write access during development
     - Devices are automatically stored under the /webnest key in Firebase

  5. Upload to Board:
     - Configure your secrets.h file with WiFi and Firebase credentials
     - Upload this sketch to your board
     - The board will sync with Firebase in real-time
*/

#include "secrets.h"
#include "WebnestDevice.h"
#include <Firebase.h>

/* Webnest only supports Firebase in Test Mode (no authentication required) */
Firebase fb(REFERENCE_URL);

/* ----- Create Webnest Device Objects ----- */
WebnestDevice* livingRoomLight = new WebnestDevice("Living Room Light");
WebnestDevice* bedroomFan = new WebnestDevice("Bedroom Fan");

// Optional: Define pins for physical device control
// #define LIGHT_PIN 5

void setup() {
    Serial.begin(115200);
    
    // Board-specific initialization
    #if !defined(ARDUINO_UNOWIFIR4)
        WiFi.mode(WIFI_STA);
    #else
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
    #endif
    
    WiFi.disconnect();
    delay(1000);

    /* Connect to WiFi */
    Serial.println();
    Serial.println();
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("-");
        delay(500);
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.println();

    // Turn on built-in LED for UNO R4 WiFi
    #if defined(ARDUINO_UNOWIFIR4)
        digitalWrite(LED_BUILTIN, HIGH);
    #endif

    /* Test Firebase connection */
    Serial.println("Testing Firebase connection...");
    String testData;
    int responseCode = fb.getString("webnest", testData);
    
    if (responseCode != 200) {
        Serial.print("Warning: Could not connect to Firebase. Response code: ");
        Serial.println(responseCode);
        Serial.println("Make sure your Firebase Realtime Database is in Test Mode.");
    } else {
        Serial.println("Firebase connection successful!");
    }
    
    Serial.println();

    /* Optional: Setup physical device pins */
    // pinMode(LIGHT_PIN, OUTPUT);
}

void loop() {
    /* ----- Poll Firebase for device updates ----- */
    String deviceData;
    int responseCode = fb.getJson("webnest", deviceData);
    
    // Check if data retrieval was successful
    if (responseCode != 200) {
        Serial.print("Failed to retrieve device data from Firebase. Response code: ");
        Serial.println(responseCode);
        delay(5000); // Wait 5 seconds before retrying
        return;
    }

    // Update all device states with the retrieved data
    WebnestDevice::poll(deviceData);

    /* ----- Display device status ----- */
    Serial.println("Device Name\t\tState\tSlider\tValue");
    Serial.println("---------------------------------------------");

    // Living Room Light
    Serial.print(livingRoomLight->getName());
    Serial.print("\t");
    Serial.print(livingRoomLight->getState() ? "ON" : "OFF");
    Serial.print("\t");
    Serial.print(livingRoomLight->getSliderEnabled() ? "Yes" : "No");
    Serial.print("\t");
    Serial.println(livingRoomLight->getSliderValue());

    // Bedroom Fan
    Serial.print(bedroomFan->getName());
    Serial.print("\t\t");
    Serial.print(bedroomFan->getState() ? "ON" : "OFF");
    Serial.print("\t");
    Serial.print(bedroomFan->getSliderEnabled() ? "Yes" : "No");
    Serial.print("\t");
    Serial.println(bedroomFan->getSliderValue());

    Serial.println();

    /* Example: Control physical devices based on state */
    // if (livingRoomLight->getState()) {
    //   // Turn on the actual light
    //   // analogWrite(LIGHT_PIN, map(livingRoomLight->getSliderValue(), 0, 100, 0, 255));
    // }

    // Wait 1 second before next update
    delay(1000);
}
