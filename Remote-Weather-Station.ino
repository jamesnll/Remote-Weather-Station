#include <WiFi.h>
#include "WiFi-Secrets.h"

void setup()
{
  Serial.begin(115200); // Start the Serial Monitor

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

void loop()
{

}
