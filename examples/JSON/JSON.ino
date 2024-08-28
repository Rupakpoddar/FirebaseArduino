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
      INFO: ArduinoJson library   
  ---------------------------------

  Download ArduinoJson library from the Library Manager:
  https://www.arduino.cc/reference/en/libraries/arduinojson/

  For guidance on serialization and deserialization, visit:
  https://arduinojson.org/v7/assistant/
*/

#include "secrets.h"
#include <Firebase.h>
#include <ArduinoJson.h>

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

  /* ----- Serialization: Set example data in Firebase ----- */

  // Create a JSON document to hold the output data
  JsonDocument docOutput;

  // Add various data types to the JSON document
  docOutput["myString"] = "Hello World!";
  docOutput["myInt"] = 123;
  docOutput["myFloat"] = 45.67;
  docOutput["myBool"] = true;

  // Create a string to hold the serialized JSON data
  String output;

  // Optional: Shrink the JSON document to fit its contents exactly
  docOutput.shrinkToFit();

  // Serialize the JSON document to a string
  serializeJson(docOutput, output);

  // Set the serialized JSON data in Firebase
  fb.setJson("Example", output);

  /* ----- Deserialization: Retrieve example data from Firebase ----- */

  // Retrieve the serialized JSON data from Firebase
  String input = fb.getJson("Example");

  // Check if the retrieved data is "NULL", indicating a retrieval error
  if (input == "NULL") {
    Serial.println("Could not retrieve data from Firebase");
  } else {
    // Create a JSON document to hold the deserialized data
    JsonDocument docInput;

    // Deserialize the JSON string into the JSON document
    DeserializationError error = deserializeJson(docInput, input);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // Extract the values from the deserialized JSON document
    const char* retrievedString = docInput["myString"]; // "Hello World!"
    int retrievedInt = docInput["myInt"];               // 123
    float retrievedFloat = docInput["myFloat"];         // 45.67
    bool retrievedBool = docInput["myBool"];            // true

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

  // Remove the example data from Firebase
  fb.remove("Example");
}

void loop() {
  // Nothing
}
