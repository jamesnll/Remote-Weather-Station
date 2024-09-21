#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"

/**
 * Function to connect the ESP32 to the WiFi network.
 */
void connect_to_wifi()
{
  // Start the connection process
  Serial.printf("Connecting to %s\n", SECRET_SSID);

  // Initiate the connection
  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);

  // Wait until the connection is established
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println(".");
  Serial.println("WiFi connected!");

  // Print the ESP32's IP address
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200); // Start the Serial Monitor
  
  connect_to_wifi();
}

void loop()
{

}
