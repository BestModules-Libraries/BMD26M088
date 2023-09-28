/*************************************************
File:             breath.ino
Description:      Breath.
Note:               
**************************************************/
#include <BMD26M088.h>
#define ADDRESS BMD26M088_I2C_ADDRESS_SCL    

BMD26M088 myBMD26M088(&Wire2);//BMCOM2 is use I2C2

void setup() 
{                                  
 myBMD26M088.begin(ADDRESS);  //I2C init            
 myBMD26M088.writeAllRGB(ADDRESS, 0xff,0,0);  //set red  
 myBMD26M088.setBrightness(ADDRESS,0xff);//set global brightness,max
 myBMD26M088.setCurrent(ADDRESS,BMD26M088_CCR_6MA);//set drive current 6mA            
}
uint8_t fall_rise_flag = 0;
uint8_t breath_time = 20;
uint8_t breath_count = 0;

void loop()
{  
  if(fall_rise_flag==0)
  {
    if(breath_count<255){
      breath_count++;
    }
    else{
      fall_rise_flag=1;
    }
     myBMD26M088.setBrightness(ADDRESS, breath_count);  
  }
  else
  { 
    if(breath_count>0){
      breath_count--;
    }
    else{
      fall_rise_flag=0;
    }
    myBMD26M088.setBrightness(ADDRESS, breath_count); 
  }
  delay(breath_time);
}
