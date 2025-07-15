/*
  ---------------------------------
            WIFI SETUP
  ---------------------------------
*/

#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

/*
  ---------------------------------
          FIREBASE SETUP
  ---------------------------------
  
  NOTE: WEBNEST ONLY SUPPORTS TEST MODE (NO AUTHENTICATION). 
  
  ** Test Mode (No Authentication) **:

    - Ensure Firebase rules are set to allow public access. Set the rules as follows:
    
      {
        "rules": {
          ".read": "true",
          ".write": "true"
        }
      }
    
    - In Webnest, devices are stored under the /webnest key
    - Each device uses its name as the key
    - Device properties: icon, state, sliderEnabled, sliderValue, order
*/

/* Firebase Project Reference URL */
#define REFERENCE_URL "https://your-database.firebaseio.com/"
