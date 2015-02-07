const int buttonpin = 7;  //signal to camera is attached to digital pin 7

void setup(){
  pinMode(buttonpin, INPUT);  //set pin 7 as INPUT to ensure no initial outgoing communication.
  digitalWrite(buttonpin, LOW);  //set pin 7 as low to ground the pin on the camera.  Only works if pin 7 is set as OUTPUT!
}

void loop(){
  pinMode(buttonpin, OUTPUT);  //set pin 7 as output which grounds the camera's on pin.
  delay(500);  //has to set to less than a second to trigger the powering on sequence.
  pinMode(buttonpin, INPUT);  //set pin 7 back to INPUT to cut outgoing communication again.
  
  delay(10000);  //delays the loop for 10 seconds and keeps the camera recording for 10 seconds.
                 //we don't need this to record, just to keep from shutting off the camera right away.
}
  
  
  
