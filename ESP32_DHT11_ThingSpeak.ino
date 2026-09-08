#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// ---------------- DHT11 ----------------

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ---------------- Wi-Fi ----------------

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------------- ThingSpeak ----------------

unsigned long channelID = 123....x;

const char* writeAPIKey = "YOUR_WRITE_API_KEY";

WiFiClient client;

void setup() {

  Serial.begin(115200);

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {

  // Reconnect Wi-Fi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Wi-Fi disconnected. Reconnecting...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi Connected Again!");
  }

  // Read DHT11 data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check sensor reading
  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read DHT11!");
    delay(2000);

    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C");

  Serial.print("    Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int statusCode =
      ThingSpeak.writeFields(channelID, writeAPIKey);

  if (statusCode == 200) {

    Serial.println("Data uploaded to ThingSpeak successfully!");
  }

  else {

    Serial.print("ThingSpeak Error Code: ");
    Serial.println(statusCode);
  }

  Serial.println("------------------------------");

  // ThingSpeak update interval
  delay(20000);
}
