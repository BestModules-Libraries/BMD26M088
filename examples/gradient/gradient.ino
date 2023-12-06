
/*************************************************
File:
Description:
Note:               
**************************************************/
#include <BMD26M088.h>
BMD26M088 myBMD26M088(&Wire2);
#define ADDRESS BMD26M088_I2C_ADDRESS_VDD

void setup() 
{
  myBMD26M088.begin(ADDRESS);  //I2C init 
  myBMD26M088.setBrightness(ADDRESS,0xff);//set global brightness,max
  myBMD26M088.setCurrent(ADDRESS,BMD26M088_CCR_6MA);//set drive current 6mA 

  //gradient control
  myBMD26M088.writeAllRGB(ADDRESS, 0xff, 0, 0);//set all RGB red gradient
  //myBMD26M088.writeRGB(ADDRESS, 1,0xff, 0, 0);//set one RGB red gradient
  //set globe gradient control
  myBMD26M088.setGradient(ADDRESS,GFS_GRADIENT_GARMA,GFT_T1_1024_FRAME);
  
}   
void loop()
{
 
}
