#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Secrets.h"
#include "DHT.h"

#define DHT_PIN 23
#define DHT_TYPE DHT11

using namespace std;

/**
 * Ideas to implement:
 *
 * Implement various locations to request weather information.
 *
 * Use DHT11 module to add local weather.
 *
 * Use LCD module to display the weather information.
 *
 * Display the weather information onto a webpage from the ESP32.
 *
 * Use potentiometer and buttons to allow user to select which location to request weather information.
 */

 DHT dht(DHT_PIN, DHT_TYPE);

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

/**
 * Function that makes an HTTP Request to OpenWeatherMap and retrieves the weather for White Rock, BC.
 * Outputs the description, temperature, and humidity to the Serial Monitor.
 */
void request_weather_info()
{
  // Check if WiFi is connected before doing anything
  if (WiFi.status() == WL_CONNECTED)
  {
    String url = "https://api.openweathermap.org/data/2.5/weather?";
    String lat = "49.026743";
    String lon = "-122.806303";
    HTTPClient http;
    int http_code;

    // Set the final URL with parameters for the HTTP Request
    http.begin(url + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + SECRET_WEATHER_API);

    // Begin the connection and send the HTTP Request
    http_code = http.GET();

    // http_code will receive an error if negative
    if (http_code > 0)
    {
      // Read the data as a JSON string
      String json_data = http.getString();

      // Retrieve some information from the JSON
      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, json_data);
      
      if (error) {
        Serial.print(F("Failed to parse JSON: "));
        Serial.println(error.f_str());
        return;
      }
      
      JsonObject obj = doc.as<JsonObject>();

      // Check if the JSON contains the expected data
      if (obj.containsKey("weather") && obj["weather"].size() > 0) {
        const char* description = obj["weather"][0]["description"].as<const char*>();
        const float temp = obj["main"]["temp"].as<float>();
        const float humidity = obj["main"]["humidity"].as<float>();

        Serial.println("--- Weather in White Rock ---");
        Serial.printf("Description: %s\nTemperature: %.2f°C\nHumidity: %.2f%%\n", description, temp, humidity);
      } else {
        Serial.println("Weather data not available.");
      }
    }
    else
    {
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(http_code).c_str());
    }

    http.end();
  }
}

void setup()
{
  Serial.begin(115200); // Start the Serial Monitor
  analogSetAttenuation(ADC_11db);
  connect_to_wifi();
  request_weather_info();

  dht.begin();
}

void loop()
{
  float dht_temp = dht.readTemperature();
  float dht_humidity = dht.readHumidity();

  // Check for errors
  if (isnan(dht_temp) || isnan(dht_humidity))
  {
    Serial.println("Failed to read from DHT sensor.");
    return;
  }

  // Compute heat index in Celcius
  float dht_heat_index = dht.computeHeatIndex(dht_temp, dht_humidity, false);

  Serial.println("--- Local Weather information ---");
  Serial.printf("Temperature: %.2f\n Humidity: %2.f\nHeat Index: %.2f\n", dht_temp, dht_humidity, dht_heat_index);

  delay(2000);
}
