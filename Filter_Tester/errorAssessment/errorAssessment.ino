#include <LiquidCrystal.h>
#include <Wire.h>
#include<SPI.h>
#include <Adafruit_LSM9DS0.h>;
#include <Adafruit_Sensor.h>;


Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();


LiquidCrystal lcd(12,11,5,4,3,2);
void setupSensor(){
   
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

long oldTime;

void setup(){
  lcd.begin(16,2);
 
  lcd.print("    Process");
  lcd.setCursor(0,2);
  lcd.print("   Initiated");
  if (!lsm.begin()){
    while(1){
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    }
    setupSensor();
  }
  oldTime=millis();
    sensors_event_t accel,mag,gyro,temp; //Clear old reading from the board
  lsm.getEvent(&accel,&mag,&gyro,&temp);
}

float oldPoints[5]={0,0,0,0,0};

float pitch=0;

float oldnetaccel=0;
float error=0;
int count=1;

void loop(){
  sensors_event_t accel,mag,gyro,temp;
  lsm.getEvent(&accel,&mag,&gyro,&temp);
  long dt=millis()-oldTime;
  oldTime=millis();
  //Angle Calculations
  float dty=gyro.gyro.y;
  float ax=accel.acceleration.x;
  
  pitch=.5*(pitch+dty*((float)dt/1000))+.5*(asin(ax)*(180/3.14157));

  float netaccel=(ax)-sin(pitch*3.14157/180);
  
  error+=(netaccel);
  
  lcd.clear();
  lcd.print("Average Drift/ns: ");
  lcd.setCursor(0,2);
  lcd.print(error/(oldTime)*1000000);
  count++;
  //delay(0);
}
