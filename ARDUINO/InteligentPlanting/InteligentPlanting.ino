#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial =  SoftwareSerial(7, 6);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int switchPin = 6;
int relay = 8;
int incomingByte; 
int sensorVlaga = A0;    
int sensorSvetlina = A1; 
int sensorValue = 0; 
int sensorPin = A3;
float temperature;
int Humidity;
int Light;

void setup() {
  lcd.begin(16, 2);
  
  pinMode(relay, OUTPUT);
  pinMode (sensorSvetlina, OUTPUT);
  pinMode(7, INPUT);
  pinMode(6, OUTPUT);
  
  Serial.begin (9600);
  mySerial.begin(9600);
  
}


void displayValue(char *value, int cursorX, int cursorY){
  lcd.setCursor(cursorX, cursorY);
  lcd.print(value);
}

void displayValue(float value, int cursorX, int cursorY){
  lcd.setCursor(cursorX, cursorY);	
  lcd.print(value);
}

void displayValue(int value, int cursorX, int cursorY){
  lcd.setCursor(cursorX, cursorY);	
  lcd.print(value);
}

void onSerialInput(char incomingByte){
  digitalWrite(relay, HIGH);
  delay(2);
}

void offSerialInput(char incomingByte){
  digitalWrite(relay, LOW);
  delay(2);
}

float calculateTemperature(int sensorPin){
  return (((analogRead(sensorPin) / 1024.0) * 5.0) - 0.5) * 100;
}

int calculateHumidity(int humidity){
  return 100 - int(analogRead(humidity)/10.23);
}

int calculateLight(int light){
  return analogRead(light);
}

void readDisplaySensors(){
  displayValue("Temp", 0,0);
  temperature = calculateTemperature(sensorPin);
  displayValue(temperature, 5, 0);
  delay(100);
  displayValue("Vlaga", 0,1);
  Humidity = calculateHumidity(sensorVlaga);
  displayValue(Humidity, 6, 1);
  displayValue("%", 8,1);
  delay(100);
  displayValue("Light", 11,0);
  displayValue("   ", 11,1);
  Light = calculateLight(sensorSvetlina) + 5;
  displayValue(Light, 11, 1);
  displayValue("%", 13,1);
  
}

void sendSensorsDataToRasberry(){
 mySerial.print(temperature );
 mySerial.print("\r");
 mySerial.print(Humidity );
 mySerial.print("\r");
 mySerial.print(Light );
 mySerial.print("\r");
}

void loop() {
  
  readDisplaySensors();
  delay(100);
  
  if (mySerial.available()) {
    int Comand = mySerial.read();
    if(Comand == 1){
     sendSensorsDataToRasberry();
    } 
    else if(Comand == 2){
    onSerialInput(mySerial.read());
    } 
    else if(Comand == 3){
    offSerialInput(mySerial.read());
    } 
    else { 
      mySerial.print("Error\r");
    }
  }
    
}

