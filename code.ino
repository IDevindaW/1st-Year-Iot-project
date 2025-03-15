#include <Arduino.h> 
#include <NewPing.h> 
#include <SoftwareSerial.h> 
#include <DFRobotDFPlayerMini.h> 
#include <Wire.h> 
#include <RTClib.h> 
#define MAX_DISTANCE 400 
const int trigPinDown = 2; 
const int echoPinDown = 3; 
const int trigPinUp = 4; 
const int echoPinUp = 5; 
const int pinIRa = A0; // Analog output from IR sensor connected to analog pin A0 
const int motorPin = 9; // Digital pin with PWM capability connected to transistor base  
const int buttonPin = 7; 
int minDownDistance = 100; 
int minUpDistance = 100; 
static unsigned long timer = millis(); 
int waitTime = 3500; 
NewPing sonarDown(trigPinDown, echoPinDown, MAX_DISTANCE); 
NewPing sonarUp(trigPinUp, echoPinUp, MAX_DISTANCE); 
SoftwareSerial mp3SoftwareSerial(10, 11); // TX,RX for DFPlayer Mini 
DFRobotDFPlayerMini DFPlayer; 
RTC_DS3231 rtc; 
 
void printDetail(uint8_t type, int value) { 
  switch (type) { 
    case TimeOut: 
      Serial.println(F("Time Out!")); 
      break; 
    case WrongStack: 
      Serial.println(F("Stack Wrong!")); 
      break; 
    case DFPlayerCardInserted: 
      Serial.println(F("Card Inserted!")); 
      break; 
    case DFPlayerCardRemoved: 
      Serial.println(F("Card Removed!")); 
      break; 
    case DFPlayerCardOnline: 
      Serial.println(F("Card Online!")); 
      break; 
    case DFPlayerPlayFinished: 
      Serial.print(F("Number:")); 
      Serial.print(value); 
      Serial.println(F(" Play Finished!")); 
      break; 
    case DFPlayerError: 
      Serial.print(F("DFPlayerError:")); 
      switch (value) { 
        case Busy: 
          Serial.println(F("Card not found")); 
          break; 
        case Sleeping: 
          Serial.println(F("Sleeping")); 
          break; 
        case SerialWrongStack: 
          Serial.println(F("Get Wrong Stack")); 
          break; 
        case CheckSumNotMatch: 
          Serial.println(F("Check Sum Not Match")); 
          break; 
        case FileIndexOut: 
          Serial.println(F("File Index Out of Bound")); 
          break; 
        case FileMismatch: 
          Serial.println(F("Cannot Find File")); 
          break; 
        case Advertise: 
          Serial.println(F("In Advertise")); 
          break; 
        default: 
          break; 
      } 
      break; 
    default: 
      break; 
  } 
} 
 
void leftAlert(int distance) { 
  if (millis() - timer > waitTime) { 
    timer = millis(); 
    DFPlayer.playLargeFolder(1, distance + 1); 
  } 
  if (DFPlayer.available()) { 
    printDetail(DFPlayer.readType(), DFPlayer.read()); // Print the detail message from DFPlayer 
  } 
} 
 
void rightAlert(int distance) { 
  if (millis() - timer > waitTime) { 
    timer = millis(); 
    DFPlayer.playLargeFolder(2, distance + 1); 
  } 
  if (DFPlayer.available()) { 
    printDetail(DFPlayer.readType(), DFPlayer.read()); // Print the detail message from DFPlayer 
  } 
} 
 
void setupDFPlayer() { 
  mp3SoftwareSerial.begin(9600); 
  Serial.println(); 
  Serial.println(F("Blind assistance smart glass...")); 
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)")); 
 
  if (!DFPlayer.begin(mp3SoftwareSerial)) { 
    Serial.println(F("Unable to begin:")); 
    Serial.println(F("1.Please recheck the connection!")); 
    Serial.println(F("2.Please insert the SD card!")); 
while (true); 
} 
Serial.println(F("DFPlayer Mini online.")); 
DFPlayer.setTimeOut(500); // Set serial communication time out 500ms 
DFPlayer.volume(30);      
// Set volume value (0~30) 
DFPlayer.EQ(DFPLAYER_EQ_NORMAL); 
DFPlayer.outputDevice(DFPLAYER_DEVICE_SD); 
} 
// Function to convert the sensor reading to distance in cm 
f
 loat calculateDistance(int sensorValue) { 
// Convert the analog value to voltage 
f
 loat voltage = sensorValue * (5.0 / 1023.0); 
// Apply the formula to convert voltage to distance for the Sharp 2Y0A21F16 sensor 
f
 loat distance = 29.988 * pow(voltage, -1.173); 
return distance; 
} 
void setup() { 
Serial.begin(9600); 
setupDFPlayer(); 
pinMode(pinIRa, INPUT);  // Set analog pin as input 
pinMode(motorPin, OUTPUT);  // Set motorPin as an output 
digitalWrite(motorPin, LOW);  // Ensure motor is off initially 
pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor for the button 
  Wire.begin(); 
  if (!rtc.begin()) { 
    Serial.println("Couldn't find RTC"); 
    while (1); 
  } 
 
  if (rtc.lostPower()) { 
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    Serial.println("RTC power lost, adjusted to compile time"); 
  } 
} 
 
void loop() { 
  delay(29); 
  int downDistance = sonarDown.ping_cm(); 
  int upDistance = sonarUp.ping_cm(); 
 
  if (downDistance < minDownDistance && downDistance > 1) { 
    Serial.print("Obstacle "); 
    Serial.print(downDistance); 
    Serial.println(" CM below"); 
 
    leftAlert(downDistance); 
  } 
 
  if (upDistance < minUpDistance && upDistance > 1) { 
    Serial.print("Obstacle "); 
    Serial.print(upDistance); 
    Serial.println(" CM ahead"); 
 
    rightAlert(upDistance); 
  } 
 
  int IRvalueA = analogRead(pinIRa);  // Read the analog value from the sensor 
  int motorIntensity = 0;  // Initialize motor intensity 
 
  // Calculate the distance from the analog value 
  float distance = calculateDistance(IRvalueA); 
 
  if (distance <= 60) {  // Distance is 60 cm or less 
    if (distance <= 10) {  // Closest distance (<= 10 cm) 
      motorIntensity = 255;   
    } else if (distance <= 15) {  // Distance between 10 and 15 cm 
      motorIntensity = 229;   
    } else if (distance <= 20) {  // Distance between 15 and 20 cm 
      motorIntensity = 204;  
    } else if (distance <= 25) {  // Distance between 20 and 25 cm 
      motorIntensity = 178;   
    } else if (distance <= 30) {  // Distance between 25 and 30 cm 
      motorIntensity = 153;  
    } else if (distance <= 35) {  // Distance between 30 and 35 cm 
      motorIntensity = 127;  
    } else if (distance <= 40) {  // Distance between 35 and 40 cm 
      motorIntensity = 102;  
    } else if (distance <= 45) {  // Distance between 40 and 45 cm 
      motorIntensity = 76;  
    } else if (distance <= 50) {  // Distance between 45 and 50 cm 
      motorIntensity = 51;   
    } else {  // Distance between 50 and 60 cm 
      motorIntensity = 25;  
    } 
  } else { 
    motorIntensity = 0;  // Distance greater than 30 cm, motor off 
  } 
 
  analogWrite(motorPin, motorIntensity);  // Set the motor intensity based on the distance  
 
  // Print the distance and motor intensity to the Serial Monitor 
  Serial.print("IR Distance = "); 
  Serial.print(distance); 
  Serial.print(" cm\tMotor Intensity = "); 
  Serial.println(motorIntensity); 
 
  // Print the ultrasonic distances 
  Serial.print("Left Distance = "); 
  Serial.print(downDistance); 
  Serial.print(" cm\tRight Distance = "); 
  Serial.println(upDistance); 
 
  // Check if the button is pressed 
  if (digitalRead(buttonPin) == LOW) { // Button is pressed 
    // Get current time from RTC 
    DateTime now = rtc.now(); 
    int hour = now.hour(); 
    int minute = now.minute(); 
 
    // Calculate folder and file number based on time 
    int folderNumber = 3 + hour;  // Folders 03 to 26 (for hours 0 to 23) 
    int fileNumber = minute;      // Files 00 to 59 
 
    // Adjust for 11 PM (23:00) which should be in folder 26 
    if (hour == 23) { 
      folderNumber = 26; 
    } 
 
    Serial.print("Playing folder: "); 
    Serial.println(folderNumber); 
    Serial.print("File number: "); 
    Serial.println(fileNumber); 
 
    // Play the audio file 
    DFPlayer.playLargeFolder(folderNumber, fileNumber); 
 
    // Debounce delay to avoid multiple triggers 
    delay(500); 
  } 
 
  delay(100);  // Delay for stability 
} 