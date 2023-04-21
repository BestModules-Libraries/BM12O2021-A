/*************************************************
File:           BM12O2021-A.h
Author:         BESTMODULES
Description:    The members in the class BM12O2021_A are defined in this file.
History：
V1.0.1  -- initial version;2023-01-11;Arduino IDE : v1.8.13
**************************************************/


#ifndef BM12O2021_A_h
#define BM12O2021_A_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define sandCMD_L 2
#define   SUCCESS           0
#define   FAIL              1
#define   timeout_Error     2
typedef enum 
{
    CMD_RESET = 0x00,
    CMD_STANDBY = 0x01,
    CMD_SYNCACTION,
    CMD_WAKEUP,
    CMD_GETFWVER = 0x03,
    CMD_DELAY = 0x20,
    CMD_SETEID = 0x80,
    CMD_SETONOFF = 0x08,
    CMD_SETPWM = 0x09,
    CMD_READSTATUS = 0x0A,
    CMD_SETOCP = 0x0B
}Command;

typedef enum
{
    OCP_0_25A = 0x00,
    OCP_0_5A = 0x01,
    OCP_0_75A = 0x02,
    OCP_1_0A = 0x03,
    OCP_1_25A = 0x04,
    OCP_1_5A = 0x05,
    OCP_1_8A = 0x06,
    OCP_2_0A = 0x07
}OCP;				//Select value of overcurrent protection

typedef enum
{
    MOS1 = 0x00,
    MOS2 = 0x01,
    MOS3 = 0x02,
    MOS4 = 0x03,
    ALL = 0x0f
}MOSn;				//Select which NMOS to set

//typedef enum
//{
//    MOS1_ON = 0x01,
//    MOS2_ON = 0x02,
//    MOS3_ON = 0x04,
//    MOS4_ON = 0x08,
//    ALL_ON = 0x0f
//}MOSn_ON;			//Select which NMOS to turn on

typedef enum
{
    F_5KHZ = 0x00,
    F_10KHZ = 0x01,
    F_20KHZ = 0x02,
    F_25KHZ = 0x03,
    F_50KHZ = 0x04
}Frequency;			//Select frequency of PWM

class BM12O2021_A
{

  public:
    BM12O2021_A(HardwareSerial* theSerial  = &Serial1);
    void begin(const uint16_t baud = 9600);
    
    uint8_t reset(uint8_t eid,bool radio = false);
    void standBy();
    uint16_t getFWVer(uint8_t eid);                        //Get version information form module  
    uint8_t setEID(uint8_t eid);          //Set EID of signle module 
    uint8_t setPWM(uint8_t eid,uint8_t mosn,uint8_t freq,uint8_t duty,bool syncAction = false);  //Set PWM's param
    uint8_t setON(uint8_t eid,uint8_t mosn,bool syncAction = false);          //Turn on MOS 
    uint8_t setOFF(uint8_t eid,uint8_t mosn,bool syncAction = false);                  //Turn off MOS 
    uint8_t getStatus(uint8_t eid);                                                 //Get MOS working status
    uint8_t setOCP(uint8_t eid,uint8_t mosn,uint8_t value,bool syncAction = false);             //Set current of overcurrent protection  	

  private:
    HardwareSerial* theSerial ;
    uint8_t mid;
    uint8_t eid;
    uint8_t mos_status;

    void sandCMD(uint8_t cmd, bool radio);
    void sandCMD_Param(uint8_t buf[], uint8_t len);
    void writeBtyes(uint16_t writeData[], uint8_t len);
   uint8_t readBtyes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime = 5);				
};

#endif
