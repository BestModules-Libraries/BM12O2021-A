/*************************************************
File:             SetEID.ino
Description:      Set the module EID to 0x03,  and then obtain the firmware version from the module,
                  You can view the data returned by the module through the serial port.
Note:               
**************************************************/
#include <BM12O2021-A.h>

#define EID 0x03
uint8_t readbuf[30] = {0};     //Save data receive from BM12O2021-A.
uint16_t version = 0;         //Vrsion number
BM12O2021_A  NMOS;            //Define a project name BM12O2021_A.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     //Init Serial for communicating with PC
  NMOS.begin(9600);       //Init Serial1 for communicating with BM12O2021-A
  delay(20);              //Delay 20ms
  if(NMOS.setEID(EID) == 0x00)      //If eid setting succeed
  {
    Serial.print("EID setting succeed,EID = ");
    Serial.print(EID); 
    Serial.println(".");
  }
  else Serial.println("EID setting failed!");
  delay(20);              //Delay 20ms
}

void loop() {
  
 
  version = NMOS.getFWVer(EID); 
  if(version <= 0x40 || version == 0x80) 
  {
    Serial.println("Error! Error code:");
    Serial.println(version,HEX);
  }
  else 
  {
    Serial.println("The version number:");
    Serial.println(version,HEX);
  } 
  delay(2000);              //Delay 2000ms
}
