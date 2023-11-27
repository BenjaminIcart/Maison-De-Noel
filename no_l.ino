#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>;
#include <Fonts/FreeSerifBoldItalic9pt7b.h> 
     // Gras + Italique (en 9, 12, 18, et 24 pts)
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>              // Gras (en 9, 12, 18, et 24 pts)
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMono9pt7b.h>  
//-----------Setup Screen-------------------
#define OLED_RESET LED_BUILTIN
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//-----------Wifi Settings-------------------
const char *ssid = "MOMORPRODS";
const char *password = "QPVWHA4KUC7E9";

const char *ssid2 = "Wifi2";
const char *password2 = "passwd2";

//-----------Time Settings-------------------
#define UTC 1

int goalH = 0;
int goalM = 0;
int goalS = 1;

int goalD = 25;
int goalMo = 12;

String textBottom = "Avant Noel";

//-----------Pins Settings-------------------
#define pinLED1 D3
#define pinLED2 D4

int timer, timerD, timerH, timerM, timerS;

void setup()
{
  
  display.setRotation(90);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Scan Wi-Fi ...");
  display.display();
 
  delay(200);
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  Serial.println(n);
  for (int i = 0; i < n; ++i)
  {
    if (WiFi.SSID(i) == ssid)
    {
      WiFi.begin(ssid, password);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Connexion au Wi-Fi: ");
      display.println(ssid);
      display.display();
      break;
    }
    if (WiFi.SSID(i) == ssid2)
    {
      WiFi.begin(ssid2, password2);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Connexion au Wi-Fi: ");
      display.println(ssid2);
      display.display();
      break;
    }
  }
  display.println("\nConnexion");
  display.display();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    display.print(".");
    display.display();
  }
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.println("Wifi Connecte!");
  display.print("IP:");
  display.println(WiFi.localIP());
  display.display();

  configTime(UTC * 3600, 0, "fr.pool.ntp.org", "time.nist.gov");
  display.println("\nWaiting for NTP...");
  delay(2000);
  while (!time(nullptr))
  {
    delay(1000);
  }
  display.println("\nConnexion NTP....OK");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();

  time_t now = time(nullptr);
  while (year(now) == 1970)
  {
    now = time(nullptr);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Recuperation de \nl'heure...");
    display.display();
  }
  
}

void displayTimer(int x, int y)
{

  display.setCursor(5, 37);
  display.setFont(&FreeSans12pt7b);
  if (timerH < 10)
    display.print("0");
  display.print(timerH);
  display.setFont(&FreeSans9pt7b);
  display.print(" : ");
  display.setFont(&FreeSans12pt7b);
  if (timerM < 10)
    display.print("0");
  display.print(timerM);
  display.setFont(&FreeSans9pt7b);
  display.print(" : ");
  display.setFont(&FreeSans12pt7b);
  if (timerS < 10)
    display.print("0");
  display.print(timerS);
  display.setFont(&FreeSans9pt7b);
 
}

time_t get_time_t(char *date_string)
{
  tmElements_t tm;
  char ds[20];

  strncpy(ds, date_string, 20);

  tm.Second = atoi(&ds[17]);
  tm.Minute = atoi(&ds[14]);
  tm.Hour = atoi(&ds[11]);
  tm.Day = atoi(&ds[8]);
  tm.Month = atoi(&ds[5]);
  tm.Year = atoi(&ds[0]) - 1970;
  return (makeTime(tm));
}

void printText(int i, time_t now)
{
  if (day(now) > 25 && month(now) == 12)
  {
    if (i == 0) //Day > 1
    {
      display.setTextSize(1);
      display.setFont(&FreeMonoBold9pt7b);
      display.setCursor(5, 80);
      
    }
    else if (i == 1) //Day < 1
    {
      textBottom = "Nouvel An";
      display.setFont(&FreeMonoBold9pt7b);
      display.setCursor(15, 60);
    }
  }
  else
  {
    if (i == 0) //Day > 1
    {
       
      display.setFont(&FreeSans9pt7b);
      display.setCursor(20, 55);
    }
    else if (i == 1) //Day < 1
    {
      display.setFont(&FreeSans9pt7b);
      display.setCursor(15, 60);
    }
  }
  display.print(textBottom);
}

void loop()
{

  time_t now = time(nullptr);
  int timerY;
  if (goalMo < month(now))
    timerY = year(now) + 1;
  else
    timerY = year(now);

  if (day(now) > 25 && month(now) == 12)
  {
    goalD = 1;
    goalMo = 1;
    textBottom = "Avant le Nouvel An";
  }
  char buff[128];
  snprintf(buff, 128, "%d.%02d.%02d %02d:%02d:%02d", timerY, goalMo, goalD, goalH, goalM, goalS);
  time_t goal = get_time_t(buff);

  time_t local = now;
  local = now + UTC * 60 * 60;
  now = local;

  timer = goal - local;

  timerD = timer / 86400;
  timerH = ((timer - (timerD * 86400)) / 3600);
  timerM = (timer - ((timerD * 86400) + (timerH * 3600))) / 60;
  timerS = (timer - ((timerD * 86400) + (timerH * 3600) + (timerM * 60)));

  Serial.println("-------Now:------");
  Serial.println(day(now));
  Serial.println(month(now));
  Serial.println(hour(now));
  Serial.println(minute(now));
  Serial.println(second(now));
  Serial.println("--------------------");

  Serial.print("Jours Restants: ");
  Serial.println(timerD);
  Serial.print("Heures Restantes: ");
  Serial.println(timerH);
  Serial.print("Minutes Restantes: ");
  Serial.println(timerM);
  Serial.print("Secondes Restantes: ");
  Serial.println(timerS);
  Serial.println("--------------------");

  display.clearDisplay();
  display.setFont();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  if (timerD > 0)
  {
    display.setCursor(23, 10);
    display.setFont(&FreeSans12pt7b);
    display.print(timerD);
    display.setFont();
      display.setTextSize(1);
      display.setFont(&FreeMonoBold9pt7b);
    display.print("JOUR");

    if (timerD > 1)
    {
      display.print("S");
      display.setTextSize(1);
      
    }
    displayTimer(3, 40);
    printText(0, now);
  }
  else if (timerD <= 0 && timerH > 0)
  {
    displayTimer(0, 30);
    printText(1, now);
  }
  else if (timerD <= 0 && timerH <= 0)
  {
    display.setCursor(20, 30);
    display.setFont(&FreeSans18pt7b);
    display.print(timerM);
    display.setFont(&FreeSans12pt7b);
    display.print(":");
    display.setFont(&FreeSans18pt7b);
    display.print(timerS);
    display.setFont(&FreeSans12pt7b);
    display.print(":");
    printText(1, now);
  }

  if (timerD <= 0 && timerH < 0)
  {
    if (month(now) == 12)
    {
      display.clearDisplay();
      display.setFont(&FreeSans12pt7b);
      display.setCursor(25, 25);
      display.print("Joyeux");
      display.setCursor(35, 55);
      display.print("Noel !");
    }
  }
  if (month(now) == 1)
  {
    display.clearDisplay();
    display.setFont(&FreeSans12pt7b);
    display.setCursor(25, 25);
    display.print("Bonne");
    display.setCursor(25, 55);
    display.print("Annee!");
  }
  display.display();

  analogWrite(pinLED1, random(120, 255));
  analogWrite(pinLED2, random(120, 255));

  delay(random(10, 100));
} 
