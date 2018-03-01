import processing.serial.*;
Serial myPort;  // The serial port
int fade = 0;

import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

String inString;

void setup(){
  
  oscP5 = new OscP5(this,12001);
  //PUT ADDRESS OF MACKBOOK PRO HERE
  myRemoteLocation = new NetAddress("192.168.1.49",12000);
  

  // Open serial port to read sensor date from the arduino
  myPort = new Serial(this, Serial.list()[5], 115200);
  myPort.clear();
  myPort.bufferUntil('\n');
  size(1920, 1080);
  background(0);
  frameRate(12);
  
}

void draw(){
background(fade);
}

void serialEvent (Serial myPort) {
 // get the ASCII string - sensor data sent by arduino connected to distance sensor
 inString = myPort.readStringUntil('\n');
 if (inString != null) {
 // trim off any whitespace:
 inString = trim(inString);
 float dist = float(inString); 
//make color fade from black to white as person approaches the sensor
 if (dist < 50){
   fade = 255;
 } else if(dist > 100){
   fade = 0;
 }
 else {
 fade = int(map(dist, 50, 100, 255, 0));
 }
 }
//send the distance info via OSC so computer 3 can access it
OscMessage myMessage = new OscMessage("/distance");
myMessage.add(fade);
oscP5.send(myMessage, myRemoteLocation); 
}