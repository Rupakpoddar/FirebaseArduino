/*
  ---------------------------------
  IMPORTANT: Configuration Reminder
  ---------------------------------
  
  Before running this code, make sure to check the "secrets.h" file
  for important configuration details such as Wi-Fi credentials and 
  Firebase settings.

  The "secrets.h" file should include:
  - Your Wi-Fi SSID and Password
  - Your Firebase Realtime Database URL
  - (OPTIONAL) Firebase Authentication Token

  Ensure that "secrets.h" is properly configured and includes the correct
  information for your project. Failure to do so may result in connection
  errors or incorrect behavior of your application.

  Note: The "secrets.h" file should be located in the same directory as
  this sketch.
*/

/*
  ---------------------------------
  INFO: WebNest Setup & Usage Guide  
  ---------------------------------

  WebNest is a rapid, straightforward solution for implementing home automation
  or remote device control using Firebase or a local server. It allows for quick
  prototyping and real-time updates without extensive setup or configuration.

  To get started with WebNest, follow these steps to set up and configure
  your devices and connect them with Firebase for real-time control and monitoring.

  1. Visit the official WebNest site:
     Go to https://rupakpoddar.github.io/WebNest/

  2. Configure the Service URL:
     - Click the gear icon in the top right corner to open the setup dialog.
     - In the "Service URL/IP" field, paste your Firebase Realtime Database Reference URL.
     - Select "Update" to apply the settings.

  3. Add Devices:
     - Click the "+" button on the top right corner to add a new device.
     - For this example, enter the device name as "Ambience Light" (case-sensitive, without quotes).
       Ensure the name matches exactly, including spaces and capitalization.
     - Choose the style for the controls (ON/OFF, Toggle, etc.) and select additional features like 
       Color Picker, Temperature, and PWM as needed.

  4. Add Another Device:
     - Repeat the process above to add another device, and name it "Pedestal Fan".
     - Again, ensure that the device name matches exactly as provided.

  5. Firebase Configuration:
     - Ensure that your Firebase Realtime Database is in "Test Mode" for proper read/write access
       during development. This allows unrestricted access without requiring authentication.

  6. Upload to Board:
     - Once you've completed the setup, configure your code properly (see secrets.h for Firebase 
       and Wi-Fi credentials) and upload this sketch to your board.
     - Once the board is running, you will see the device states getting updated in real-time on the WebNest interface.  
*/

#include "secrets.h"
#include "WebNestDevice.h"
#include <Firebase.h>

/* WebNest only supports Firebase in Test Mode */
Firebase fb(REFERENCE_URL);

/* ----- Create WebNest devices ----- */
WebNestDevice* ambienceLight = new WebNestDevice("Ambience Light");
WebNestDevice* pedestalFan = new WebNestDevice("Pedestal Fan");

void setup() {
  Serial.begin(115200);
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

  #if defined(ARDUINO_UNOWIFIR4)
    digitalWrite(LED_BUILTIN, HIGH);
  #endif
}

void loop() {
  /* ----- Poll and update the device states from Firebase ----- */
  WebNestDevice::poll(fb.getJson("" /* Get Everything */));

  /* ----- Print updated values ----- */
  Serial.println("Device Name\t\tState\tTemp\tPWM\tColor");

  // For Ambience Light
  Serial.print(ambienceLight->getName());
  Serial.print("\t\t");
  Serial.print(ambienceLight->getState());
  Serial.print("\t");
  Serial.print(ambienceLight->getTemperature());
  Serial.print("\t");
  Serial.print(ambienceLight->getPWM());
  Serial.print("\t");
  Serial.println(ambienceLight->getColor(), HEX);

  // For Pedestal Fan
  Serial.print(pedestalFan->getName());
  Serial.print("\t\t");
  Serial.print(pedestalFan->getState());
  Serial.print("\t");
  Serial.print(pedestalFan->getTemperature());
  Serial.print("\t");
  Serial.print(pedestalFan->getPWM());
  Serial.print("\t");
  Serial.println(pedestalFan->getColor(), HEX);

  Serial.println();
  delay(1000);
}
