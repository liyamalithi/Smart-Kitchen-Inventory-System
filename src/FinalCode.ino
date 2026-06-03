#define BLYNK_TEMPLATE_ID "TMPL6LLHs4YfH"
#define BLYNK_TEMPLATE_NAME "kitchen inventory system"
#define BLYNK_AUTH_TOKEN "gz31KOh7qmT3sGrdjTYcekOPutEinZKX"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

Servo myservo;

#define basketHeight 13 

//  ultrasonic sensor pins
const int trigPin = 5;  // Trigger pin
const int echoPin = 18; // Echo pin

long duration;
int distance;
int x_data[2];
int wholeSum;

// Initialize the LCD with I2C address 0x27 (this might need to be changed depending on your LCD model)
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Malithi";
char pass[] = "11223344";

BlynkTimer timer;

int MaxLevel = 100;

void setup() {

    //ultrasonic
 pinMode(trigPin,OUTPUT);  
 pinMode(echoPin,INPUT);

  // Start the serial communication
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); 

  myservo.setPeriodHertz(50);    // standard 50 hz servo(MG90 is a 50Hz Motor)
	myservo.attach(13, 1000, 2000); // attaches the servo on pin 13 to the servo object

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

 
}

void loop() {
 
  lcd.setCursor(0, 0);
  lcd.print("Level: ");

  if(touchRead(4)<30){
      myservo.write(90); 

  }
  else if(touchRead(4)>30){
      myservo.write(0); 

  }

   //Ultrasonic sensor              
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);
  int dis = duration * 0.034 / 2;

//moving avarage filter for ultrasonic sensor   
 for(int i =0;i<2;i++){
    x_data[i] = dis;
  }
  for(int j =0;j<2;j++){
    wholeSum = wholeSum + x_data[j] ;
  }
  distance = wholeSum / 2;

  
  x_data[2] ={0};
  wholeSum = 0;

  int persentage = ((basketHeight - distance) * 100)/ basketHeight;

  //int blynkDistance = (15-distance);
  if (persentage <= MaxLevel) {
    Blynk.virtualWrite(V0, persentage);
  } else {
    Blynk.virtualWrite(V0, 0);
  }

  // Print the distance on the LCD
  lcd.setCursor(7, 0);
  lcd.print(distance);
  lcd.print(" cm  "); // Clear previous reading

  lcd.setCursor(0, 1);
  lcd.print("Presentage: ");

  lcd.setCursor(11, 1);
  lcd.print(persentage);
  lcd.print(" % ");
  
  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Add a short delay before the next reading
  delay(200);
   lcd.clear(); 
}
