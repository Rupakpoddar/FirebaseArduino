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

Firebase::Firebase(String referenceURL) {
	_host = referenceURL;

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
  String _data = "\"" + data + "\"";
  return this->set(path, _data);
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

int Firebase::set(String path, String msg) {
	connect_to_host();
  String jsonObject = String("/") + path + String(".json");

  _httpsClient.print(String("PUT ") + jsonObject + " HTTP/1.1\r\n" +
          "Host: " + _host + "\r\n" +
          "Connection: close\r\n" +
          "Accept: */*\r\n" +
          "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
          "Content-Type: application/json;charset=utf-8\r\n" +
          "Content-Length: " + msg.length() + "\r\n" +
          "\r\n" +
          msg + "\r\n");

  while (_httpsClient.connected()) {
    String line = _httpsClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String line;
  while(_httpsClient.available()) {
    line = _httpsClient.readStringUntil('\n');
    if (line.length() > 0)
      return 200; // Success
  }

  return 400;     // Failure
}

int Firebase::pushString(String path, String data) {
  String _data = "\"" + data + "\"";
  return this->push(path, _data);
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

int Firebase::push(String path, String msg) {
	connect_to_host();
  String jsonObject = String("/") + path + String(".json");

  _httpsClient.print(String("POST ") + jsonObject + " HTTP/1.1\r\n" +
          "Host: " + _host + "\r\n" +
          "Connection: close\r\n" +
          "Accept: */*\r\n" +
          "User-Agent: Mozilla/4.0 (compatible; Arduino Device; Windows NT 5.1)\r\n" +
          "Content-Type: application/json;charset=utf-8\r\n" +
          "Content-Length: " + msg.length() + "\r\n" +
          "\r\n" +
          msg + "\r\n");

  while (_httpsClient.connected()) {
    String line = _httpsClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String line;
  while(_httpsClient.available()) {
    line = _httpsClient.readStringUntil('\n');
    if (line.length() > 0)
      return 200; // Success
  }

  return 400;     // Failure
}

String Firebase::getString(String path) {
  return String(this->get(path));
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

String Firebase::get(String path) {
	connect_to_host();
  String jsonObject = String("/") + path + String(".json");

  _httpsClient.print(String("GET ") + jsonObject + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Skip HTTP headers
  bool headersEnded = false;
  while (_httpsClient.connected() || _httpsClient.available()) {
    String line = _httpsClient.readStringUntil('\n');
    if (line == "\r") { // End of headers
      headersEnded = true;
      break;
    }
  }

  // Read the body
  String body = "";
  while (_httpsClient.available()) {
    String line = _httpsClient.readStringUntil('\n');
    if (line.length() > 0) {
      body += line; // Accumulate response body
    }
  }

  // Remove any potential extra quotes
  if (body.startsWith("\"") && body.endsWith("\"")) {
    body = body.substring(1, body.length() - 1);
  }

  return body;
}

int Firebase::remove(String path) {
	connect_to_host();
  String jsonObject = String("/") + path + String(".json");

  _httpsClient.print(String("DELETE ") + jsonObject + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "Connection: close\r\n\r\n");

  while (_httpsClient.connected()) {
    String line = _httpsClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String line;
  while(_httpsClient.available()) {
    line = _httpsClient.readStringUntil('\n');
    if (line.length() > 0)
      return 200; // Success
  }

  return 400;     // Failure
}
