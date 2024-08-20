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

#include <ArduinoJson.h>  // https://www.arduino.cc/reference/en/libraries/arduinojson/
#include <Firebase.h>
#include "secrets.h"

/* Use the following instance for Test Mode (No Authentication) */
Firebase fb(REFERENCE_URL);

/* Use the following instance for Locked Mode (With Authentication) */
// Firebase fb(REFERENCE_URL, AUTH_TOKEN);

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

  /* ----- */ 

  /* Set example data in Firebase to be retrieved later */
  fb.setString("Example/setString", "Hello World!");
  fb.setInt("Example/setInt", 123);
  fb.setFloat("Example/setFloat", 45.67);
  fb.setBool("Example/setBool", true);

  /* Retrieve example data from Firebase */
  String input = fb.getString("Example");
  if (input == "NULL") {
    Serial.println("Could not retrieve data from Firebase");
  } else {
    /* Deserialize the retrieved input */
    /* Refer to: https://arduinojson.org/v7/assistant */
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* retrievedString = doc["setString"]; // "Hello World!"
    int retrievedInt = doc["setInt"];               // 123
    float retrievedFloat = doc["setFloat"];         // 45.67
    bool retrievedBool = doc["setBool"];            // true

    /* Print the deserialized input */
    Serial.print("Retrieved String:\t");
    Serial.println(retrievedString);
    Serial.print("Retrieved Int:\t\t");
    Serial.println(retrievedInt);
    Serial.print("Retrieved Float:\t");
    Serial.println(retrievedFloat);
    Serial.print("Retrieved Bool:\t\t");
    Serial.println(retrievedBool);
  }

  /* Remove example data from Firebase */
  fb.remove("Example");
}

void loop() {
  // Nothing
}
