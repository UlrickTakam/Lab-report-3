/*Ultrasonic Sensor with DHT11
This program will factor in the speed of light to get a  more accurate distance value from the ultrasonic sensor.
It does so by using a formula that takes into account the temperature and humidity of the room (values taken from the DHT11)*/

int duration;
float distance; //Float stores distance value in cm
float meter; //Float stores distance value in m
int hum;    // Integer stores humidity value in percent
int temp;   // Integer stores temperature value in Celcius
float soundsp;  // Integer stores calculated speed of sound in M/S
float soundcm;  // Integer stores calculated speed of sound in cm/ms
int iterations = 5;

// Include NewPing Library and DHT Library
#include "NewPing.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7       // DHT11 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 11
#define TRIGGER_PIN  10 //define pin 10 as the trigger
#define ECHO_PIN     13 // define pin 13 as the echo
#define MAX_DISTANCE 400 //define 400 cm as the maximum distance the sensor can reach
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS; //Creating a delay value that is assigned to an unsigned integer

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
 lcd.begin(16, 2); // set up the LCD's number of columns and rows
  Serial.begin(9600); //initialize serial communication
  dht.begin(); // Initialize device
 pinMode(TRIGGER_PIN, OUTPUT); //define the trigger pin as an output
 digitalWrite(TRIGGER_PIN, LOW); //assign the value 0 to the trigger pin
 delayMicroseconds(2);
 pinMode(ECHO_PIN, INPUT); //define the echo pin as an output
 delay(6000);
 Serial.println("Distance:");
  // Print temperature sensor details.
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000; // Set delay between sensor readings based on sensor details
}
void loop()
{
  delay(delayMS);  // Delay so DHT11 sensor can stabalize
   sensors_event_t event;
   dht.humidity().getEvent(&event);
   dht.temperature().getEvent(&event);
    hum = event.relative_humidity;  // Get Humidity value
    temp= event.temperature;  // Get Temperature value
    
    // Calculate the Speed of Sound in M/S
    soundsp = 331.4 + (0.606 * temp) + (0.0124 * hum);
    
    // Convert to cm/ms
    
    soundcm = soundsp / 10000;
    
  duration = sonar.ping_median(iterations);
  
  // Calculate the distance
  distance = (duration / 2) * soundcm;
  
  // Send results to Serial Monitor and LCD
    Serial.print("Distance: ");
 
    if (distance >= 400 || distance <= 2) {
    Serial.print("Out of range");
    lcd.setCursor(0, 0);
    lcd.print("Out of range");
    }
    else {
    Serial.print("Distance: "); //print the distance in cm on the serial monitor
    Serial.print(distance);
    Serial.print("cm");
 lcd.setCursor(0, 0); //set the lcd cursor to the first column and first row
 lcd.print("Distance:"); //print the distance in cm on the LCD
 lcd.print(distance);
 lcd.print("cm");
 meter=distance/100; //equation for the distance in meters
 Serial.print("In meters: "); //print the distance in m on the serial monitor
 Serial.print(meter);
 Serial.println("m");
 lcd.setCursor(0, 1); //set the lcd cursor to the first column and second row
 lcd.print("In meters:"); //print the distance in m on the LCD
 lcd.print(meter);
 lcd.print("m");
    delay(500);
    }
  
  Serial.println(" ");
}
