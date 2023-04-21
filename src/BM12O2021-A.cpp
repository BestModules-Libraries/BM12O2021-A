/*************************************************
File:           BM12O2021-A.cpp
Author:         BESTMODULES
Description:    The members in the class BM12O2021_A are defined in this file.
History：
V1.0.1  -- initial version;2023-01-11;Arduino IDE : v1.8.13
**************************************************/
#include <BM12O2021-A.h>


/*************************************************
Description:  Initializes the data member of class BM12O2021_A
Parameters:        theSerial  : pointer of class Serial,The default is Serial1.         
Return:         
Others:         
*************************************************/
BM12O2021_A::BM12O2021_A(HardwareSerial* theSerial ) 
{
    this->theSerial  = theSerial ;
    this->mid = 0x1f;
    this->eid = 0;
}

/*************************************************
Description:	Set serial port parameters and enable serial port.
Parameters:        baud : Baudrate.          
Return:             
Others:            
*************************************************/
void BM12O2021_A::begin(const uint16_t baud)
{
    this->theSerial ->begin(baud, SERIAL_9N1);
    this->theSerial ->SetRxStatus(DISABLE);
    this->theSerial ->SetTxStatus(ENABLE);
}

/*************************************************
Description:	The module is required to reset.
Parameters:   eid : Value of eid(EID=0~15).    
              radio : Choose whether to broadcast.           
Return:       0:SUCCESS 1:FAIL      
Others:            
*************************************************/
uint8_t BM12O2021_A::reset(uint8_t eid,bool radio) //Broadcast instructions are optional
{
    uint8_t pbuf[4]={0};
    this->eid = eid;
    this->sandCMD(CMD_RESET, radio);
    if(radio)
    {
      return SUCCESS;
    }
    else
    {
      if(readBtyes(pbuf,4) == SUCCESS)
      {
        if(pbuf[2] == 0x00)
        {
            return SUCCESS;
        }
        else 
        {
            return FAIL; 
        }
      } 
      else
      {
        return FAIL;  
      }  
    }   
}

/*************************************************
Description:	The module is required to standby mode.
Parameters:           
Return:             
Others:            
*************************************************/
void BM12O2021_A::standBy(void)  //Broadcast directives
{
    this->sandCMD(CMD_STANDBY, 1);
}

/*************************************************
Description:	Get version information from module.
Parameters:     eid : Value of eid(EID=0~15).
Return:         If the acceptance is correct, the version number will be returned; 
                otherwise, return 0.
Others:            
*************************************************/
uint16_t BM12O2021_A::getFWVer(uint8_t eid) //Non-broadcast directives
{
    
    this->eid = eid;
    this->sandCMD(CMD_GETFWVER, 0);
    uint8_t pbuf[6]={0};
    uint16_t version = 0;
    if(readBtyes(pbuf,6)==SUCCESS)
    { 
      if(pbuf[2] == 0x00)
      {
          version = (pbuf[4] << 8) | pbuf[3];
          return version;
      }
      else 
      {
        return 0; 
      }
    }
    else 
    {
      return 0; 
    }
}

/*************************************************
Description:	Set the Eid of the module.
Parameters:     eid:Value of eid(EID=0~15).
Return: 
                0:success
                1:fail 
Others:            
*************************************************/
uint8_t BM12O2021_A::setEID(uint8_t eid) //Broadcast directives
{
    uint8_t pbuf[4]={0};
    this->eid = eid;
    this->sandCMD(CMD_SETEID+(eid&0x0f), 1);
    if(readBtyes(pbuf,4) == SUCCESS)
    {
      if(pbuf[2] == 0x00)
      {
          return SUCCESS;
      }
      else 
      {
          return FAIL; 
      }
    } 
    else
    {
      return FAIL;  
    }  
}

/*************************************************
Description:	Set the working frequency and duty of PWM output of the module.
Parameters:   eid : Value of eid.      
              mosn : MOS selection(n=1~4).
              Freq : Frequency of PWM.
              Duty : Duty of PWM.
              syncAction : Delay selection.
Return:                       
                0:success
                1:fail 
Others:
*************************************************/
uint8_t BM12O2021_A::setPWM(uint8_t eid,uint8_t mosn,uint8_t freq,uint8_t duty,bool syncAction)
{
    uint8_t buf[4]={0};
    uint8_t pbuf[4]={0};
    this->eid = eid;
    if(syncAction) buf[0] += CMD_DELAY;
    else buf[0] = CMD_SETPWM;
    buf[1] = mosn;
    buf[2] = freq;
    buf[3] = duty;
  
    sandCMD_Param(buf,4);

    if(readBtyes(pbuf,4)==SUCCESS)
     { 
      if(pbuf[2] == 0x00)
      {
          this->mos_status |= (1<<mosn);
          return SUCCESS;
      }
      else 
      {
          return FAIL; 
      }
     }
     else 
     {
        return FAIL; 
     }
}

/*************************************************
Description:	Set ON status of NMOSn.
Parameters:   eid : Value of eid.      
              mosn : MOS selection(n=1~4).
              syncAction : Delay selection.
Return:     
                0:success
                1:fail 
Others:            
*************************************************/
uint8_t BM12O2021_A::setON(uint8_t eid,uint8_t mosn,bool syncAction)
{
  uint8_t buf[2] = {0};
  uint8_t pbuf[4]= {0};
  
  this->eid = eid;
  if(syncAction) 
  {
    buf[0] += CMD_DELAY;
  }
  else 
  {
    buf[0] = CMD_SETONOFF;
  }

  if(mosn != ALL)   
  {
    buf[1] = this->mos_status | (1<<(mosn)); 
  }
  else  
  {
    buf[1] = ALL;
  }

  this->mos_status = buf[1];
  
  sandCMD_Param(buf,2);

  if(readBtyes(pbuf,4)==SUCCESS)
   {
    if(pbuf[2] == 0x00)
    {
        return SUCCESS;
    }
    else 
    {
        return FAIL; 
    } 
   } 
   else
   {
     return FAIL; 
   }
}

/*************************************************
Description:	Set OFF status of NMOSn.
Parameters:   eid : Value of eid.      
              mosn : MOS selection(n=1~4).
              syncAction : Delay selection.
Return:       
              0:success
              1:fail
Others:            
*************************************************/
uint8_t BM12O2021_A::setOFF(uint8_t eid,uint8_t mosn,bool syncAction)
{
  uint8_t buf[2] = {0};
  uint8_t pbuf[4]= {0};
  
  this->eid = eid;
  if(syncAction) 
  {
    buf[0] += CMD_DELAY;
  }
  else 
  {
    buf[0] = CMD_SETONOFF;
  }

  if(mosn != ALL)   
  {
    buf[1] = this->mos_status & (~(1<<(mosn))); 
  }
  else  
  {
    buf[1] = ~(ALL);
  }
  this->mos_status = buf[1];
  
  sandCMD_Param(buf,2);

  if(readBtyes(pbuf,4)==SUCCESS)
   { 
      if(pbuf[2] == 0x00)
      {
          return SUCCESS;
      }
      else 
      {
          return FAIL; 
      }
   } 
   else 
   {
     return FAIL; 
   }
}

/*************************************************
Description:	Read NMOS working status.
Parameters:     eid : Value of eid.           
Return:         pbuf[3]：NMOS status
		0：false
Others:            
*************************************************/
uint8_t BM12O2021_A::getStatus(uint8_t eid)
{
  uint8_t pbuf[5]={0};
  this->sandCMD(CMD_READSTATUS, 0);   
  this->eid = eid;
   if(readBtyes(pbuf,5)==SUCCESS)
    {
      if(pbuf[2] == 0x00) 
      {
        this->mos_status = pbuf[3] & 0x0f;
        return pbuf[3];
      }
      else 
      {
          return 0; 
      }
    } 
   else 
   {
     return 0; 
   }  
}

/*************************************************
Description:	Set overcurrent protection value of the module .
Parameters:   eid : Value of eid.     
              mosn : NMOS selection(n=1~4).
              Value : The value of Overcurrent protection selection.
              syncAction : Delay selection.
Return:      
              0:success
              1:fail
Others:            
*************************************************/
uint8_t BM12O2021_A::setOCP(uint8_t eid,uint8_t mosn,uint8_t value,bool syncAction)
{
  uint8_t buf[3]= {0};
  uint8_t pbuf[4] = {0};

  this->eid = eid;
  if(syncAction) buf[0] += CMD_DELAY;
  else buf[0] = CMD_SETOCP;
  buf[1] = mosn;
  buf[2] = value;
  sandCMD_Param(buf,3); 
   
  if(readBtyes(pbuf,4)==SUCCESS)
    {
      if(pbuf[2] == 0x00)
      {
          return SUCCESS;
      }
      else 
      {
          return FAIL; 
      }
    }
   else 
   {
     return FAIL; 
   }    
}

/*************************************************
Description:  writeBtyes.
Parameters:   writeData[] : Data to be written.
              len : Data length.           
Return:             
Others:            
*************************************************/
void BM12O2021_A::writeBtyes(uint16_t writeData[], uint8_t len)
{
  this->theSerial ->SetTxStatus(ENABLE);
  this->theSerial ->SetRxStatus(DISABLE);

  for(uint8_t i = 0; i<len ;i++)
  {
     this->theSerial ->write(writeData[i]);
  }
   this->theSerial ->flush();
  }

/*************************************************
Description:  readBtyes.
Parameters:   rxBuf[] : Store data.
              rlen：Read data length.
              waitTime:Set timeout.          
Return:         
              0:success
              1:fail 
              2: Timeout error  
Others:            
*************************************************/
uint8_t BM12O2021_A::readBtyes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
{  
   uint8_t i ,delayCnt =0;
   uint8_t check = 0;

   this->theSerial ->SetTxStatus(DISABLE);
   this->theSerial ->SetRxStatus(ENABLE);
  
    for (i = 0; i < rlen;i++)
    {
      delayCnt = 0;
      while (theSerial ->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return timeout_Error; // Timeout error
        }
      }
     rBuf[i] =this->theSerial ->read(); 
    }

    /* checkSum  */
  for(i = 0; i < rlen - 1; i++)
    {
        check += rBuf[i];
    }
  if((check) == (uint8_t)(~rBuf[rlen-1]))
   {
     if(((rBuf[0] & 0xff) == mid)&&((rBuf[1] & 0x0f) == eid))
      { 
        return SUCCESS;
      }
      else
      {
         return FAIL; 
      }
    }
    else
    {
        return FAIL;
    }
  }
/*************************************************
Description:  send command.
Parameters:   cmd : Command sent by master.
              radio : Choose whether to broadcast.           
Return:             
Others:            
*************************************************/
void BM12O2021_A::sandCMD(uint8_t cmd, bool radio)
{
    uint16_t _mid;
    uint8_t _Tlen_eid, _instr, _check_sum;
    uint16_t senddata[4];
    if(radio) 
    {
        _mid = 0x100;
        _Tlen_eid = sandCMD_L<<4;
    }
    else 
    {
        _mid = 0x100 + this->mid;
        _Tlen_eid = (sandCMD_L<<4) + (this->eid & 0x0f);
    }
    _instr = cmd;
    _check_sum = ~(_mid + _Tlen_eid + _instr);
    
    senddata[0] = _mid;
    senddata[1] = _Tlen_eid;
    senddata[2] = _instr;
    senddata[3] = _check_sum;
    writeBtyes(senddata,4);
}

/*************************************************
Description:  send command and param.
Parameters:   buf : An array that stores commands and parameters.
              len : Length of buf,Unit: byte.          
Return:             
Others:            
*************************************************/
void BM12O2021_A::sandCMD_Param(uint8_t buf[], uint8_t len)
{
    uint16_t _mid;
    uint8_t y = 0, _Tlen_eid, _TLen1, _check_sum;
    uint16_t tmp[255];//tmp[len+4];
    _mid = 0x100 + this->mid;
    if(len>14)
    {
        _Tlen_eid = (this->eid & 0x0f);
        _TLen1 = len + 1;
    }
    else
    {
        _Tlen_eid = ((len+1)<<4) + (this->eid & 0x0f);
        _TLen1 = 0;
    }
    _check_sum = _mid + _Tlen_eid + _TLen1;
    
    tmp[y] = _mid;
    y++;
    tmp[y] = _Tlen_eid;
    y++;
    if(_TLen1) 
    { 
      tmp[y] = _TLen1;
       y++;
    }
    for(uint8_t i = 0; i < len; i++)
    {
        tmp[y] = buf[i];
        _check_sum += buf[i];
        y++;
    }
    tmp[y] = (uint8_t)~(_check_sum);
    writeBtyes(tmp,y+1);

}
