#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>
#include <iostream>
using namespace std;

const char *ssid = "KOSTBAGAS";
const char *password = "bagassdkangen";
// const char *ssid = "ASUS_X00TD";
// const char *password = "113333555555";

ESP8266WebServer server(80);
Servo myservo;

int servoPin = D3; // Pin servo terhubung ke D1 (GPIO5) di NodeMCU

RTC_DS1307 rtc;

String html;
String rh = "00";
String rm = "00";
int j1_h = 6;
int j1_m = 0;
int j2_h = 16;
int j2_m = 0;

void handleRoot()
{
  server.send(200, "text/html", html);
}

void handleServo()
{
  DateTime recent = rtc.now();
  rh = recent.hour();
  rm = recent.minute();
  String str_j1_h = String(to_string(j1_h).c_str());
  String str_j1_m = String(to_string(j1_m).c_str());
  String str_j2_h = String(to_string(j2_h).c_str());
  String str_j2_m = String(to_string(j2_m).c_str());
  html = "";
  html += "<html><head><title>Fish Feeder</title><link rel='icon' type='image/x-icon' href='https://www.clipartmax.com/png/small/307-3075608_betta-transparent-png-betta-fish-png.png'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'><style>body{background-image: url('https://images.unsplash.com/photo-1528460033278-a6ba57020470?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=435&q=80'); height: 100vh; background-size: cover;}</style></head><body class='d-flex align-items-center justify-content-center bg-image'><div class='text-center'><h1>Fish Feeder</h1><br><p>Jadwal pemberian pakan : <br>" + str_j1_h + "." + str_j1_m + " | " + str_j2_h + "." + str_j2_m + "</p><p>Terakhir diberi makan pada pukul " + rh + "." + rm + "</p><p>Saat ini pukul pukul " + recent.hour() + "." + recent.minute() + "." + recent.second() + "</p><p><button data-toggle='modal' data-target='#ganti' class='btn btn-secondary'>Ubah Jadwal</button> <a href=\'servo\'\'><button class='btn btn-primary'>Beri Makan</button></a></p></div><div class='modal fade' id='ganti' tabindex='-1' role='dialog' aria-labelledby='exampleModalCenterTitle' aria-hidden='true'><div class='modal-dialog modal-dialog-centered' role='document'><div class='modal-content'><div class='modal-header'><h5 class='modal-title' id='exampleModalLongTitle'>Ubah Jadwal</h5></div><div class='modal-body'><form action='/submit' method='post'><div class='form-group'><label for='jadwal1'>Jadwal 1</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j1-h' value='" + str_j1_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j1-m' value='" + str_j1_m + "'></div></div></div><div class='form-group mt-2'><label for='jadwal2'>Jadwal 2</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j2-h' value='" + str_j2_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j2-m' value='" + str_j2_m + "'></div></div></div></div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-dismiss='modal'>Batal</button><button type='submit' class='btn btn-primary'>Simpan</button></div></form></div></div></div><script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script></body></html>";
  myservo.write(45);
  delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
  myservo.write(90);
  server.send(200, "text/html", html);
}

void handleChange()
{
  if (server.hasArg("j1-h") && server.hasArg("j1-m") && server.hasArg("j2-h") && server.hasArg("j2-m"))
  {
    j1_h = server.arg("j1-h").toInt();
    j1_m = server.arg("j1-m").toInt();
    j2_h = server.arg("j2-h").toInt();
    j2_m = server.arg("j2-m").toInt();
    Serial.println("berhasil berubah");
    String str_j1_h = String(to_string(j1_h).c_str());
    String str_j1_m = String(to_string(j1_m).c_str());
    String str_j2_h = String(to_string(j2_h).c_str());
    String str_j2_m = String(to_string(j2_m).c_str());
    html = "";
    html += "<html><head><title>Fish Feeder</title><link rel='icon' type='image/x-icon' href='https://www.clipartmax.com/png/small/307-3075608_betta-transparent-png-betta-fish-png.png'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'><style>body{background-image: url('https://images.unsplash.com/photo-1528460033278-a6ba57020470?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=435&q=80'); height: 100vh; background-size: cover;}</style></head><body class='d-flex align-items-center justify-content-center bg-image'><div class='text-center'><h1>Fish Feeder</h1><br><p>Jadwal pemberian pakan : <br>" + str_j1_h + "." + str_j1_m + " | " + str_j2_h + "." + str_j2_m + "</p><p>Terakhir diberi makan pada pukul " + rh + "." + rm + "</p><p><button data-toggle='modal' data-target='#ganti' class='btn btn-secondary'>Ubah Jadwal</button> <a href=\'servo\'\'><button class='btn btn-primary'>Beri Makan</button></a></p></div><div class='modal fade' id='ganti' tabindex='-1' role='dialog' aria-labelledby='exampleModalCenterTitle' aria-hidden='true'><div class='modal-dialog modal-dialog-centered' role='document'><div class='modal-content'><div class='modal-header'><h5 class='modal-title' id='exampleModalLongTitle'>Ubah Jadwal</h5></div><div class='modal-body'><form action='/submit' method='post'><div class='form-group'><label for='jadwal1'>Jadwal 1</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j1-h' value='" + str_j1_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j1-m' value='" + str_j1_m + "'></div></div></div><div class='form-group mt-2'><label for='jadwal2'>Jadwal 2</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j2-h' value='" + str_j2_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j2-m' value='" + str_j2_m + "'></div></div></div></div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-dismiss='modal'>Batal</button><button type='submit' class='btn btn-primary'>Simpan</button></div></form></div></div></div><script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script></body></html>";
    server.send(200, "text/html", html);
  }
  else
  {
    Serial.println("terjadi kesalahan");
    server.send(200, "text/html", html);
  }
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

  String str_j1_h = String(to_string(j1_h).c_str());
  String str_j1_m = String(to_string(j1_m).c_str());
  String str_j2_h = String(to_string(j2_h).c_str());
  String str_j2_m = String(to_string(j2_m).c_str());
  html += "<html><head><title>Fish Feeder</title><link rel='icon' type='image/x-icon' href='https://www.clipartmax.com/png/small/307-3075608_betta-transparent-png-betta-fish-png.png'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'><style>body{background-image: url('https://images.unsplash.com/photo-1528460033278-a6ba57020470?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=435&q=80'); height: 100vh; background-size: cover;}</style></head><body class='d-flex align-items-center justify-content-center bg-image'><div class='text-center'><h1>Fish Feeder</h1><br><p>Jadwal pemberian pakan : <br>" + str_j1_h + "." + str_j1_m + " | " + str_j2_h + "." + str_j2_m + "</p><p>Terakhir diberi makan pada pukul " + rh + "." + rm + "</p><p><button data-toggle='modal' data-target='#ganti' class='btn btn-secondary'>Ubah Jadwal</button> <a href=\'servo\'\'><button class='btn btn-primary'>Beri Makan</button></a></p></div><div class='modal fade' id='ganti' tabindex='-1' role='dialog' aria-labelledby='exampleModalCenterTitle' aria-hidden='true'><div class='modal-dialog modal-dialog-centered' role='document'><div class='modal-content'><div class='modal-header'><h5 class='modal-title' id='exampleModalLongTitle'>Ubah Jadwal</h5></div><div class='modal-body'><form action='/submit' method='post'><div class='form-group'><label for='jadwal1'>Jadwal 1</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j1-h' value='" + str_j1_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j1-m' value='" + str_j1_m + "'></div></div></div><div class='form-group mt-2'><label for='jadwal2'>Jadwal 2</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j2-h' value='" + str_j2_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j2-m' value='" + str_j2_m + "'></div></div></div></div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-dismiss='modal'>Batal</button><button type='submit' class='btn btn-primary'>Simpan</button></div></form></div></div></div><script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script></body></html>";

  server.on("/", handleRoot);
  server.on("/servo", handleServo);
  server.on("/submit", handleChange);

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

  // Jadwalkan pergerakan servo pada pukul 6:00 dan 16:00
  if (hour == j1_h && minute == j1_m && second == 0)
  {
    rh = hour;
    rm = minute;
    String str_j1_h = String(to_string(j1_h).c_str());
    String str_j1_m = String(to_string(j1_m).c_str());
    String str_j2_h = String(to_string(j2_h).c_str());
    String str_j2_m = String(to_string(j2_m).c_str());
    html = "";
    html += "<html><head><title>Fish Feeder</title><link rel='icon' type='image/x-icon' href='https://www.clipartmax.com/png/small/307-3075608_betta-transparent-png-betta-fish-png.png'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'><style>body{background-image: url('https://images.unsplash.com/photo-1528460033278-a6ba57020470?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=435&q=80'); height: 100vh; background-size: cover;}</style></head><body class='d-flex align-items-center justify-content-center bg-image'><div class='text-center'><h1>Fish Feeder</h1><br><p>Jadwal pemberian pakan : <br>" + str_j1_h + "." + str_j1_m + " | " + str_j2_h + "." + str_j2_m + "</p><p>Terakhir diberi makan pada pukul " + rh + "." + rm + "</p><p>Saat ini pukul pukul " + hour + "." + minute + "." + second + "</p><p><button data-toggle='modal' data-target='#ganti' class='btn btn-secondary'>Ubah Jadwal</button> <a href=\'servo\'\'><button class='btn btn-primary'>Beri Makan</button></a></p></div><div class='modal fade' id='ganti' tabindex='-1' role='dialog' aria-labelledby='exampleModalCenterTitle' aria-hidden='true'><div class='modal-dialog modal-dialog-centered' role='document'><div class='modal-content'><div class='modal-header'><h5 class='modal-title' id='exampleModalLongTitle'>Ubah Jadwal</h5></div><div class='modal-body'><form action='/submit' method='post'><div class='form-group'><label for='jadwal1'>Jadwal 1</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j1-h' value='" + str_j1_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j1-m' value='" + str_j1_m + "'></div></div></div><div class='form-group mt-2'><label for='jadwal2'>Jadwal 2</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j2-h' value='" + str_j2_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j2-m' value='" + str_j2_m + "'></div></div></div></div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-dismiss='modal'>Batal</button><button type='submit' class='btn btn-primary'>Simpan</button></div></form></div></div></div><script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script></body></html>";
    myservo.write(45);
    delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
    myservo.write(90);
  }
  else if (hour == j2_h && minute == j2_m && second == 0)
  {
    rh = hour;
    rm = minute;
    String str_j1_h = String(to_string(j1_h).c_str());
    String str_j1_m = String(to_string(j1_m).c_str());
    String str_j2_h = String(to_string(j2_h).c_str());
    String str_j2_m = String(to_string(j2_m).c_str());
    html = "";
    html += "<html><head><title>Fish Feeder</title><link rel='icon' type='image/x-icon' href='https://www.clipartmax.com/png/small/307-3075608_betta-transparent-png-betta-fish-png.png'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM' crossorigin='anonymous'><style>body{background-image: url('https://images.unsplash.com/photo-1528460033278-a6ba57020470?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=435&q=80'); height: 100vh; background-size: cover;}</style></head><body class='d-flex align-items-center justify-content-center bg-image'><div class='text-center'><h1>Fish Feeder</h1><br><p>Jadwal pemberian pakan : <br>" + str_j1_h + "." + str_j1_m + " | " + str_j2_h + "." + str_j2_m + "</p><p>Terakhir diberi makan pada pukul " + rh + "." + rm + "</p><p>Saat ini pukul pukul " + hour + "." + minute + "." + second + "</p><p><button data-toggle='modal' data-target='#ganti' class='btn btn-secondary'>Ubah Jadwal</button> <a href=\'servo\'\'><button class='btn btn-primary'>Beri Makan</button></a></p></div><div class='modal fade' id='ganti' tabindex='-1' role='dialog' aria-labelledby='exampleModalCenterTitle' aria-hidden='true'><div class='modal-dialog modal-dialog-centered' role='document'><div class='modal-content'><div class='modal-header'><h5 class='modal-title' id='exampleModalLongTitle'>Ubah Jadwal</h5></div><div class='modal-body'><form action='/submit' method='post'><div class='form-group'><label for='jadwal1'>Jadwal 1</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j1-h' value='" + str_j1_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j1-m' value='" + str_j1_m + "'></div></div></div><div class='form-group mt-2'><label for='jadwal2'>Jadwal 2</label><div class='row'><div class='col'><input type='number' min='0' max='23' class='form-control' name='j2-h' value='" + str_j2_h + "'></div><div class='col'><input type='number' min='0' max='59' class='form-control' name='j2-m' value='" + str_j2_m + "'></div></div></div></div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-dismiss='modal'>Batal</button><button type='submit' class='btn btn-primary'>Simpan</button></div></form></div></div></div><script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script><script src='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script></body></html>";
    myservo.write(45);
    delay(2280); // Berikan waktu untuk servo mencapai posisi yang diinginkan
    myservo.write(90);
  }
  delay(1000);
}
