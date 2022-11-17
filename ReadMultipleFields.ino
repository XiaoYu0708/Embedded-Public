#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = SECRET_CH_ID_WEATHER_STATION;

int statusCode = 0;
int field[2] = {1, 2};

void setup() {
  Serial.begin(115200);      // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }
}

void loop() {

  // Read and store all the latest field values, location coordinates, status message, and created-at timestamp
  // use ThingSpeak.readMultipleFields(channelNumber, readAPIKey) for private channels
  statusCode = ThingSpeak.readMultipleFields(weatherStationChannelNumber,ReadAPIKey);

  if (statusCode == 200)
  {
    // Fetch the stored data

    int T = ThingSpeak.getFieldAsInt(field[0]); // Field 1
    int H = ThingSpeak.getFieldAsFloat(field[1]); // Field 2

    String statusMessage = ThingSpeak.getStatus(); // Status message
    String latitude = ThingSpeak.getLatitude(); // Latitude
    String longitude = ThingSpeak.getLongitude(); // Longitude
    String elevation = ThingSpeak.getElevation(); // Elevation
    String createdAt = ThingSpeak.getCreatedAt(); // Created-at timestamp

    Serial.println("溫度: " + String(T));
    Serial.println("濕度: " + String(H));
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  Serial.println();
  delay(5000); // no need to fetch too often

}
