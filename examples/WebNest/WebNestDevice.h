#ifndef WEBNEST_DEVICE_H
#define WEBNEST_DEVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define MAX_DEVICES 16

class WebnestDevice {
  public:
    WebnestDevice(String name);  // Constructor
    ~WebnestDevice();            // Destructor

    // Public Getters
    String getName();            
    String getIcon();
    bool getState();
    bool getSliderEnabled();
    int getSliderValue();
    int getOrder();

    // Static poll method to update devices using a JSON string
    static void poll(const String& jsonString);

  private:
    String name;
    String icon;
    bool state;
    bool sliderEnabled;
    int sliderValue;
    int order;

    // Private Setters (only accessible within the class)
    void setName(String name);
    void setIcon(String icon);
    void setState(bool state);
    void setSliderEnabled(bool enabled);
    void setSliderValue(int value);
    void setOrder(int order);

    static WebnestDevice* deviceList[MAX_DEVICES];  // Array of pointers to WebnestDevice
};

// Static member definition
WebnestDevice* WebnestDevice::deviceList[MAX_DEVICES] = {NULL};  // Initialize all to NULL

// Constructor: Initializes the device and adds it to the list
WebnestDevice::WebnestDevice(String name) {
  this->setName(name);
  this->setIcon("icon1.png");
  this->setState(false);
  this->setSliderEnabled(false);
  this->setSliderValue(100);
  this->setOrder(0);

  // Add the device to the list (find the first empty spot)
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (deviceList[i] == NULL) {
      deviceList[i] = this;
      break;
    }
  }
}

// Destructor: Removes the device from the list
WebnestDevice::~WebnestDevice() {
  // Remove the device from the list
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (deviceList[i] == this) {
      deviceList[i] = NULL;
      break;
    }
  }
}

// Private Setters
void WebnestDevice::setName(String name) {
  this->name = name;
}

void WebnestDevice::setIcon(String icon) {
  this->icon = icon;
}

void WebnestDevice::setState(bool state) {
  this->state = state;
}

void WebnestDevice::setSliderEnabled(bool enabled) {
  this->sliderEnabled = enabled;
}

void WebnestDevice::setSliderValue(int value) {
  this->sliderValue = constrain(value, 0, 100);
}

void WebnestDevice::setOrder(int order) {
  this->order = order;
}

// Public Getters
String WebnestDevice::getName() {
  return name;
}

String WebnestDevice::getIcon() {
  return icon;
}

bool WebnestDevice::getState() {
  return state;
}

bool WebnestDevice::getSliderEnabled() {
  return sliderEnabled;
}

int WebnestDevice::getSliderValue() {
  return sliderValue;
}

int WebnestDevice::getOrder() {
  return order;
}

// Static poll method: Parses the JSON string and updates device states
void WebnestDevice::poll(const String& jsonString) {
  if (jsonString == "NULL" || jsonString == "") {
    Serial.println("No data found in Firebase under /webnest");
    return;
  }

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

        // Update the Icon if present in JSON
        if (deviceData.containsKey("icon")) {
          String iconValue = deviceData["icon"];
          deviceList[i]->setIcon(iconValue);
        }

        // Update the State if present in JSON
        if (deviceData.containsKey("state")) {
          bool stateValue = deviceData["state"];
          deviceList[i]->setState(stateValue);
        }

        // Update the Slider Enabled if present in JSON
        if (deviceData.containsKey("sliderEnabled")) {
          bool sliderEnabledValue = deviceData["sliderEnabled"];
          deviceList[i]->setSliderEnabled(sliderEnabledValue);
        }

        // Update the Slider Value if present in JSON
        if (deviceData.containsKey("sliderValue")) {
          int sliderVal = deviceData["sliderValue"];
          deviceList[i]->setSliderValue(sliderVal);
        }

        // Update the Order if present in JSON
        if (deviceData.containsKey("order")) {
          int orderValue = deviceData["order"];
          deviceList[i]->setOrder(orderValue);
        }
      }
    }
  }
}

#endif // WEBNEST_DEVICE_H
