#include <Firebase.h>

#define _SSID "ENTER HERE"          // Your WiFi SSID
#define _PASSWORD "ENTER HERE"      // Your WiFi Password
#define REFERENCE_URL "ENTER HERE"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

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

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  #if defined(ARDUINO_UNOWIFIR4)
    digitalWrite(LED_BUILTIN, HIGH);
  #endif

//================================================================//
//================================================================//

  // Examples of setting String, integer and float values.
  firebase.setString("Example/setString", "It's Working");
  firebase.setInt("Example/setInt", 123);
  firebase.setFloat("Example/setFloat", 45.32);
  firebase.setBool("Example/setBool", true);

  // Examples of pushing String, integer and float values.
  firebase.pushString("push", "Hello");
  firebase.pushInt("push", 789);
  firebase.pushFloat("push", 89.54);
  firebase.pushBool("push", false);

  // Example of getting a String.
  String data1 = firebase.getString("Example/setString");
  Serial.print("Received String:\t");
  Serial.println(data1);

  // Example of getting an int.
  int data2 = firebase.getInt("Example/setInt");
  Serial.print("Received Int:\t\t");
  Serial.println(data2);

  // Example of getting a float.
  float data3 = firebase.getFloat("Example/setFloat");
  Serial.print("Received Float:\t\t");
  Serial.println(data3);

  // Example of getting a boolean.
  bool data4 = firebase.getBool("Example/setBool");
  Serial.print("Received Bool:\t\t");
  Serial.println(data4);

  // Example of data deletion.
  firebase.remove("Example");
}

void loop() {
  // Nothing
}
