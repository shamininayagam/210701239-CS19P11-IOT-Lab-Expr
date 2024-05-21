#define BLYNK_TEMPLATE_ID "TMPL3JoxujNOo"
#define BLYNK_TEMPLATE_NAME "WEIGHTSYSTEM"
#define BLYNK_AUTH_TOKEN "aC0fyuJ08vBDgm2GSwpujykXpRoVzuPz"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>
#define trigPin 33
#define echoPin 32

char auth[] = "aC0fyuJ08vBDgm2GSwpujykXpRoVzuPz"; // Blynk auth token
char ssid[] = "Galqxy S21 FE 5G b537"; // Your Wi-Fi network SSID
char pass[] = "12345678"; // Your Wi-Fi network password

long duration;
int distance;
bool systemState = false; // Variable to store the system state (on/off)

Servo myservo;

int calculateDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Blynk.virtualWrite(V2, distance);
  
  // Check distance and send alert if less than 20 cm
  if (distance < 20)
  {
    Blynk.logEvent("alert_now", "Object is less than 20 cm away!");
  }

  return distance;
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  myservo.attach(5);
  Serial.begin(9600);
}

BLYNK_WRITE(V4) // Function to handle the button press
{
  int pinValue = param.asInt();
  systemState = (pinValue == 1); // Set systemState based on button press

  // Log event based on system state
  if (systemState)
  {
    Blynk.logEvent("alert_now", "System has been turned ON");
  }
  else
  {
    Blynk.logEvent("alert_now", "System has been turned OFF");
  }
}

void loop()
{
  Blynk.run();

  if (systemState) // Only run if system is turned on
  {
    int i;
    for (i = 15; i <= 165; i++)
    {
      if (!systemState) break; // Check the system state
      myservo.write(i);
      delay(15);
      calculateDistance();
      Serial.print(i);
      Serial.print(",");
      Serial.print(distance);
      Serial.print(".");
      Blynk.virtualWrite(V1, i);
    }
    for (i = 165; i >= 15; i--)
    {
      if (!systemState) break; // Check the system state
      myservo.write(i);
      delay(15);
      calculateDistance();
      Serial.print(i);
      Serial.print(",");
      Serial.print(distance);
      Serial.print(".");
      Blynk.virtualWrite(V1, i);
    }
  }
}
