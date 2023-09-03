#include <SoftwareSerial.h>
#define RX_of_Bluetooth_module 10
#define TX_of_Bluetooth_module 11
SoftwareSerial HM10(RX_of_Bluetooth_module, TX_of_Bluetooth_module); 
char New;
char Old;
void setup(){
    HM10.begin(9600);
Serial.begin(9600);
HM10.println("AT+INQ");
if (HM10.available()) {
 Serial.write(HM10.read());
  }
delay(5000);
HM10.println("AT+CONN1");
Serial.println("o");
}
void loop(){IR_Send();
}
void IR_Send(){
  int X = analogRead(A0);
  int Y = analogRead(A1);
  if(X>800&Y>150&Y<800){
 New = 'a';delay(300);
    } else if(X<150&Y>150&X<800){
  New = 'b';delay(300);
    } else if(Y>800&X>150&X<800){
  New = 'd';delay(300);
    } else if(Y<150&X>150&X<800){
  New = 'e'; delay(300);
    } else {
  New = 'c';
}
 if(Old != New){HM10.println(New);}
 Old = New;
 delay(30);
}
