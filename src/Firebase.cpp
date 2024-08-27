/*

	MIT License

	Copyright (c) 2024 Rupak Poddar

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

#include "Firebase.h"

Firebase::Firebase(String referenceURL, String authToken) {
	_host = referenceURL;
  _authToken = authToken;

  if (_host.startsWith("https://")) {
    _host.remove(0, 8);
  }

  if (_host.endsWith("/")) {
    _host.remove(_host.length() - 1);
  }
  
  #if !defined(ARDUINO_UNOWIFIR4)
      _httpsClient.setInsecure();
  #endif
}

void Firebase::connect_to_host() {
	int r = 0;
  while((!_httpsClient.connect(_host.c_str(), PORT)) && (r < 30)) {
      delay(100);
      r++;
  }
}

int Firebase::setString(String path, String data) {
  // Add quotes around the data string
  data = "\"" + data + "\"";
  return this->set(path, data);
}

int Firebase::setInt(String path, int data) {
  return this->set(path, String(data));
}

int Firebase::setFloat(String path, float data) {
  return this->set(path, String(data));
}

int Firebase::setBool(String path, bool data) {
  return this->set(path, data ? "true" : "false");
}

int Firebase::setJson(String path, String data) {
  // Check if the data string has leading and trailing quotes
  if (data.startsWith("\"") && data.endsWith("\"")) {
    // Remove the leading and trailing quotes from the data string
    data = data.substring(1, data.length() - 1);
  }
  return this->set(path, data);
}

int Firebase::set(String path, String msg) {
	connect_to_host();

  String jsonObject = "";
  if (_authToken != "") {
    jsonObject = String("/") + path + String(".json?auth=") + _authToken;
  } else {
    jsonObject = String("/") + path + String(".json");
  }

  _httpsClient.print(String("PUT ") + jsonObject + " HTTP/1.1\r\n" +
          "Host: " + _host + "\r\n" +
          "Connection: close\r\n" +
          "Accept: */*\r\n" +
          "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
          "Content-Type: application/json;charset=utf-8\r\n" +
          "Content-Length: " + msg.length() + "\r\n" +
          "\r\n" +
          msg + "\r\n");

  String responseLine;
  int responseCode = 0;
  bool headersEnded = false;

  while (_httpsClient.connected() || _httpsClient.available()) {
    if (_httpsClient.available()) {
      responseLine = _httpsClient.readStringUntil('\n');
      responseLine.trim();  // Remove any leading or trailing whitespace

      if (!headersEnded) {
        // Check for the status line
        if (responseLine.startsWith("HTTP/")) {
          int firstSpace = responseLine.indexOf(' ');
          int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

          if (firstSpace > 0 && secondSpace > 0) {
            responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
          }
        }
        
        // End of headers
        if (responseLine.length() == 0) {
          headersEnded = true;
        }
      }
    }
  }

  // Return the response code
  return responseCode;
}

int Firebase::pushString(String path, String data) {
  // Add quotes around the data string
  data = "\"" + data + "\"";
  return this->push(path, data);
}

int Firebase::pushInt(String path, int data) {
  return this->push(path, String(data));
}

int Firebase::pushFloat(String path, float data) {
  return this->push(path, String(data));
}

int Firebase::pushBool(String path, bool data) {
  return this->push(path, data ? "true" : "false");
}

int Firebase::pushJson(String path, String data) {
  // Check if the data string has leading and trailing quotes
  if (data.startsWith("\"") && data.endsWith("\"")) {
    // Remove the leading and trailing quotes from the data string
    data = data.substring(1, data.length() - 1);
  }
  return this->push(path, data);
}

int Firebase::push(String path, String msg) {
	connect_to_host();

  String jsonObject = "";
  if (_authToken != "") {
    jsonObject = String("/") + path + String(".json?auth=") + _authToken;
  } else {
    jsonObject = String("/") + path + String(".json");
  }

  _httpsClient.print(String("POST ") + jsonObject + " HTTP/1.1\r\n" +
          "Host: " + _host + "\r\n" +
          "Connection: close\r\n" +
          "Accept: */*\r\n" +
          "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
          "Content-Type: application/json;charset=utf-8\r\n" +
          "Content-Length: " + msg.length() + "\r\n" +
          "\r\n" +
          msg + "\r\n");

  String responseLine;
  int responseCode = 0;
  bool headersEnded = false;

  while (_httpsClient.connected() || _httpsClient.available()) {
    if (_httpsClient.available()) {
      responseLine = _httpsClient.readStringUntil('\n');
      responseLine.trim();  // Remove any leading or trailing whitespace

      if (!headersEnded) {
        // Check for the status line
        if (responseLine.startsWith("HTTP/")) {
          int firstSpace = responseLine.indexOf(' ');
          int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

          if (firstSpace > 0 && secondSpace > 0) {
            responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
          }
        }
        
        // End of headers
        if (responseLine.length() == 0) {
          headersEnded = true;
        }
      }
    }
  }

  // Return the response code
  return responseCode;
}

String Firebase::getString(String path) {
  return this->get(path);
}

int Firebase::getInt(String path) {
  return this->get(path).toInt();
}

float Firebase::getFloat(String path) {
  return this->get(path).toFloat();
}

bool Firebase::getBool(String path) {
  return this->get(path) == "true";
}

String Firebase::getJson(String path) {
  String response = this->get(path);
  // Check if the response string has leading and trailing quotes
  if (response.startsWith("\"") && response.endsWith("\"")) {
    // Remove the leading and trailing quotes from the response string
    response = response.substring(1, response.length() - 1);
  }
  return response;
}

String Firebase::get(String path) {
	connect_to_host();

  String jsonObject = "";
  if (_authToken != "") {
    jsonObject = String("/") + path + String(".json?auth=") + _authToken;
  } else {
    jsonObject = String("/") + path + String(".json");
  }

  _httpsClient.print(String("GET ") + jsonObject + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "Connection: close\r\n\r\n");

  String responseLine;
  bool headersEnded = false;
  String body = "";
  int statusCode = 0;

  while (_httpsClient.connected() || _httpsClient.available()) {
    if (_httpsClient.available()) {
      responseLine = _httpsClient.readStringUntil('\n');
      responseLine.trim();  // Remove leading/trailing whitespace

      if (!headersEnded) {
        if (responseLine.length() == 0) {
          // Empty line indicates end of headers
          headersEnded = true;
        } else if (responseLine.startsWith("HTTP/")) {
          // Extract and optionally log status code if needed
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

  if (statusCode != 200) {
    return "NULL";
  }

  // Remove any potential extra quotes from the body
  if (body.startsWith("\"") && body.endsWith("\"")) {
    body = body.substring(1, body.length() - 1);
  }

  return body;
}

int Firebase::remove(String path) {
	connect_to_host();

  String jsonObject = "";
  if (_authToken != "") {
    jsonObject = String("/") + path + String(".json?auth=") + _authToken;
  } else {
    jsonObject = String("/") + path + String(".json");
  }

  _httpsClient.print(String("DELETE ") + jsonObject + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "Connection: close\r\n\r\n");

  String responseLine;
  int responseCode = 0;
  bool headersEnded = false;

  while (_httpsClient.connected() || _httpsClient.available()) {
    if (_httpsClient.available()) {
      responseLine = _httpsClient.readStringUntil('\n');
      responseLine.trim();  // Remove any leading or trailing whitespace

      if (!headersEnded) {
        // Check for the status line
        if (responseLine.startsWith("HTTP/")) {
          int firstSpace = responseLine.indexOf(' ');
          int secondSpace = responseLine.indexOf(' ', firstSpace + 1);

          if (firstSpace > 0 && secondSpace > 0) {
            responseCode = responseLine.substring(firstSpace + 1, secondSpace).toInt();
          }
        }
        
        // End of headers
        if (responseLine.length() == 0) {
          headersEnded = true;
        }
      }
    }
  }

  // Return the response code
  return responseCode;
}
