Code files for multimedia installation Threshold

The installation uses three mac computers and two Arduinos.

Arduino 1 (connected to distance sensor in hallway)
soundHallSonar.ino - reads the distance from the ultrasonic sensor in the hallway and outputs it via Serial at 115200

Arduino 2 (connected to pressure plate at main room entrance)
pressurePlatetoElWireLED.ino - when the pressure plate sensor is triggered on entry, runs the LED light pattern

Computer 1 (mac mini connected to hallway monitor)
whiteOutMirrorSensorOSC.pde - reads the sensor data from Ardunio 1 via Serial at 115200 and creates the fade effect for the hallway monitor, forwards sensor data via OSC so other computers can access it

Computer 2 (mac mini connected to Kinect and speakers)
kinectToOSC - detects blobs in main space, outputs /numBlobs and /blobs/position (blob num, loc.x, loc.y, loc.z) via OSC on port 12345
soundZonesOSC/soundZones.pd - reads in /mode data via OSC from OF program on port 54321, triggers soundscape based on mode

Computer 3 (Macbook Pro connected to projector)
visualsLayout - receives blob data from the Kinect via OSC on port 12345 and plays video based on viewer location, sends /mode data via OSC on port 54321 so Pure Data can run the right audio
