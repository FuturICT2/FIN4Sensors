int sensorPin = A0;
int active = 0;
int val = 0;

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  val = analogRead(sensorPin);

   if(val >= 300) {
    active = 1;
    Serial.println("------------ HIGH GAS QUANTITIES ------------");
  }

  if (active && val <= 120) {
    // Logging
    Serial.println("************** AIR QUALITY IMPROVED ***************");
    active = 0;
  }

  digitalWrite(LED_BUILTIN, not active);

  Serial.print("Sensor value: ");
  Serial.println(val);
  delay(400);
}
