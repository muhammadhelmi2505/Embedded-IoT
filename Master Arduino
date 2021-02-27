//-------------------------------i2c master-slave-------------------------------
#include <Wire.h>


//---------------ultrasonic-----------------------------------------------------
const int trigPin = 2; //initalize pin and variable for ultrasonic sensor
const int echoPin = 3;
long duration;
int lengthdoor;
int door;

void ultrasonic(){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
lengthdoor= duration*0.034/2;//getting the distance
 if (lengthdoor<10){
  door=1;
}
else if(lengthdoor>=10){
  door=0;
}
else{
  door=3;
}
}

//----------------------------------photodiode------------
const int ldrpin = A0;//initalize pin and variable for ldr
const int lightpin=8;
int ldrvalue=0;
int ldrstate=0;
int templdrstate=0;

void lightdiode(){
  ldrvalue = analogRead(lightpin);

if (ldrvalue <= 100) {
ldrstate=1;
} 
else if (ldrvalue>100){
  ldrstate=0;
}
else{
 ldrstate=0;
}

if(ldrstate != templdrstate){

  if(ldrstate==0){
  digitalWrite(lightpin,LOW);
  templdrstate=ldrstate;
  }
  else if(ldrstate==1){
    digitalWrite(lightpin,HIGH);
    templdrstate=ldrstate;
  }
  else{
    digitalWrite(lightpin,LOW);
  }
}
}

//------------------------temperature--------------------------
float Vin=5.0;     // [V]        
float Rt=10000;    // Resistor t [ohm]
float R0=10000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
// use the datasheet to get this data.
float T1=273.15;      // [K] in datasheet 0º C
float T2=373.15;      // [K] in datasheet 100° C
float RT1=35563;   // [ohms]  resistence in T1
float RT2=549;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output


int temperature=0;   // variable output
int temperaturestate=0;
int temptemperaturestate=0;
const int temperaturepin = A1;
const int fanpin=9;

void temperature_sensor()
{
Vout=Vin*((float)(analogRead(temperaturepin))/1024.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  temperature=TempK-273.15;
 
  if (temperature>=29) {
temperaturestate=1;
} 
else if (temperature<29){
  temperaturestate=0;
}
else{
 temperaturestate=0;
}

if(temperaturestate != temptemperaturestate){

  if(temperaturestate==0){
  digitalWrite(fanpin,LOW);
  temptemperaturestate=temperaturestate;
  }
  else if(temperaturestate==1){
    digitalWrite(fanpin,HIGH);
    temptemperaturestate=temperaturestate;
  }
  else{
    digitalWrite(fanpin,LOW);
  }
}
   
}

//--------------------gas----------------------------------------------------
#include <Servo.h> //initalize pin and variable for gas sensor
Servo servo1; 
int gas=0; 
int gasvalue=0;
int tempgas=0;
const int gaspin = A2;
const int servopin=10;
const int buzzerpin=11;

void gas_sensor()
{
  gasvalue= analogRead(gaspin);
  Serial.println(gasvalue);
  if (gasvalue>=200) {
      gas=1;
      
} 
else if (gasvalue<200){
  gas=0;
}
else{
 gas=0;
}
Serial.println(gas);
delay(500);
if(gas != tempgas){

  if(gas==0){
  servo1.write(0);
  analogWrite(buzzerpin,0);
  tempgas=gas;
  }
  else if(gas==1){
   servo1.write(120);
   analogWrite(buzzerpin,125);
    tempgas=gas;
  }
  else{
    servo1.write(0);
  }
}
}
//--------------------------------12c-------------------------
void send2slave(){
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(temperature);              // sends variable 
  Wire.write(door); 
  Wire.write(gas); 
  Wire.endTransmission();    // stop transmitting
}

//-----------------------------------setup---------------------------------

void setup() {

Serial.begin(9600);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

pinMode(lightpin, OUTPUT);
pinMode(ldrpin, INPUT);

pinMode(temperaturepin, INPUT);
pinMode(fanpin, OUTPUT);
 
 //parâmetros
  beta=(log(RT1/RT2))/((1/T1)-(1/T2));
  Rinf=R0*exp(-beta/T0);
  

servo1.attach(servopin);
pinMode(gaspin, INPUT);
servo1.write(0);

Wire.begin(); //initialize master
  
}

//------------------------------loop------------------------------------------
void loop() {
  
  ultrasonic();
  lightdiode();
  temperature_sensor();
  gas_sensor();
  
  send2slave();
  
 

  

}
