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
  Serial.begin(9600);
  lcd.begin(16,2);
 
  lcd.print("got one");
  if (!lsm.begin()){
    while(1){
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    }
    setupSensor();
  }
  oldTime=millis();
}

float oldPoints[5]={0,0,0,0,0};

void loop(){
  sensors_event_t accel,mag,gyro,temp;
  lsm.getEvent(&accel,&mag,&gyro,&temp);
  long dt=millis()-oldTime;
  oldTime=millis();
  float ax=accel.acceleration.x;
  //linear approximation
  float points[5];
  points[0]=ax;
  for(int i=1;i<5;i++){
    points[i]=oldPoints[i-1];
  }
  for(int i=0;i<5;i++){
     oldPoints[i]=points[i]; 
  }
  float xsum;
  xsum=points[0]+points[2]+points[3]+points[4];
  lcd.clear();
  lcd.print(xsum);
  Serial.print(ax);
  Serial.print(',');
  Serial.println(xsum/5);
  
  if(Serial.available()>0){
    delay(100);
    lcd.clear();
    while(Serial.available()>0){
      lcd.write(Serial.read());
    }
  }
  //delay(100);
}
