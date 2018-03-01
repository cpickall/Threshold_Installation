
#include <NewPing.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 240 // Maximum distance to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//variables for smoothing functions
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average



void setup() {
  Serial.begin(115200);
  //initialize smoothing array
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;  
}

void loop() {
  delay(50);                      // Wait 50ms between pings
  float uS = sonar.ping_cm(); 
  //smooth the signal
   // subtract the last reading:
  total= total - readings[readIndex];         
  // read from the sensor:  
  readings[readIndex] = uS;
  // add the reading to the total:
  total= total + readings[readIndex];       
  // advance to the next position in the array:  
  readIndex = readIndex + 1;                    

  // if we're at the end of the array...
  if (readIndex >= numReadings)              
    // ...wrap around to the beginning: 
    readIndex = 0;                           

  // calculate the average:
  average = total / numReadings;   
  
  if(average > 5){
    Serial.println(average);
  }
}
