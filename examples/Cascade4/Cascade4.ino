
/*************************************************
File:             Cascade4.ino
Description:    Cascade 4 module on bus  
Note:               
**************************************************/
#include "BMD26M088.h"


BMD26M088 myBMD26M088(&Wire2); //BMCOM2 is use I2C2


#define i2c_addr_boardcast BMD26M088_I2C_ADDRESS_BOARDCAST
#define i2c_addr_board1 BMD26M088_I2C_ADDRESS_VDD
#define i2c_addr_board2 BMD26M088_I2C_ADDRESS_GND
#define i2c_addr_board3 BMD26M088_I2C_ADDRESS_SCL
#define i2c_addr_board4 BMD26M088_I2C_ADDRESS_SDA

void setup() 
{                                           
    myBMD26M088.begin(i2c_addr_boardcast);  //I2C init    
    myBMD26M088.setBrightness(i2c_addr_boardcast,0xff);//set global brightness,max
    myBMD26M088.setCurrent(i2c_addr_boardcast,BMD26M088_CCR_6MA);//set drive current 6mA   

    myBMD26M088.writeAllRGB(i2c_addr_board1, 0xff,0,0);     
    myBMD26M088.writeAllRGB(i2c_addr_board2, 0,0xff,0); 
    myBMD26M088.writeAllRGB(i2c_addr_board3, 0,0,0xff);       
    myBMD26M088.writeAllRGB(i2c_addr_board4, 0xff,0xff,0);                    
}
void loop()
{
                                        
}
