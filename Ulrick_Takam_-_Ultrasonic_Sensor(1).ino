int trig=10; //define pin 10 as the trigger
int echo=13; // define pin 13 as the echo
int duration;
float distance; //Float stores distance value in cm
float meter; //Float stores distance value in m

// include the library code for the LCD
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
 lcd.begin(16, 2); // set up the LCD's number of columns and rows
 Serial.begin(9600); //initialize serial communication
 pinMode(trig, OUTPUT); //define the trigger pin as an output
 digitalWrite(trig, LOW); //assign the value 0 to the trigger pin
 delayMicroseconds(2);
 pinMode(echo, INPUT); //define the echo pin as an output
 delay(6000); //Delay to stabilize the program and device
 Serial.println("Distance:");
}
void loop()
{
 digitalWrite(trig, HIGH); //write the trigger value as 1
 delayMicroseconds(10); //wait 10 microseconds
 digitalWrite(trig, LOW); //write the trigger value as 0
 duration = pulseIn(echo, HIGH); //make the echo value pulse from 1 to 0 continuously
 if(duration>=38000.00){ //if the signal from the echo takes 38000 mircroseconds or more to come back tot he trigger, display out of range on the serial monitor and LCD
 Serial.print("Out range");
 lcd.setCursor(0, 0);
 lcd.print("Out of range");
 }
 else{ //if not, execute this program
 distance = duration/58.00; //equation for the distance in cm
 Serial.print(distance); //print the distance in cm on the serial monitor
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
 }
 delay(1000); //delay between the loops
}
