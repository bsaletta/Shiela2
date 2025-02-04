// Sweep Sample
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <SabertoothSimplified.h>

SabertoothSimplified ST; // We'll name the Sabertooth object ST.
                         // For how to configure the Sabertooth, see the DIP Switch Wizard for
                         //   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                         // Be sure to select Simplified Serial Mode for use with this library.
                         // This sample uses a baud rate of 9600.
                         //
                         // Connections to make:
                         //   Arduino TX->1  ->  Sabertooth S1
                         //   Arduino GND    ->  Sabertooth 0V
                         //   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
                         //
                         // If you want to use a pin other than TX->1, see the SoftwareSerial example.

const int WaterPin = 0;
int water_level;

void setup()
{
  SabertoothTXPinSerial.begin(9600); // This is the baud rate you chose with the DIP switches.
  pinMode(WaterPin, INPUT);
}

void loop()
{
  water_level = analogRead(WaterPin);
  Serial.print(water_level);
  delay(1000);
  
  if (water_level == 1023){
    
    while (analogRead(WaterPin) == 1023){
      int power;
      
      // Ramp motor 1 and motor 2 from -127 to 127 (full reverse to full forward),
      // waiting 20 ms (1/50th of a second) per value.
      for (power = -127; power <= 127; power ++)
      {
        ST.motor(1, power);
        //ST.motor(2, power);
        delay(20);
      }
  
      // Now go back the way we came.
      for (power = 127; power >= -127; power --)
      {
        ST.motor(1, power);
        //ST.motor(2, power);
        delay(20);
      }
    }
  }
  else
    {
      ST.motor(1,0);
      delay(20);
    }
}
