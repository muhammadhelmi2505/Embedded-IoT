#include <Wire.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7)

#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(10, 11);   //Pin 10 and 11 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "WIFI@MMU";       // WiFi SSID
String myPWD = "wifi@mmu"; // WiFi Password
String myAPI = "982F0JTJPKD3W2HO";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String field1= "field1"; 
String field2= "field2"; 
String field3= "field3"; 

//data variable
int temperature=0;
int door=0;
int gas=0;

//display lcd variable
String strdoor;
String strgas;

String stropen = "Open ";
String strclose = "Close";

String strok = "OK   ";
String stralarm = "ALARM";

String strerr = "ERR  ";

//display lcd function
void displaylcd(){
lcd.setCursor(0,1);
lcd.print(temperature);
lcd.setCursor(2,1);
lcd.print((char)223);
lcd.setCursor(3,1);
lcd.print("C");
lcd.setCursor(5,1);
lcd.print(strdoor);
lcd.setCursor(11,1);
lcd.print(strgas);
}

//display in bluetooth function
void displaybt(){
  Serial.println(temperature); //
    Serial.println("+");  
    Serial.println(door);
  Serial.println("+");
  Serial.println(gas);
  Serial.println("+");
    delay(500); 
}

//receive data from master function
void receiveEvent (int howMany)                    //This Function is called when Slave receives value from master
{
   temperature = Wire.read();  
   door = Wire.read();
   gas = Wire.read(); //Used to read value received from master and store in variable SlaveReceived
}

//check door status function
void checkdoor(){
 if(door==0){
  strdoor=strclose;
 }
 else if(door==1){
  strdoor=stropen;
 }
else{ strdoor=strerr;}
}

//check gas status function
void checkgas(){
 if(gas==0){
  strgas=strok;
 }
 else if(gas==1){
  strgas=stralarm;
 }
else{ strgas=strerr;}
}


void setup() {

Wire.begin(9); //initialize slave
Wire.onReceive(receiveEvent);
  
lcd.begin(16, 2);
lcd.setCursor(0,0);
lcd.print("TEMP DOOR  GAS");

Serial.begin(9600);
espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
 
}

//sending data to cloud function
void sendwifi(){
  String sendData = "GET /update?api_key="+ myAPI +"&"+ field1 +"="+String(temperature)+"&"+ field2 +"="+String(door)+"&"+ field3 +"="+String(gas);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
   espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(100);
}

 String espData(String command, const int timeout, boolean debug)
{
 
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  
  return response;
}

void loop() {
checkdoor();
checkgas();
displaylcd();
displaybt();
sendwifi();

}
