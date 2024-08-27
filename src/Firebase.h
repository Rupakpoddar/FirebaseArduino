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

#ifndef Firebase_h
	#define Firebase_h
	#include "Arduino.h"

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

	#define PORT 443

	class Firebase
	{
		public:
			Firebase(String referenceURL, String authToken = "");
			int setString(String path, String data);
			int setInt(String path, int data);
			int setFloat(String path, float data);
			int setBool(String path, bool data);
			int setJson(String path, String data);
			int pushString(String path, String data);
			int pushInt(String path, int data);
			int pushFloat(String path, float data);
			int pushBool(String path, bool data);
			int pushJson(String path, String data);
			String getString(String path);
			int getInt(String path);
			float getFloat(String path);
			bool getBool(String path);
			String getJson(String path);
			int remove(String path);

		private:
			String _host, _authToken;
			void connect_to_host();

			int set(String path, String msg);
			int push(String path, String msg);
			String get(String path);

			#if defined(ARDUINO_UNOWIFIR4)
				WiFiSSLClient _httpsClient;
			#else
				WiFiClientSecure _httpsClient;
			#endif
	};
#endif
