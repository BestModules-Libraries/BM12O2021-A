/*************************************************
File:             SetPWM.ino
Description:      Enable MOS2 and MOS3 on the module with Eid=3 output PWM signals,
                  and then obtain the working status of MOS from the module,
                  You can view the data returned by the module through the serial port.
Note:               
**************************************************/
#include <BM12O2021-A.h>

#define EID 0x03
BM12O2021_A  NMOS;            //Define a project name BM12O2021_A.
uint8_t nmos_status;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);   	//Init Serial for communicating with PC
	NMOS.begin(9600);     	//Init Serial1 for communicating with BM12O2021-A
//	delay(2000);            	//Delay 20ms

}

void loop() {
	NMOS.setOFF(EID,ALL);					//close all pwm channel
	NMOS.setOCP(EID,MOS1,OCP_1_0A);         //set over current value 1.0A
	NMOS.setPWM(EID,MOS2, F_25KHZ, 50);	  //set PWM of MOS2,Frequency:25KHZ  Duty:50%
	NMOS.setPWM(EID,MOS3, F_25KHZ, 50);	  //set PWM of MOS3,Frequency:25KHZ  Duty:50%
	nmos_status = NMOS.getStatus(EID);                    //get the status of NMOS
  Serial.print("NMOS state:");
  Serial.println(nmos_status,BIN);
  delay(20);

}
