//Constants
const int hygrometer = A0;  //Hygrometer sensor analog pin output at pin A0 of Arduino
const int greenLedPin = 3;
const int yellowLedPin = 5;
const int redLedPin = 6;
const int ledPin = 11;
const int buttonPin = 2;

//Variables
int brightness;
int newValue;
int value;

int green;
int yellow;
int red;

int buttonState = 0;

float humidityValue = 0;
float temperatureValue = 0;


/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x3F

// set the LCD address to 0x3F for the QunqiI2C 1602 16 chars 2 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3C, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#include <dht.h>

dht DHT;

#define DHT11_PIN 9 // set up the DHT to read from pin 7





void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(hygrometer, INPUT);
  pinMode(buttonPin, INPUT); // this is a NO button
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight

// ------- 3 quick blinks of backlight to confirm LED operation -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  
}

void loop() {

  int value;
  
  value = valueFunction(value);
  
  brightness = brightnessFunction(brightness, ledPin, 1023);

  if ((value <= 100) && (value >= 75))
  {
    digitalWrite(redLedPin, LOW);
    green = brightnessFunction(green, greenLedPin, 815);
    if (value > 80)
    {
      digitalWrite(yellowLedPin, LOW);
    }
    else if ((value > 74) && (value <= 80))
    {
      digitalWrite(yellowLedPin, HIGH);
    }
    //yellow = brightnessFunction(yellow, yellowLedPin);  //these commented out as the yellow seemed excessively bright during testing
    //yellow = yellow + 204;
  }

  if ((value < 80) && (value >= 60))
  {
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
  }

  else if (value < 60)
  {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    red = reverseBrightnessFunction(red, redLedPin);    
  }
  
  if (value != newValue) 
  {
    newValue = valueFunction(newValue);
    Serial.print("Soil humidity: ");
    Serial.print(newValue);
    Serial.println('%');
  }
   
  value = newValue;

  // read the data from the DHT11
  int chk = DHT.read11(DHT11_PIN);

  Serial.print("Read sensor: ");
    switch (chk)
    {
      case DHTLIB_OK: 
      Serial.println("OK"); 
        break;
        case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
        break;
        case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
        break;
        default: 
      Serial.println("Unknown error"); 
        break;
    }
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  int temperatureValue = DHT.temperature;
  lcd.print(Fahrenheit(temperatureValue), 0);
  lcd.print(char(223));
  lcd.print("|Soil:");
  delay(500);
  
  lcd.setCursor(0,1);
  lcd.print("Hum. = ");
  int humidityValue = DHT.humidity;
  lcd.print(humidityValue);
  lcd.print("%");
  lcd.print("|  ");
  lcd.print(value);
  lcd.print("%");
  delay(500);
  
  
  

}

double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

int humidity(int humidityValue) {
  humidityValue = DHT.humidity;
  return humidityValue;
}

int temperature(int temperatureValue) {
  temperatureValue = DHT.temperature;
  return temperatureValue;
}  


int valueFunction(int hygrometerValue)
{
  // When the plant is watered well the sensor will read a value 380~400, I will keep the 400
  // value but if you want you can change it below.
  hygrometerValue = analogRead(hygrometer);   //Read analog value
  hygrometerValue = constrain(hygrometerValue, 400, 1023); //Keep the ranges!
  hygrometerValue = map(hygrometerValue, 400, 1023, 100, 0); //Map value : 400 will be 100 and 1023 will be 0
  return hygrometerValue;

}

int brightnessFunction(int ledBrightness, int pin, int highValue)
{
  ledBrightness = analogRead(hygrometer);
  ledBrightness = constrain(ledBrightness, 400, 1023);
  ledBrightness = map(ledBrightness, 400, highValue, 255, 0);
  analogWrite(pin, ledBrightness);
  delay(500); //Read every 0.5 sec.
}

int reverseBrightnessFunction(int reverseLedBrightness, int pin)
{
  reverseLedBrightness = analogRead(hygrometer);
  reverseLedBrightness = constrain(reverseLedBrightness, 400, 1023);
  reverseLedBrightness = map(reverseLedBrightness, 400, 1023, 0, 255);
  analogWrite(pin, reverseLedBrightness);
  delay(500); //Read every 0.5 sec.
}
