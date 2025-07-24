#ifndef FIREBASE_H
#define FIREBASE_H

#include "Arduino.h"

// Platform-specific WiFi library includes
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <WiFiClientSecure.h>
#elif defined(ARDUINO_UNOWIFIR4)
    #include <WiFiS3.h>
#else
    #error "Please connect a compatible board: ESP8266, ESP32, or Arduino UNO R4 WiFi."
#endif

#define PORT 443  // HTTPS port for Firebase

/**
 * Firebase Realtime Database library for Arduino
 * Supports ESP8266, ESP32, and Arduino UNO R4 WiFi
 */
class Firebase {
    public:
        /**
         * Constructor
         * @param referenceURL Firebase Realtime Database URL
         * @param authToken Optional authentication token (default: empty)
         */
        Firebase(String referenceURL, String authToken = "");

        // Set methods - store data at a specific path
        int setString(String path, String data);
        int setInt(String path, int data);
        int setFloat(String path, float data);
        int setBool(String path, bool data);
        int setJson(String path, String data);

        // Push methods - create unique keys and store data
        int pushString(String path, String data);
        int pushInt(String path, int data);
        int pushFloat(String path, float data);
        int pushBool(String path, bool data);
        int pushJson(String path, String data);

        // Get methods - retrieve data from a specific path (returns status code)
        int getString(String path, String& result);
        int getInt(String path, int& result);
        int getFloat(String path, float& result);
        int getBool(String path, bool& result);
        int getJson(String path, String& result);

        // Remove method - delete data from a specific path
        int remove(String path);

    private:
        String _host;           // Firebase host URL
        String _authToken;      // Authentication token

        // Internal methods
        void connect_to_host();
        int set(String path, String msg);
        int push(String path, String msg);
        int get(String path, String& result);

        // Platform-specific HTTPS client
        #if defined(ARDUINO_UNOWIFIR4)
            WiFiSSLClient _httpsClient;
        #else
            WiFiClientSecure _httpsClient;
        #endif
};

#endif
