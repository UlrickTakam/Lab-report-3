/* DHT Temperature & Humidity Sensor with Pushbutton
This program will read the temperature and the humidity of a room from the DTH 11 sensor that is connected to the Arduino board.
It will then write these values on the LCD
The pushbutton will change the displayed value from the Celsius degrees to the Farenheit degrees and the humidity*/

//Including the libraries needed for thsi program
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7   // Digital pin connected to the DHT sensor 

#define DHTTYPE    DHT11     // DHT 11 is the type of DHT sensor used


DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS; //Creating a delay value that is assigned to an unsigned integer
int button = 13;
int counter = 0;
int buttonState = 0;
int lastButtonState = 0;
int debounceDelay = 50;

#include <LiquidCrystal.h> // include the library code for the LCD

/* initialize the library by associating any needed LCD interface pin
with the arduino pin number it is connected to*/
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte customChar2[8] = { // Creating the degrees symbol in order to display it on the LCD
  0b00000,
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  Serial.begin(9600); //initialize serial communication
  dht.begin(); // Initialize device
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor; 
  dht.temperature().getSensor(&sensor);// Print temperature sensor details
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  dht.humidity().getSensor(&sensor); // Print humidity sensor details
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  delayMS = sensor.min_delay / 1000; // Set delay between sensor readings based on sensor details
  lcd.createChar(2, customChar2);
}

void loop() {
// read the current state of the button
int reading = digitalRead(button);

// check if the button state has changed
if (reading != lastButtonState) {
// delay for the debounce period
delay(debounceDelay);
// read the button state again to make sure it's stable
reading = digitalRead(button);

// update the button state if it's stable
if (reading != buttonState) {
buttonState = reading;

// check if the button was just pressed
if (buttonState == LOW) {
// increment the counter
counter++;

// reset the counter if we've reached the maximum count of 2
if (counter > 2) {
counter = 0;
}
}
}
}

// execute the corresponding task based on the current counter value
if (counter == 0) {
  // Get temperature event and print its value
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) { // If Arduino isn't picking up values from the sensor, display the message "Error"
    Serial.println(F("Error reading temperature!"));
    lcd.setCursor(0, 0); //Set the cursor at the first column and first row
    lcd.print("Error temperature");
  }
  else {
    lcd.setCursor(0, 0); //Set the cursor at the first column and first row
    lcd.print(("Temp: "));
    lcd.print(event.temperature); // Print the temperature value
    lcd.setCursor(11, 0); //Set the cursor at the 12th column and first row
    lcd.write((byte)2); //Print the degrees symbol created earlier
    lcd.setCursor(12, 0); //Set the cursor at the 13th column and first row
    lcd.print(("C"));
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
} else if (counter == 1) {
  // Get temperature event and print its value
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) { // If Arduino isn't picking up values from the sensor, display the message "Error"
    Serial.println(F("Error reading temperature!"));
    lcd.setCursor(0, 0); //Set the cursor at the first column and first row
    lcd.print("Error temperature");

  }
  else {
    int farenheit = (event.temperature * 1.8) + 32;
    lcd.setCursor(0, 0); //Set the cursor at the first column and first row
    lcd.print(("Temp: "));
    lcd.print(farenheit); // Print the temperature value
    lcd.setCursor(11, 0); //Set the cursor at the 12th column and first row
    lcd.write((byte)2); //Print the degrees symbol created earlier
    lcd.setCursor(12, 0); //Set the cursor at the 13th column and first row
    lcd.print(("F"));
    Serial.print(F("Temperature: "));
    Serial.print(farenheit);
    Serial.println(F("°F"));
  }
} else if (counter == 2) {
 // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) { // If Arduino isn't picking up values from the sensor, display the message "Error"
    Serial.println(F("Error reading humidity!"));
    lcd.setCursor(0, 1);
    lcd.print("Error humidity");
  }
  else {
    lcd.setCursor(0, 1); //Set the cursor at the first column and second row
    lcd.print(("Humidity:"));
    lcd.print(event.relative_humidity); // Print the humidity value
    lcd.print(("%"));
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}

// remember the current button state for the next iteration
lastButtonState = reading;
delay(200); // Delay of 200 milliseconds between the loops
}
