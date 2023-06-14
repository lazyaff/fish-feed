#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>

const char *ssid = "ASUS_X00TD";
const char *password = "113333555555";

ESP8266WebServer server(80);
Servo myservo;

int servoPin = D1; // Pin servo terhubung ke D1 (GPIO5) di NodeMCU

RTC_DS1307 rtc;

void moveServo(int pos)
{
  myservo.write(pos);
  delay(1000);      // Berikan waktu untuk servo mencapai posisi yang diinginkan
  myservo.detach(); // Lepaskan servo setelah mencapai posisi
}

void handleRoot()
{
  String html = "<html><body><h1>Servo Control</h1>";
  html += "<p>Use /servo?pos=xxx to set the servo position (0-180).</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleServo()
{
  String servoPos = server.arg("pos");
  int pos = servoPos.toInt();

  if (pos >= 0 && pos <= 180)
  {
    moveServo(pos);
    server.send(200, "text/plain", "Servo position set to: " + String(pos));
  }
  else
  {
    server.send(400, "text/plain", "Invalid servo position");
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/servo", handleServo);

  server.begin();

  myservo.attach(servoPin);
  myservo.write(90); // Posisi awal servo (90 derajat)

  Wire.begin(D2, D1); // SDA pin terhubung ke D2 (GPIO4), SCL pin terhubung ke D1 (GPIO5)
  rtc.begin();

  // Set waktu RTC secara manual jika diperlukan
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
  server.handleClient();

  DateTime now = rtc.now();
  int currentHour = now.hour();

  // Jadwalkan pergerakan servo pada pukul 10:00 dan 18:00
  if (currentHour == 10 && now.minute() == 0 && now.second() == 0)
  {
    moveServo(45); // Posisi servo pada sudut 45 derajat
  }
  else if (currentHour == 18 && now.minute() == 0 && now.second() == 0)
  {
    moveServo(135); // Posisi servo pada sudut 135 derajat
  }
}
