/*
  ---------------------------------
      INFO: ArduinoJson library   
  ---------------------------------

  Download ArduinoJson library from the Library Manager:
  https://www.arduino.cc/reference/en/libraries/arduinojson/
*/

#include <Arduino.h>
#include <ArduinoJson.h>

#define MAX_DEVICES 16

class WebNestDevice {
  public:
    WebNestDevice(String name);  // Constructor
    ~WebNestDevice();            // Destructor

    // Public Getters
    String getName();            
    uint32_t getColor();
    int getState();
    int getPWM();
    int getTemperature();

    // Static poll method to update devices using a JSON string
    static void poll(const String& jsonString);

  private:
    String name;
    uint32_t color;
    byte state;
    byte pwm;
    byte temperature;

    // Private Setters (only accessible within the class)
    void setName(String name);
    void setColor(uint32_t color);
    void setState(int state);
    void setPWM(int pwm);
    void setTemperature(int temperature);

    static WebNestDevice* deviceList[MAX_DEVICES];  // Array of pointers to WebNestDevice
};

// Static member definition
WebNestDevice* WebNestDevice::deviceList[MAX_DEVICES] = {NULL};  // Initialize all to NULL

// Constructor: Initializes the device and adds it to the list
WebNestDevice::WebNestDevice(String name) {
  this->setName(name);
  this->setColor(0xFFE0B2);
  this->setState(0);
  this->setPWM(50);
  this->setTemperature(50);

  // Add the device to the list (find the first empty spot)
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (deviceList[i] == NULL) {
      deviceList[i] = this;
      break;
    }
  }
}

// Destructor: Removes the device from the list
WebNestDevice::~WebNestDevice() {
  // Remove the device from the list
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (deviceList[i] == this) {
      deviceList[i] = NULL;
      break;
    }
  }
}

// Private Setters
void WebNestDevice::setName(String name) {
  this->name = name;
}

void WebNestDevice::setColor(uint32_t color) {
  this->color = color;
}

void WebNestDevice::setState(int state) {
  this->state = state;
}

void WebNestDevice::setPWM(int pwm) {
  this->pwm = pwm;
}

void WebNestDevice::setTemperature(int temperature) {
  this->temperature = temperature;
}

// Public Getters
String WebNestDevice::getName() {
  return name;
}

uint32_t WebNestDevice::getColor() {
  return color;
}

int WebNestDevice::getState() {
  return state;
}

int WebNestDevice::getPWM() {
  return pwm;
}

int WebNestDevice::getTemperature() {
  return temperature;
}

// Static poll method: Parses the JSON string and updates device states
void WebNestDevice::poll(const String& jsonString) {
  if (jsonString == "NULL") {
    Serial.println("Could not retrieve data from Firebase");
  } else {
    // Create a JSON document
    JsonDocument doc;

    // Deserialize the JSON string into the JSON document
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // Loop through each device in the list
    for (int i = 0; i < MAX_DEVICES; i++) {
      if (deviceList[i] != NULL) {
        String deviceName = deviceList[i]->getName();
        
        // Check if the JSON object has an entry for this device
        if (doc.containsKey(deviceName)) {
          JsonObject deviceData = doc[deviceName];

          // Update the Color if present in JSON
          if (deviceData.containsKey("Color")) {
            String colorStr = deviceData["Color"];
            uint32_t colorValue = strtoul(colorStr.c_str(), NULL, 16);  // Convert hex string to uint32_t
            deviceList[i]->setColor(colorValue);
          }

          // Update the State if present in JSON
          if (deviceData.containsKey("State")) {
            int stateValue = deviceData["State"];
            deviceList[i]->setState(stateValue);
          }

          // Update the PWM if present in JSON
          if (deviceData.containsKey("PWM")) {
            int pwmValue = deviceData["PWM"];
            deviceList[i]->setPWM(pwmValue);
          }

          // Update the Temperature if present in JSON
          if (deviceData.containsKey("Temperature")) {
            int temperatureValue = deviceData["Temperature"];
            deviceList[i]->setTemperature(temperatureValue);
          }
        }
      }
    }
  }
}
