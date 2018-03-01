#include <Adafruit_DotStar.h>
#include <SPI.h>         

#define NUMPIXELS 144 // Number of LEDs in strip

#define DATAPIN    4  //yellow
#define CLOCKPIN   5  //green
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
  
const int buttonPin = 2;     // pressure plate
const int ledPin =  13;      // out to elWire

// variables will change:
int buttonState = 0;         // variable for reading the push button status

bool entry = 0;
float myClock = 0;
int stripCount = 0;



void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);     
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}


int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xffffff;      // 'On' color (white)

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  if (buttonState == LOW) {    
   entry = 1;   
   myClock = millis();
  }

//if someone has entered the space, run the LED pattern
if(entry == 1){
  digitalWrite(ledPin, HIGH); 
  if (millis() - myClock < 14000){
      strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
  }

  else {
    digitalWrite(ledPin, LOW);
    for(int i = 0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, 0);
    }
    strip.show();                     // Refresh strip
    delay(20);
    
  }

}

  



} 
