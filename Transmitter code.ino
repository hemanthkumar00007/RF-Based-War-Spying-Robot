#include <DHT.h>
#include <TinyGPSPlus.h>

#define DHTPIN 6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int data;

int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;

int gasSensor = A0;
int metalSensor = 24;

int trigPin = 7;
int echoPin = 8;

long duration;
int distance;

TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(gasSensor, INPUT);
  pinMode(metalSensor, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  dht.begin();
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 20 && distance > 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    // Serial.println("ALERT:OBSTACLE");
    Serial1.println(" ALERT:OBSTACLE ");
    delay(500);
  }

  if (digitalRead(metalSensor) == LOW) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    Serial1.println(" METAL DETECTED ");
    sendSMS();
    delay(1000);
  }

  if (Serial1.available() > 0) {
    data = Serial1.read();

    if (data == 1) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }

    else if (data == 2) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }

    else if (data == 3) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }

    else if (data == 4) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }

    else if (data == 5) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }

    else if (data == 6) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      float temperature = dht.readTemperature();
      int gasValue = analogRead(gasSensor);

      Serial1.print("T:");
      Serial1.print(temperature);
      Serial1.print("C ");

      Serial1.print("G:");
      Serial1.println(gasValue);

      sendSMS();
    }
  }
}

void sendSMS() {

  double lat = 0, lon = 0;
  unsigned long start = millis();
  while (millis() - start < 10000) {
    while (Serial2.available()) {
      if (gps.encode(Serial2.read())) {
        if (gps.location.isValid()) {
          lat = gps.location.lat();
          lon = gps.location.lng();
        }
      }
    }
  }

  String message;
  if (lat != 0 && lon != 0) {
    message = "Location Updated! Location: https://maps.google.com/?q=";
    message += String(lat, 6) + "," + String(lon, 6);
  } else {
    message = "Location Updated! GPS not fixed yet.";
  }
  Serial2.println("AT");
  delay(500);
  Serial2.println("AT+CMGF=1");
  delay(500);
  Serial2.print("AT+CMGS=\"+919390659644\"\r");
  delay(500);
  Serial2.print(message);
  Serial2.write(26);

  delay(5000);
}
