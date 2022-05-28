#include<ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "Your wifi name"
#define WLAN_PASS       "wifi password"
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "username"
#define AIO_KEY         "your key"
/**************************PINS Setup ********************************************/

int Left_Fire = D6 ;            
int Mid_Fire = A0 ;          
int Right_Fire = D4 ;             

LiquidCrystal_I2C lcd(0x27, 16, 2);
int SCL_pin1 = 5; 
int SDA_pin1 = 4; 

int fireleft;
int firemid;
int fireright;
int fire1;
int fire2;
int fire3;
int duration2;
int distanceCm2;
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

const int trigPin2 = D7;           
const int echoPin2 = D8;
          
WiFiUDP udp;
NTPClient timeClient(udp,"time.nist.gov",7200);
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish firel = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/firel");
Adafruit_MQTT_Publish firem = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/firem");
Adafruit_MQTT_Publish firer = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/firer");
Adafruit_MQTT_Publish distance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/distance");
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void setup() {
  Serial.begin(115200);
  
  pinMode(Left_Fire, INPUT);  // declare Left_Fire sensor as input
  pinMode(Mid_Fire, INPUT);  // declare Mid_Fire sensor as input
  pinMode(Right_Fire, INPUT);  // declare Right_Fire sensor as input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  
  pinMode(SCL_pin1, OUTPUT);
  pinMode(SDA_pin1, OUTPUT);
  digitalWrite(SDA_pin1,LOW);
  digitalWrite(SCL_pin1, LOW);
  Wire.begin(SDA_pin1 ,SCL_pin1);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.clear();
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  lcd.print("> Fire Fighter <");
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print(" Robot  Project ");


  Serial.println(F("Adafruit MQTT demo"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  timeClient.begin();
  delay(1000);
  
  lcd.clear();
 
}
uint32_t x=0;
void loop() {

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

//**************Sensors*******************
fireleft = digitalRead(Left_Fire);
firemid = analogRead(Mid_Fire);
fireright = digitalRead(Right_Fire);

//Second Ultrasonic
// Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculate the distance
  distanceCm2 = duration2 * SOUND_VELOCITY/2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance2 (cm): ");
  Serial.println(distanceCm2);
  delay(1000);

  if(fireleft == 0 ){
    fire1 = 181;
    }
  if(fireright == 0){
    fire3 = 179 ;
    }
  else{
    fire1 = 0 ;
    fire3 = 0 ;
  }
  fire2 = firemid+100;
  Serial.print(fire1);
  Serial.print(fire2);
  Serial.print(fire3);
  
  lcd.setCursor(0, 0);
  lcd.print("FDs : ");
  lcd.print(fire1);
  lcd.print("*");
  lcd.print(fire2);
  lcd.print("*");
  lcd.print(fire3);
  lcd.print("*");
  lcd.setCursor(0, 1);
  lcd.print("Distance : ");
  lcd.print(distanceCm2);

  firel.publish(int(fire1));
  firem.publish(int(fire2));
  firer.publish(int(fire3));
  distance.publish(int(distanceCm2));
  delay(10000);
  lcd.clear();
   
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
