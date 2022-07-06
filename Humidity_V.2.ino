#include <Servo.h>
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define LED_ALARM_PIN 3
#define BUZZER_PIN    5

#define HUMIDITY_THRESHOLD 7
#define AIR_HUMIDITY   2
#define MUXIMUM_ANGLE  180
#define MINIMUM_ANGLE  0

#define SERVO_PIN 9
#define STEP_ANGLE 10

#define SCAN_SPEED_SEC 2000

#define START_PASSWORD 'S'
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
Servo Pointer;

static bool Mode = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Humidity Sensor!..."));
  Serial.println(F("Enter The Start Password..."));
  while (Mode == false) {
    while (Serial.available()==0);
    char str = Serial.read();
    Serial.println("Received,"+str);
    if ( str == START_PASSWORD) {
      Serial.println(F("Welcome..."));
      Mode = true;
      pinMode(LED_ALARM_PIN, OUTPUT);
      pinMode(BUZZER_PIN, OUTPUT);
      pinMode(SERVO_PIN, OUTPUT);

      Pointer.attach(SERVO_PIN);
      dht.begin();
    }
    else {
      Serial.println(F("Invalid..."));
    }
  }
}

void loop() {
  static uint8_t currentAngle = 90;

  // Wait a few seconds between measurements.
  delay(SCAN_SPEED_SEC);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();

  // Check if read failed and exit early (to try again).
  Serial.print(F("Humidity: "));
  Serial.println(humidity - AIR_HUMIDITY);

  if (humidity - AIR_HUMIDITY >= HUMIDITY_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_ALARM_PIN, HIGH);

    /*    For Validation  */
    if ( (currentAngle <= STEP_ANGLE ) ) {
      currentAngle = MINIMUM_ANGLE;
    }
    else {
      currentAngle -= STEP_ANGLE;
    }
  }

  else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_ALARM_PIN, LOW);

    /*    For Validation  */
    if (currentAngle + STEP_ANGLE > MUXIMUM_ANGLE) {
      currentAngle = MUXIMUM_ANGLE;
    }
    else {
      currentAngle += STEP_ANGLE;
    }
  }
  Serial.println(currentAngle);
  Pointer.write (currentAngle);
}
