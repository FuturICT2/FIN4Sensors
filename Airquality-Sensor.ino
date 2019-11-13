#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Network Parameters
char ssid[] = ""; //SSID of your Wi-Fi router
char pass[] = ""; //Password of your Wi-Fi router
char accessToken[] = "";

char pingAddress[] = ""; // sensor ping end point
char verificationAddress[] = ""; // Claim verification end point
int verificationSent = 0;

int sensorPin = A0;

int val = 0;
int ppm = 0;
int active = 0;

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, 1);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  val = analogRead(sensorPin);

   if(!active && val >= 450) {
    active = 1;
    Serial.println("------------ ACTIVE ------------");
  }

  if (active && val <= 160) {

    if (!verificationSent) {
      // Send Verification
      verifyClaim();
      verificationSent = 1;
    }

    // Logging
    Serial.println("************** AIR QUALITY IMPROVED ***************");
    Serial.println(val);

    // Turn on Led
    digitalWrite(LED_BUILTIN, 0);
  }

  ping();

  Serial.print("Sensor value: ");
  Serial.println(val);
  delay(400);
}


// Ping Server
void ping() {
  
  // Declare object of class HTTPClient
  HTTPClient http;

  // Specify Ping destination
  http.begin(pingAddress);

  // Specify Content type and authenticate as Oracle
  http.addHeader("Content-Type", "application/json");

  // Send Request and receive http Code
  int httpCode = http.POST(String("{\"accessToken\": \"") + accessToken + String("\"}"));

  // Logging
  if (httpCode == 200) {
    //Serial.println("\n Ping! ------ \n");
  } else {
    Serial.println("\n server not available \n");
  }
  
  // Close connection
  http.end();
}


// Send Verification of Claim to Server 
void verifyClaim() {

  // Declare object of class HTTPClient
  HTTPClient http;

  // Specify request destination
  http.begin(verificationAddress);

  // Specify content-type header and "log in" as Oracle
  http.addHeader("Content-Type", "application/json");

  // Actually send the request
  int httpCode = http.POST(String("{\"isAccepted\": true, \"accessToken\": \"") + accessToken + String("\"}"));

  // Get the response payload
  String payload = http.getString();

  // Logging
  Serial.print("httpCode = ");
  Serial.println(httpCode);
  Serial.print("payload is = ");
  Serial.println(payload);

  // Close connection
  http.end();
}
