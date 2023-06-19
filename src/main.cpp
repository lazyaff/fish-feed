#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>

const char *ssid = "KOSTBAGAS";
const char *password = "bagassdkangen";
// const char *ssid = "ASUS_X00TD";
// const char *password = "113333555555";

ESP8266WebServer server(80);
Servo myservo;

int servoPin = D3; // Pin servo terhubung ke D1 (GPIO5) di NodeMCU

RTC_DS1307 rtc;

String html;

void handleRoot()
{
  server.send(200, "text/html", html);
}

void handleServo()
{
  myservo.write(45);
  delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
  myservo.write(90);
  server.send(200, "text/html", html);
}

void setup()
{
  Serial.begin(9600);

  myservo.attach(servoPin);

  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.print("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  html += "<html><head><title>Fish Feeder</title><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'></head>";
  html += "<body class='text-center'><h1>Fish Feeder</h1>";
  html += "<p>Tekan tombol dibawah untuk memberi makan ikan.</p>";
  html += "<p><a href=\"servo\"\"><button class='btn btn-primary'>Mulai</button></a></p>";
  html += "</body></html>";

  server.on("/", handleRoot);
  server.on("/servo", handleServo);

  server.begin();

  Wire.begin(D2, D1); // SDA pin terhubung ke D2 (GPIO4), SCL pin terhubung ke D1 (GPIO5)
  rtc.begin();

  // Set waktu RTC secara manual jika diperlukan
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
  server.handleClient();

  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  Serial.println();
  Serial.print(hour);
  Serial.print(".");
  Serial.print(minute);
  Serial.print(".");
  Serial.print(second);

  // Jadwalkan pergerakan servo pada pukul 10:00 dan 18:00
  if (hour == 6 && minute == 0 && second == 0)
  {
    myservo.write(45);
    delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
    myservo.write(90);
  }
  else if (hour == 16 && minute == 0 && second == 0)
  {
    myservo.write(45);
    delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
    myservo.write(90);
  }
  delay(1000);
}
