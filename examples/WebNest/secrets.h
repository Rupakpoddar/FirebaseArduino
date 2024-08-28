/*

          WIFI SETUP

*/

#define WIFI_SSID     "YOUR-WIFI-SSID"
#define WIFI_PASSWORD "YOUR-WIFI-PASSWORD"

/*

          FIREBASE SETUP

  ------------------------------------------
  IMPORTANT: WebNest only supports Test Mode
  ------------------------------------------

  ** Test Mode (No Authentication) **:

    - Ensure Firebase rules are set to allow public access. Set the rules as follows:
      {
        "rules": {
          ".read": "true",
          ".write": "true"
        }
      }
*/

/* Firebase Project Reference URL */
#define REFERENCE_URL "https://your-database.firebaseio.com/"
