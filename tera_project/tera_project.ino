#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include <Servo.h> // Add the servo library
 
int flag = 0;
// Define the pins for the LCD
const int lcdRS = 12;
const int lcdEN = 11;
const int lcdD4 = 10;
const int lcdD5 = 9;
const int lcdD6 = 8;
const int lcdD7 = 7;

// Create an instance of the LCD library
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

// Define the pin for the temperature sensor
const int tempPin = 6; 

// Create an instance of the OneWire library
OneWire oneWire(tempPin);

// Create an instance of the DallasTemperature library
DallasTemperature sensors(&oneWire);

// Define the pin for the DHT11 sensor
const int dhtPin = 2;

// Create an instance of the DHT library
DHT dht(dhtPin, DHT11);

// Define the pin for the fan
const int fanPin = 3;

// Define the pin for the servo motor
const int servoPin = 5; 

// Define the pin for the PIR motion sensor
const int pirPin = A0; 

// Create an instance of the Servo library
Servo servo; 

unsigned long lastTrigger = 0; // Variable to store the time of the last motion detection

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

  sensors.begin(); 

  // Initialize the DHT11 sensor
  dht.begin();

  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize the fan pin as an output
  pinMode(fanPin, OUTPUT);

  // Initialize the servo pin as an output
  pinMode(servoPin, OUTPUT);

  // Initialize the PIR motion sensor pin as an input
  pinMode(pirPin, INPUT);

  // Attach the servo motor and wait for it to settle
  servo.attach(servoPin); 
  delay(100); // Add a short delay to allow the servo motor to settle
}

void loop() {
  // Turn on the fan
  digitalWrite(fanPin, HIGH);

  // Read the temperature from the DS18B20 sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Read the humidity from the DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();


  // Clear the LCD
  lcd.setCursor(0, 1);
  lcd.print("                ");

lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

  // Display the temperature and humidity on the LCD
  lcd.setCursor(0, 1);
  lcd.print(tempC);
  lcd.print("C ");
  lcd.print(humidity);
  lcd.print("%");

  // Print the values to the serial console
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Check if the temperature is 30°C or higher
  if (temperature >= 30) {
    // Turn on the fan
    digitalWrite(fanPin, HIGH);
  } else {
    // Turn off the fan
    digitalWrite(fanPin, LOW);
  }

  // Check if the humidity is 50% or higher
  if (humidity >= 85 && flag == 0) {
   // scan from 0 to 180 degrees
    for(int angle = 0; angle < 180; angle++) {
        servo.write(angle);
        delay(15);
    }
    flag = 1;
    delay(1000);
  }
 else if (humidity <= 85 ){
    // now scan back from 180 to 0 degrees
    for(int angle = 180; angle > 0; angle--) {
        servo.write(angle);
        delay(15);
        flag = 0;
    }
  }

// Check for motion using the PIR motion sensor
  int pirState = digitalRead(pirPin);
  if (pirState == LOW) {
    // Display a message on the LCD when motion is detected
    lcd.setCursor(0, 0);
    lcd.print("Motion detected!");
    lcd.setCursor(0, 1);
    lcd.print("Weird insect alert!");
    delay(2000); // Wait for 2 seconds before clearing the LCD
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  // Wait for 1 second before taking the next reading
  delay(500);
}