/*
Nerve V1.5.2 Program by- Dheeraj Sharma.
from - Science & Technology Research Organization of India.
Head Office - Delhi,India.
E-mail - GreatNerve@gmail.com
More Information - https://www.youtube.com/channel/UCQp4Ybkte2p9-g3JpICjl2A
*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>//LiquidCrystal library for Print Data in LCD.
#include <AFMotor.h> // AFMotor library for motor driver shield.
#include <EEPROM.h> // EEPROM library for cammand storage.
#include <Servo.h>  //add Servo Motor library            
#include <NewPing.h>//add Ultrasonic sensor library
#define TRIG_PIN 56 // Pin 56  blue wire on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN  57// Pin 57 green wire on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 400 // sets maximum useable sensor measuring distance to 300cm
#define COLL_DIST 50 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.
LiquidCrystal_I2C lcd(0x3f,16,2);
AF_DCMotor leftmotor(3);// Set left motor in M4 terminal.
AF_DCMotor rightmotor(4);// Set right motor in M3 terminal.
Servo myservo;  // create servo object to control a servo 
int leftDistance, rightDistance; //distances on either side
const int Alchol_Sensor = 55;//Connect Alchol Sensor in pin 55.
const int PIR = 58;//Connect PIR Sensor in pin 58.
const int LDR = 59;//Connect LDR Sensor in pin 59.
const int Buzzer = 61;//Connect Buzzer System in pin 61.
const int LIGHT = 62;//Connect LIGHT System in pin 62.
const int Full_Run_Speed = 150;//Maximum Full run speed is 150 Approx 60% duty cycle.
const int Full_Turn_Speed = 200;//Maximum Full Turn speed is 200 Approx 80% duty cycle Because if motor turn it need more power.
byte Read;// Read byte from EEPROM.
int curDist = 0;
int addr = 0;
int address = 0;
/*------------------------------------------------------------        
                         VOID SETUP
-------------------------------–--––-------------------------*/
void setup() {   
   myservo.attach(9);  // attaches the servo on pin 9.
   Serial1.begin(9600); // Set Bluetooth Module buand rate 9600.
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();// Clean All thing from display.
  lcd.print("Installing...");//LCD print Installing... 
  lcd.setCursor(1, 1);
  lcd.print("Dheeraj Sharma");//LCD print Dheeraj Sharma.
 pinMode(Alchol_Sensor,INPUT);// Declare Alchol_Sensor is INPUT.
 pinMode(LDR,INPUT_PULLUP);// Declare LDR is INPUT PULLUP Because we not need Pulldown Resistor.
 pinMode(PIR,INPUT);// Declare PIR is INPUT.
 pinMode(LIGHT,OUTPUT);// Declare LIGHT is OUTPUT.
 pinMode(Buzzer,OUTPUT);// Declare Buzzer is OUTPUT.
      for (int i = 0 ; i < EEPROM.length() ; i++) {
 // Setup every restart all Previous Data Erased.
    EEPROM.write(i, 0);
  }
}
/*------------------------------------------------------------        
                         VOID LOOP
-------------------------------–--––-------------------------*/
void loop(){
 if(digitalRead(PIR)==LOW){
 // If PIR sensor not Detect Motion So Alchol Detect is Activate.
     digitalWrite(Buzzer,LOW);
 if(digitalRead(Alchol_Sensor)==LOW){
 // If Alchol sensor not Detect Alchol So Mode selection is Activate.     
    if(analogRead(A0)<100){
 //If Potentiometer Value is less than 100 so Bluetooth Write is Activate.
     Bluetooth();   
   }else if(analogRead(A0)>100&analogRead(A0)<500){
    //If Potentiometer Value is Greater than 100 and less so Bluetooth Write is Activate.
      Bluetooth_Read_Mode();          
    }else if(analogRead(A0)>500){
 //If Potentiometer Value is Greater than 500 so Auto Drive Mode is Activate.
    Auto_Drive_Mode();                   
        } else{
  // else nothin do.          
        }
    }else{
  // If Alchol sensor  Detect Alchol So Auto Drive Mode is Activate.        
  Auto_Drive_Mode();     
    }}else{
  // If PIR sensor  Detect Motion So Vehical Stop Move.
   digitalWrite(Buzzer,HIGH);
  lcd.clear();
  lcd.print("Humans Detected");
  lcd.setCursor(1, 1);
  lcd.print("Dheeraj Sharma");
   stop(); 
   delay(100);
    }
if(digitalRead(LDR)==HIGH){
     // If LDR sensor Detect Dark So LIGHT is ON.
    digitalWrite(LIGHT,HIGH);
    }else{
    // If LDR sensor Detect LIGT So LIGHT is OFF. 
        digitalWrite(LIGHT,LOW);
    }
}
/*------------------------------------------------------------        
                           Distance
-------------------------------–--––-------------------------*/
int Distance() { // read the ultrasonic sensor distance
  delay(70);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
   if(cm==0){// If no obsticle in forward sensor so distance is 400.
     cm=400;
}
Serial1.println(cm);
   return cm;
}
/*------------------------------------------------------------        
                      Auto Drive Mode
-------------------------------–--––-------------------------*/
void Auto_Drive_Mode() {
  lcd.clear();
  lcd.print("Auto Drive Mode");
  lcd.setCursor(1, 1);
  lcd.print("Distance ");
  lcd.print(Distance());//LCD print Distance.
  myservo.write(90);  // move eyes forward
    delay(50);
  curDist = Distance(); 
  if ( curDist < COLL_DIST) {changePath();  // If forward is blocked change direction
  } else{
  forward();} // move forward
  delay(50); 
  lcd.clear();
  lcd.print("Auto Drive Mode");
  lcd.setCursor(1, 1);
  lcd.print("Distance ");
  lcd.print(Distance());//LCD print Distance.
}
/*------------------------------------------------------------        
                          CHANGE PATH
-------------------------------–--––-------------------------*/
void changePath() {
    backward();
    delay(500);
    stop();   // stop forward movement
    for(int i=90;i>=0;i-=10){// Move servo slowly Because It Create Harvey load in system.
    myservo.write(i);  // check distance to the right
    }
    delay(500);
    rightDistance = Distance(); //set right distance.
       delay(500);
   for(int i=0;i<=180;i+=10){// Move servo slowly Because It Create Harvey load in system.
    myservo.write(i);  // check distace to the left
    }
    delay(700);
    leftDistance = Distance(); //set left distance.
    delay(500);
    for(int i=180;i>=90;i-=10){// Move servo slowly Because It Create Harvey load in system.
    myservo.write(i); //return to center
       }
        delay(400);
    compareDistance();}
/*------------------------------------------------------------        
                       COMPARE DISTANCE
-------------------------------–--––-------------------------*/
void compareDistance(){
  if (leftDistance>rightDistance) //If left side distance is greater is than right side so Vehical move Leftward.
  {
    left();
    delay(1000);
    forward();
  }
  else if (rightDistance>leftDistance) //If right side distance is greater is than left side so Vehical move Rightward.
  {
    right();
    delay(1000);
    forward();
  }else{
 // else Repeat the process.
  Auto_Drive_Mode();
}} 
/*------------------------------------------------------------        
                      BLUETOOTH Mode
-------------------------------–--––-------------------------*/           
void Bluetooth() {   
  lcd.clear();
  lcd.print("Bluetooth Write");
  lcd.setCursor(1, 1);
  lcd.print("Dheeraj Sharma");
   myservo.write(90);// move eyes forward
  if(sonar.ping_cm()< 30&sonar.ping_cm()>0){
     backward();
     delay(500);
     stop();  
    }else{
  if (Serial1.available()>0) {
     int Cammand = Serial1.read();
        EEPROM.write(addr, Cammand);//every cammand are stored.
        switch (Cammand)
      {
        case 'a':
        {
          forward();//If Receive Cammand is "a" so moter run forward.
          break;
        }
        case 'b':
        {
          backward();//If Receive Cammand is "b" so moter run backward.
         break;
        }
       case 'c':
      {
         stop();//If Receive Cammand is "c" so moter stop.
          break;
        }
         case 'd':
        {
         right();//If Receive Cammand is "d" so moter run right.
         break;
        }
       case 'e':
      {
         left();//If Receive Cammand is "e" so moter run left.
      break;
        }
        
      }
      addr = addr + 1;
  if (addr == EEPROM.length()) {
    addr = 0;
  }
    }
     delay(10);//Every 100 Millisecond every cammand stored.
   } myservo.write(90);
 } 
/*------------------------------------------------------------        
                    BLUETOOTH READ Mode
-------------------------------–--––-------------------------*/
void Bluetooth_Read_Mode() {
  lcd.clear();
  lcd.print("Bluetooth Read");
  lcd.setCursor(1, 1);
  lcd.print("Dheeraj Sharma");
    myservo.write(90); // move eyes forward.
      if(sonar.ping_cm()< 30&sonar.ping_cm()>0){
      backward();
      delay(500);
      stop();  
    }else{
    Read = EEPROM.read(address);
    if(Read > 0){//This can read cammand if cammand is greater than 0.
          switch (Read)
      {
        case 'a':
        {
          forward();//If Receive Cammand is "a" so moter run lforward.
        break;
        }
        case 'b':
        {
         backward();//If Receive Cammand is "b" so moter run backward.
         break;
        }
       case 'c':
      {
         stop();//If Receive Cammand is "c" so moter stop.
          break;
        }
         case 'd':
        {
         right();//If Receive Cammand is "d" so moter run right.
         break;
        }
       case 'e':
      {
         left();//If Receive Cammand is "e" so moter run left.
          break;
        }
       }   
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }  
  delay(300);}}
 myservo.write(90); 
}
/*------------------------------------------------------------        
                          MOVE WARD
-------------------------------–-----------------------------*/
void forward() {// both motor run forward.
    leftmotor.run(FORWARD);
    rightmotor.run(FORWARD);
    int Make=Full_Run_Speed-30;
    int Make1=Full_Run_Speed+30;
    rightmotor.setSpeed(Make);  
    leftmotor.setSpeed(Make1);
}

void backward() {// both motor run backward.
    leftmotor.run(BACKWARD);
    rightmotor.run(BACKWARD);
    leftmotor.setSpeed(Full_Run_Speed); 
    rightmotor.setSpeed(Full_Run_Speed); 
}

void left() {//left motor stop & Right motor run backward.
    leftmotor.run(RELEASE);
    rightmotor.run(FORWARD);
    rightmotor.setSpeed(Full_Turn_Speed);
}

void right() {//Right motor stop & Left motor run forward.
    rightmotor.run(RELEASE);
    leftmotor.run(FORWARD);
    leftmotor.setSpeed(Full_Turn_Speed); 
}

void stop() {//both motor stop.
    leftmotor.run(RELEASE);
    rightmotor.run(RELEASE);
}