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
  - (OPTIONAL) Firebase Authentication Token

  Ensure that "secrets.h" is properly configured and includes the correct
  information for your project. Failure to do so may result in connection
  errors or incorrect behavior of your application.
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

    /* ===== JSON SERIALIZATION: CREATE AND SEND DATA ===== */
    
    Serial.println("Creating JSON data...");
    
    /*
      For guidance on serialization and deserialization, visit:
      https://arduinojson.org/v7/assistant/
    */

    // Create a JSON document to hold the output data
    JsonDocument docOutput;

    // Add various data types to the JSON document
    docOutput["myString"] = "Hello World!";
    docOutput["myInt"] = 123;
    docOutput["myFloat"] = 45.67;
    docOutput["myBool"] = true;

    // You can also create nested objects
    JsonObject sensor = docOutput.createNestedObject("sensor");
    sensor["temperature"] = 25.6;
    sensor["humidity"] = 60.3;
    sensor["status"] = "active";

    // Create a string to hold the serialized JSON data
    String output;

    // Optional: Shrink the JSON document to fit its contents exactly
    docOutput.shrinkToFit();

    // Serialize the JSON document to a string
    serializeJson(docOutput, output);

    Serial.println("JSON data created:");
    Serial.println(output);
    Serial.println();

    // Set the serialized JSON data in Firebase
    Serial.println("Sending JSON to Firebase...");
    int responseCode = fb.setJson("Example", output);
    Serial.print("Set JSON - Response Code: ");
    Serial.println(responseCode);

    if (responseCode == 200) {
        Serial.println("JSON data successfully sent to Firebase!");
    } else {
        Serial.println("Failed to send JSON data to Firebase!");
        Serial.print("Response code ");
        Serial.print(responseCode);
        Serial.println(" indicates an error occurred.");
        return; // Exit if we can't send data
    }

    Serial.println();

    /* ===== JSON DESERIALIZATION: RETRIEVE AND PARSE DATA ===== */
    
    Serial.println("Retrieving JSON from Firebase...");

    // Retrieve the serialized JSON data from Firebase
    String input;
    responseCode = fb.getJson("Example", input);
    Serial.print("Get JSON - Response Code: ");
    Serial.println(responseCode);

    // Check if the retrieval was successful
    if (responseCode != 200) {
        Serial.println("Could not retrieve data from Firebase");
        Serial.print("Response code ");
        Serial.print(responseCode);
        Serial.println(" indicates an error occurred.");
        Serial.println("This might be due to:");
        Serial.println("- Network connectivity issues");
        Serial.println("- Incorrect Firebase URL or path");
        Serial.println("- Authentication problems");
        return;
    }

    Serial.println("JSON data retrieved:");
    Serial.println(input);
    Serial.println();

    // Create a JSON document to hold the deserialized data
    JsonDocument docInput;

    // Deserialize the JSON string into the JSON document
    DeserializationError error = deserializeJson(docInput, input);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    Serial.println("Parsing JSON data...");

    // Extract the values from the deserialized JSON document
    const char* retrievedString = docInput["myString"];   // "Hello World!"
    int retrievedInt = docInput["myInt"];                 // 123
    float retrievedFloat = docInput["myFloat"];           // 45.67
    bool retrievedBool = docInput["myBool"];              // true

    // Extract nested object values
    float temperature = docInput["sensor"]["temperature"]; // 25.6
    float humidity = docInput["sensor"]["humidity"];       // 60.3
    const char* status = docInput["sensor"]["status"];     // "active"

    /* Print the deserialized data */
    Serial.println("--- Parsed Data ---");
    Serial.print("Retrieved String: ");
    Serial.println(retrievedString);
    Serial.print("Retrieved Int: ");
    Serial.println(retrievedInt);
    Serial.print("Retrieved Float: ");
    Serial.println(retrievedFloat);
    Serial.print("Retrieved Bool: ");
    Serial.println(retrievedBool);
    
    Serial.println("--- Sensor Data ---");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Status: ");
    Serial.println(status);

    Serial.println();

    /* ===== CLEANUP ===== */
    
    Serial.println("Cleaning up...");
    
    // Remove the example data from Firebase
    responseCode = fb.remove("Example");
    Serial.print("Remove Example - Response Code: ");
    Serial.println(responseCode);

    if (responseCode == 200) {
        Serial.println("Example data successfully removed from Firebase!");
    } else {
        Serial.println("Failed to remove example data from Firebase!");
        Serial.print("Response code ");
        Serial.print(responseCode);
        Serial.println(" indicates an error occurred.");
    }

    Serial.println();
    Serial.println("JSON example completed!");
}

void loop() {
    // Nothing to do here
}
