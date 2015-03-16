#include <SPI.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>
#include <SD.h>
#include <SabertoothSimplified.h>
#include <Servo.h>

//Set data line for SD Card
//const int chipSelect = 10;
//Establish MotorShield Mode
SabertoothSimplified ST;
const char* saveFile="03141501.txt"; //file name for this run

//Arduino Hookup Pins-------------------------
//PWM: 3,5,6,9,10,11
#define buoyancyOutTrigger 1 //Trip when buoyancy plunger is all the way out 
#define buoyancyInTrigger 9//Trip when buoyancy plunger is all the way in
#define servoPin1 5 //Signal Pin for Servo 1
#define servoPin2 6 //Signal Pin for Servo 2
#define servoPin3 8 //Signal Pin for Servo 3
#define INT1XM 3 //Acceleration Data Ready Pin
#define INT2XM 4 //Magnomonator Data Ready Pin
#define DRDYG 2   //Gyroscope Data Ready Pin
#define camerapin 7 //Set camera on/off switch to pin 7
#define chipSelect 10 //#define makes the compiler handle the variable so the arduino dosen't have to make space for it!
//SPI pins are taken for SD card
//DI  -> 11 (MOSI)
//DO  -> 12 (MISO)    For the
//CS  -> 10 (SS)      Arduino Uno
//CLK -> 13 (SCK)
//I2C Pins Are used for the IMU, LSM9DSO
//SDA -> A4 (SDA)
//SCL -> A5 (SCL)
//TTL Serial used for SaberTooth
//
//---------------------------------


#define LSM9DS0_XM 0x1D
#define LSM9DS0_G 0x6B

//Filter Variables
#define abuffer 5 //accleration buffer size
#define gMin 2 //dps
#define gDeviation 0.035 //acceptable deviation from 1g as gravity
#define Gc 32.174 //Gravitational Constant
#define mbuffer 10//magnometer buffer size
//#define headingTolerance 100 //degrees deviation between readings
#define declination -12.15//declination in area of test
#define truncateValue 1000000 //truncate values smaller than 1/this 
#define maxVelocity 10 //Maximum anticipated velocity in m/s
#define maxAccel 5 //Maximum g (remember gravity)
//#define angleError 0 //acceptible angle error
//#define gyroSettlingTime 2000000//Delay for gyro to settle microseconds

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

//Name servos
Servo myservo1;
Servo myservo2;
Servo myservo3;

//Set initial position to zero degrees
int pos = 0;

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
//float temperature;
float heading;
boolean aFlag=false,gFlag=false,mFlag=false;

float aMag=0;

long timer; //Used to time things with in the loop
long lastTime;

void setup() {
  //Establish communication to motorshield
  SabertoothTXPinSerial.begin(9600);
  //Serial.begin(115200);//for debuging
  //Switch camera on
  pinMode(camerapin, INPUT);
  digitalWrite(camerapin, LOW);
  pinMode(camerapin, OUTPUT);
  delay(500);
  pinMode(camerapin, INPUT);
  
  //assign each servo a communication pin
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  
  pinMode(10,OUTPUT); //define the chipselect for SD Card
  pinMode(INT1XM,INPUT);
  pinMode(INT2XM,INPUT);
  pinMode(DRDYG,INPUT);
  
  SD.begin(chipSelect);
  
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
 //if(gFlag)updateGyro(); //if enough data has been collected update gyroscope
 if(mFlag)updateMag(); //if enough data has been collected update mag
 if(aFlag && gFlag)updatePosition(); //if both acceleration and gyro are ready updatePosition
 
 doISR();//Do ISR
 
 if((timer-lastTime)>500){ //If .5 seconds have passed
  //save to SD card
  //Position is in the P vector
  //orientation is in the o vector
  File dataFile = SD.open(saveFile, FILE_WRITE);
  
  if (dataFile){
    int i;
    for (i = 0; i < 3; i = i + 1){ 
    dataFile.print(P[i]);
    dataFile.print(",");
    //Serial.print(P[i]);
    //Serial.print(",");
    }
    for (i = 0; i < 3; i = i + 1){
    dataFile.print(o[i]);
    (i!=2)?dataFile.print(','):dataFile.print('\n'); //shorthand if else format
    //Serial.print(o[i]);
    //(i!=2)?Serial.print(','):Serial.print('\n');
    //Serial.print(",");
    }
   // dataFile.print('\n');//newline character to let us know that the line is over
    //Serial.print('\n');
    dataFile.close();
   }

lastTime=timer;
  }
}
