#include <SPI.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>

#define LSM9DS0_XM 0x1D
#define LSM9DS0_G 0x6B

//Filter Variables
#define abuffer 5 //accleration buffer size
#define gMin 2 //dps
#define gDeviation 0.015 //acceptable deviation from 1g as gravity
#define Gc 32.174 //Gravitational Constant
#define mbuffer 20//magnometer buffer size
#define headingTolerance 50 //degrees deviation between readings
#define declination -12.15//declination in area of test
#define truncateValue 1000000 //truncate values smaller than 1/this 
#define maxVelocity 10 //Maximum anticipated velocity in m/s
#define maxAccel 1.5 //Maximum g (remember gravity)
#define angleError 0 //acceptible angle error
#define gyroSettlingTime 2000000//Delay for gyro to settle microseconds

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

const byte INT1XM = 7;
const byte INT2XM = 6;  //Define the pins where these are attached
const byte DRDYG = 5;

float abias[3]={0,0,0},gbias[3]={0,0,0};

long now=0;
long lastUpdate=0;
long gyroNow=0;
long gyroLastUpdate=0;
long  count=0;
float a[3]={0,0,0},g[3]={0,0,0},m[3]={0,0,0};
float P[3]={0,0,0},V[3]={0,0,0},o[3]={0,0,0},A[3]={0,0,0};
float G[3]={0,0,0},D[3]={0,0,0},oldV[3]={0,0,0};
float dTheta[3]={0,0,0};
float gravity=1.0;
float temperature;
float heading;
boolean aFlag=false,gFlag=false,mFlag=false;

float aMag=0;

long timer; //Used to time things with in the loop
long lastTime;
void setup() {
  pinMode(INT1XM,INPUT);
  pinMode(INT2XM,INPUT);
  pinMode(DRDYG,INPUT);
  
  uint32_t status = dof.begin();
  
  dof.setAccelScale(dof.A_SCALE_2G);
  dof.setGyroScale(dof.G_SCALE_245DPS);
  dof.setMagScale(dof.M_SCALE_2GS);
  dof.setAccelODR(dof.A_ODR_200);
  dof.setAccelABW(dof.A_ABW_50);
  dof. setGyroODR(dof.G_ODR_190_BW_125);
  dof.setMagODR(dof.M_ODR_125);
  
  dof.calLSM9DS0(gbias, abias);
  
  now=micros();
  gyroNow=now;
  lastTime=millis();  
}

void loop() {
  timer=millis();
 if(digitalRead(DRDYG))readGyro(); //if Gyro Data is ready read Gyro
 if(digitalRead(INT1XM))readAccel(); //if Acceleration Data is ready read Acceleration
 if(digitalRead(INT2XM))readMag(); //if Mag is ready read heading
 if(gFlag)updateGyro(); //if enough data has been collected update gyroscope
 if(mFlag)updateMag(); //if enough data has been collected update mag
 if(aFlag && gFlag)updatePosition(); //if both acceleration and gyro are ready updatePosition
 if((timer-lastTime)>500){ //If .5 seconds has passed
  //save to SD card
  //Position is in the P vector
  //orientation is in the o vector
 }

lastTime=timer;
}

