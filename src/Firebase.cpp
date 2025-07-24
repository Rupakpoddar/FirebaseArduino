#include "Firebase.h"

/**
 * Constructor for Firebase class
 * @param referenceURL The Firebase Realtime Database URL
 * @param authToken Optional authentication token for secured access
 */
Firebase::Firebase(String referenceURL, String authToken) {
    _host = referenceURL;
    _authToken = authToken;

    // Remove protocol prefix if present
    if (_host.startsWith("https://")) {
        _host.remove(0, 8);
    }

    // Remove trailing slash if present
    if (_host.endsWith("/")) {
        _host.remove(_host.length() - 1);
    }
    
    // Set insecure mode for non-UNO R4 boards
    #if !defined(ARDUINO_UNOWIFIR4)
        _httpsClient.setInsecure();
    #endif
}

/**
 * Establish connection to Firebase host
 * Retries up to 30 times with 100ms delays
 */
void Firebase::connect_to_host() {
    int retries = 0;
    while ((!_httpsClient.connect(_host.c_str(), PORT)) && (retries < 30)) {
        delay(100);
        retries++;
    }
}

/**
 * Set a string value at the specified path
 * @param path Firebase path to store the data
 * @param data String value to store
 * @return HTTP response code (200 = success)
 */
int Firebase::setString(String path, String data) {
    // Add quotes around the data string for JSON format
    data = "\"" + data + "\"";
    return this->set(path, data);
}

/**
 * Set an integer value at the specified path
 * @param path Firebase path to store the data
 * @param data Integer value to store
 * @return HTTP response code (200 = success)
 */
int Firebase::setInt(String path, int data) {
    return this->set(path, String(data));
}

/**
 * Set a float value at the specified path
 * @param path Firebase path to store the data
 * @param data Float value to store
 * @return HTTP response code (200 = success)
 */
int Firebase::setFloat(String path, float data) {
    return this->set(path, String(data));
}

/**
 * Set a boolean value at the specified path
 * @param path Firebase path to store the data
 * @param data Boolean value to store
 * @return HTTP response code (200 = success)
 */
int Firebase::setBool(String path, bool data) {
    return this->set(path, data ? "true" : "false");
}

/**
 * Set a JSON object at the specified path
 * @param path Firebase path to store the data
 * @param data JSON string to store
 * @return HTTP response code (200 = success)
 */
int Firebase::setJson(String path, String data) {
    // Remove quotes if present (JSON objects shouldn't be quoted)
    if (data.startsWith("\"") && data.endsWith("\"")) {
        data = data.substring(1, data.length() - 1);
    }
    return this->set(path, data);
}

/**
 * Internal method to set data using HTTP PUT request
 * @param path Firebase path
 * @param msg Data to send
 * @return HTTP response code
 */
int Firebase::set(String path, String msg) {
    connect_to_host();

    // Build the JSON endpoint URL
    String jsonObject = "";
    if (_authToken != "") {
        jsonObject = String("/") + path + String(".json?auth=") + _authToken;
    } else {
        jsonObject = String("/") + path + String(".json");
    }

    // Send HTTP PUT request
    _httpsClient.print(String("PUT ") + jsonObject + " HTTP/1.1\r\n" +
                      "Host: " + _host + "\r\n" +
                      "Connection: close\r\n" +
                      "Accept: */*\r\n" +
                      "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
                      "Content-Type: application/json;charset=utf-8\r\n" +
                      "Content-Length: " + msg.length() + "\r\n" +
                      "\r\n" +
                      msg + "\r\n");

    // Parse response to extract status code
    String responseLine;
    int responseCode = 0;
    bool headersEnded = false;

    while (_httpsClient.connected() || _httpsClient.available()) {
        if (_httpsClient.available()) {
            responseLine = _httpsClient.readStringUntil('\n');
            responseLine.trim();

            if (!headersEnded) {
                // Extract status code from HTTP response line
                if (responseLine.startsWith("HTTP/")) {
                    int firstSpace = responseLine.indexOf(' ');
                    int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

                    if (firstSpace > 0 && secondSpace > 0) {
                        responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
                    }
                }
                
                // Check for end of headers
                if (responseLine.length() == 0) {
                    headersEnded = true;
                }
            }
        }
    }

    return responseCode;
}

/**
 * Push a string value to the specified path (creates unique key)
 * @param path Firebase path to push the data
 * @param data String value to push
 * @return HTTP response code (200 = success)
 */
int Firebase::pushString(String path, String data) {
    // Add quotes around the data string for JSON format
    data = "\"" + data + "\"";
    return this->push(path, data);
}

/**
 * Push an integer value to the specified path (creates unique key)
 * @param path Firebase path to push the data
 * @param data Integer value to push
 * @return HTTP response code (200 = success)
 */
int Firebase::pushInt(String path, int data) {
    return this->push(path, String(data));
}

/**
 * Push a float value to the specified path (creates unique key)
 * @param path Firebase path to push the data
 * @param data Float value to push
 * @return HTTP response code (200 = success)
 */
int Firebase::pushFloat(String path, float data) {
    return this->push(path, String(data));
}

/**
 * Push a boolean value to the specified path (creates unique key)
 * @param path Firebase path to push the data
 * @param data Boolean value to push
 * @return HTTP response code (200 = success)
 */
int Firebase::pushBool(String path, bool data) {
    return this->push(path, data ? "true" : "false");
}

/**
 * Push a JSON object to the specified path (creates unique key)
 * @param path Firebase path to push the data
 * @param data JSON string to push
 * @return HTTP response code (200 = success)
 */
int Firebase::pushJson(String path, String data) {
    // Remove quotes if present (JSON objects shouldn't be quoted)
    if (data.startsWith("\"") && data.endsWith("\"")) {
        data = data.substring(1, data.length() - 1);
    }
    return this->push(path, data);
}

/**
 * Internal method to push data using HTTP POST request
 * @param path Firebase path
 * @param msg Data to send
 * @return HTTP response code
 */
int Firebase::push(String path, String msg) {
    connect_to_host();

    // Build the JSON endpoint URL
    String jsonObject = "";
    if (_authToken != "") {
        jsonObject = String("/") + path + String(".json?auth=") + _authToken;
    } else {
        jsonObject = String("/") + path + String(".json");
    }

    // Send HTTP POST request
    _httpsClient.print(String("POST ") + jsonObject + " HTTP/1.1\r\n" +
                      "Host: " + _host + "\r\n" +
                      "Connection: close\r\n" +
                      "Accept: */*\r\n" +
                      "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
                      "Content-Type: application/json;charset=utf-8\r\n" +
                      "Content-Length: " + msg.length() + "\r\n" +
                      "\r\n" +
                      msg + "\r\n");

    // Parse response to extract status code
    String responseLine;
    int responseCode = 0;
    bool headersEnded = false;

    while (_httpsClient.connected() || _httpsClient.available()) {
        if (_httpsClient.available()) {
            responseLine = _httpsClient.readStringUntil('\n');
            responseLine.trim();

            if (!headersEnded) {
                // Extract status code from HTTP response line
                if (responseLine.startsWith("HTTP/")) {
                    int firstSpace = responseLine.indexOf(' ');
                    int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

                    if (firstSpace > 0 && secondSpace > 0) {
                        responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
                    }
                }
                
                // Check for end of headers
                if (responseLine.length() == 0) {
                    headersEnded = true;
                }
            }
        }
    }

    return responseCode;
}

/**
 * Get a string value from the specified path
 * @param path Firebase path to retrieve data from
 * @param result Reference to store the retrieved string value
 * @return HTTP status code (200 = success)
 */
int Firebase::getString(String path, String& result) {
    return this->get(path, result);
}

/**
 * Get an integer value from the specified path
 * @param path Firebase path to retrieve data from
 * @param result Reference to store the retrieved integer value
 * @return HTTP status code (200 = success)
 */
int Firebase::getInt(String path, int& result) {
    String data;
    int statusCode = this->get(path, data);
    
    if (statusCode == 200) {
        result = data.toInt();
    } else {
        result = 0;
    }
    
    return statusCode;
}

/**
 * Get a float value from the specified path
 * @param path Firebase path to retrieve data from
 * @param result Reference to store the retrieved float value
 * @return HTTP status code (200 = success)
 */
int Firebase::getFloat(String path, float& result) {
    String data;
    int statusCode = this->get(path, data);
    
    if (statusCode == 200) {
        result = data.toFloat();
    } else {
        result = 0.0;
    }
    
    return statusCode;
}

/**
 * Get a boolean value from the specified path
 * @param path Firebase path to retrieve data from
 * @param result Reference to store the retrieved boolean value
 * @return HTTP status code (200 = success)
 */
int Firebase::getBool(String path, bool& result) {
    String data;
    int statusCode = this->get(path, data);
    
    if (statusCode == 200) {
        result = (data == "true");
    } else {
        result = false;
    }
    
    return statusCode;
}

/**
 * Get a JSON object from the specified path
 * @param path Firebase path to retrieve data from
 * @param result Reference to store the retrieved JSON string
 * @return HTTP status code (200 = success)
 */
int Firebase::getJson(String path, String& result) {
    // getJson is an alias for getString - they work identically
    return this->getString(path, result);
}

/**
 * Internal method to get data using HTTP GET request
 * @param path Firebase path
 * @param result Reference to store the retrieved data
 * @return HTTP status code
 */
int Firebase::get(String path, String& result) {
    connect_to_host();

    // Build the JSON endpoint URL
    String jsonObject = "";
    if (_authToken != "") {
        jsonObject = String("/") + path + String(".json?auth=") + _authToken;
    } else {
        jsonObject = String("/") + path + String(".json");
    }

    // Send HTTP GET request
    _httpsClient.print(String("GET ") + jsonObject + " HTTP/1.1\r\n" +
                      "Host: " + _host + "\r\n" +
                      "Connection: close\r\n\r\n");

    // Parse response
    String responseLine;
    bool headersEnded = false;
    String body = "";
    int statusCode = 0;

    while (_httpsClient.connected() || _httpsClient.available()) {
        if (_httpsClient.available()) {
            responseLine = _httpsClient.readStringUntil('\n');
            responseLine.trim();

            if (!headersEnded) {
                if (responseLine.length() == 0) {
                    // Empty line indicates end of headers
                    headersEnded = true;
                } else if (responseLine.startsWith("HTTP/")) {
                    // Extract status code
                    int firstSpace = responseLine.indexOf(' ');
                    int secondSpace = responseLine.indexOf(' ', firstSpace + 1);
                    if (firstSpace > 0 && secondSpace > 0) {
                        statusCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
                    }
                }
            } else {
                // Append body content
                body += responseLine;
            }
        }
    }

    // Set result based on status code
    if (statusCode == 200) {
        // Remove quotes from simple values
        if (body.startsWith("\"") && body.endsWith("\"")) {
            result = body.substring(1, body.length() - 1);
        } else {
            result = body;
        }
    } else {
        result = "NULL";
    }

    return statusCode;
}

/**
 * Remove data from the specified path
 * @param path Firebase path to remove data from
 * @return HTTP response code (200 = success)
 */
int Firebase::remove(String path) {
    connect_to_host();

    // Build the JSON endpoint URL
    String jsonObject = "";
    if (_authToken != "") {
        jsonObject = String("/") + path + String(".json?auth=") + _authToken;
    } else {
        jsonObject = String("/") + path + String(".json");
    }

    // Send HTTP DELETE request
    _httpsClient.print(String("DELETE ") + jsonObject + " HTTP/1.1\r\n" +
                      "Host: " + _host + "\r\n" +
                      "Connection: close\r\n\r\n");

    // Parse response to extract status code
    String responseLine;
    int responseCode = 0;
    bool headersEnded = false;

    while (_httpsClient.connected() || _httpsClient.available()) {
        if (_httpsClient.available()) {
            responseLine = _httpsClient.readStringUntil('\n');
            responseLine.trim();

            if (!headersEnded) {
                // Extract status code from HTTP response line
                if (responseLine.startsWith("HTTP/")) {
                    int firstSpace = responseLine.indexOf(' ');
                    int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

                    if (firstSpace > 0 && secondSpace > 0) {
                        responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
                    }
                }
                
                // Check for end of headers
                if (responseLine.length() == 0) {
                    headersEnded = true;
                }
            }
        }
    }

    return responseCode;
}
