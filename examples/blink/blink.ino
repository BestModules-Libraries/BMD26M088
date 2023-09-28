
/*************************************************
File:             blink.ino
Description:     blink
Note:               
**************************************************/
#include <BMD26M088.h>
BMD26M088 myBMD26M088(&Wire2);
#define ADDRESS BMD26M088_I2C_ADDRESS_SCL
void setup() 
{                                  
    myBMD26M088.begin(ADDRESS);  //I2C init 
    myBMD26M088.setBrightness(ADDRESS,0xff);//set global brightness,max
    myBMD26M088.setCurrent(ADDRESS,BMD26M088_CCR_6MA);//set drive current 6mA                 
}
void loop()
{     
  //set init color from color table by index.
    myBMD26M088.writeAllRGB(ADDRESS, 0xff,0,0);  
    delay(200);//delay 200ms
    myBMD26M088.clearAll(ADDRESS);//switch off all light
    delay(200);                                       
}