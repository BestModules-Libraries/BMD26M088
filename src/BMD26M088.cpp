/*****************************************************************
File:          BMD26M088.cpp
Author:        BESTMODULES
Description:   I2C communication with the BMD26M088 and control the RGB.
History：         
V1.0.1  -- initial version;2023-01-20;Arduino IDE : ≥v1.8.16
******************************************************************/
#include "BMD26M088.h"
/**********************************************************
Description: constructor 
parameter:
    Input:       *wirePort: Wire object if your board has more than one I2C interface              
    Output:         
Return:            
Others:      
**********************************************************/
BMD26M088::BMD26M088(TwoWire *theWire)
{	
	_i2cPort = theWire;
}
/**********************************************************
Description: Module Initial 
parameter:
    Input:                  
    Output:       
Return:            
Others:      
**********************************************************/
void BMD26M088::begin(uint8_t i2c_addr,uint32_t i2c_Clock)
{
	_i2cPort->begin();
	_i2cPort->setClock(i2c_Clock);
    //static display image   
    writeCmd(i2c_addr,HT16D33_CMD_DISPLAY_MODE,
                                    BMD26M088_DISPLAY_STATIC);         
    //display value mode,matrix type is 16x16
    writeCmd(i2c_addr,HT16D33_CMD_CONFIG_MODE,
            BMD26M088_DISPLAY_VALUE|BMD26M088_MATRIX_16x16); 
    //Configure HT16D33B and set the constant flow rate to 6ma      
    writeCmd(i2c_addr,HT16D33_CMD_SET_CURRENT,
                           BMD26M088_CCR_6MA);   
    //The global brightness,maximum 
    writeCmd(i2c_addr,HT16D33_CMD_SET_BRIGHTNESS,0xff);   
    //Blank area voltage control,default 0x8f; 
    writeCmd(i2c_addr,HT16D33_CMD_SET_V_BLANK,0x8f);    
    //set Internal crystal oscillator on          
    writeCmd(i2c_addr,HT16D33_CMD_SYS_CONTROL,
                           BMD26M088_SYS_OSC_ON);
    //set mode for normaly display 
    writeCmd(i2c_addr,HT16D33_CMD_SYS_CONTROL,
                            BMD26M088_SYS_DIS_ON);  
    //clear screen 
    clearAll(i2c_addr);  
    //set led ram on                  
    setLedRAMEnable(i2c_addr);     

    
}
/*************************************************
Description : check connection
parameter:
    Input:      
         i2c_addr:I2C address 
Return:  
Others:        
*************************************************/
bool BMD26M088::isConnected(uint8_t i2c_addr)
{
	uint8_t triesBeforeGiveup = 5;
	for (uint8_t x = 0; x < triesBeforeGiveup; x++)
	{
		_i2cPort->beginTransmission(i2c_addr);
		if (_i2cPort->endTransmission() == 0)
		{
			return true;
		}
		delay(100);
	}
	return false;
}


/*************************************************
Description :clear all/one/raw/column
parameter:
    Input:      
         i2c_addr:I2C address 
Return:  
Others:        
*************************************************/
void BMD26M088::clearAll(uint8_t i2c_addr)
{
    writeAllRGB(i2c_addr,0,0,0);
}
void BMD26M088::clearRGB(uint8_t i2c_addr, uint8_t RGB_Number)
{
    writeRGB(i2c_addr,RGB_Number,0,0,0);
}
void BMD26M088::clearRow(uint8_t i2c_addr, uint8_t RowIndex)
{
    writeRow(i2c_addr,RowIndex,0,0,0);
}
void BMD26M088::clearColumn(uint8_t i2c_addr, uint8_t ColumnIndex)
{
    writeColumn(i2c_addr,ColumnIndex,0,0,0);
}
/*************************************************
Description : write a rgb to display,the color data from color seting
parameter:
    Input:      
         i2c_addr:I2C address 
         RGB_Number:RGB index    
        R/G/B:color tdata 
Return:  
Others:        
*************************************************/
void BMD26M088::writeRGB(uint8_t i2c_addr,uint8_t RGB_Number, uint8_t R, uint8_t G, uint8_t B)
{
     const uint8_t *p=RAM_INDEX_TABLE[0];

	_i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
    _i2cPort->write(p[RGB_Number]);	
	_i2cPort->write(R);                    
    _i2cPort->write(G);                    
    _i2cPort->write(B);                    
	_i2cPort->endTransmission();
}
/*************************************************
Description : write a column to display the same color from color table
parameter:
    Input:      
         i2c_addr:I2C address     
        R/G/B:color data      
Return:  
Others:        
*************************************************/
void BMD26M088::writeColumn(uint8_t i2c_addr, uint8_t ColumnIndex, uint8_t R, uint8_t G, uint8_t B)
{
            uint8_t ram_addr=RAM_INDEX_TABLE[ColumnIndex][0];
             uint8_t rgb_count=0;

            _i2cPort->beginTransmission(i2c_addr);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(ram_addr); 
            
            while(1){//combine rgb data and some dumy data to liner address
                _i2cPort->write(R);                    
                _i2cPort->write(G);                    
                _i2cPort->write(B);  
                rgb_count++;//point to next rgb
                if(rgb_count>7) break;//is last rgb
                ram_addr+=3; //the address for next rgb;
                //if the ram_addr no equal to the next address  
                while(ram_addr<RAM_INDEX_TABLE[ColumnIndex][rgb_count]){
                    _i2cPort->write(0); //insert zero  
                    ram_addr++;    //ram_addr add 1
                } 
            }
            _i2cPort->endTransmission(); 
}

/*************************************************
Description : write a raw to display the same color from color table
parameter:
    Input:      
         i2c_addr:I2C address     
        R/G/B:color data      
Return:  
Others:        
*************************************************/
void BMD26M088::writeRow(uint8_t i2c_addr, uint8_t RawIndex, uint8_t R, uint8_t G, uint8_t B)
{
            for(uint8_t j = 0; j < 8; j++){

            _i2cPort->beginTransmission(i2c_addr);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(RAM_INDEX_TABLE[RawIndex][j] ); 
             _i2cPort->write(R);                    
            _i2cPort->write(G);                   
            _i2cPort->write(B); 
            _i2cPort->endTransmission();
        }  
}
/*************************************************
Description : write all rgb to display the same color from seting.
parameter:
    Input:      
         i2c_addr:I2C address     
        R/G/B:color data    
Return:  
Others:        
*************************************************/
void BMD26M088::writeAllRGB(uint8_t i2c_addr, uint8_t R, uint8_t G, uint8_t B)	
{
    for(uint8_t i = 0; i < 8; i++)//write 8 column
    {
            uint8_t ram_addr=RAM_INDEX_TABLE[i][0];
             uint8_t rgb_count=0;

            _i2cPort->beginTransmission(i2c_addr);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(ram_addr); 
            
            while(1){//combine rgb data and some dumy data to liner address
                _i2cPort->write(R);                    
                _i2cPort->write(G);                   
                _i2cPort->write(B);  
                rgb_count++;//point to next rgb
                if(rgb_count>7) break;//is last rgb
                ram_addr+=3; //the address for next rgb;
                //if the ram_addr no equal to the next address  
                while(ram_addr<RAM_INDEX_TABLE[i][rgb_count]){
                    _i2cPort->write(0); //insert zero  
                    ram_addr++;    //ram_addr add 1
                } 
            }
            _i2cPort->endTransmission(); 
    }
}
/*************************************************
Description : set display a char from char table.
parameter:
    Input:      
         i2c_addr:I2C address   
         R/G/B:color    
Return:  
Others:        
*************************************************/
void BMD26M088::DrawAsciiChar(uint8_t i2c_addr, char Ch, uint8_t R, uint8_t G, uint8_t B)
{
  uint8_t Temp, Index;
  
  Index = Ch - 32;

  for(uint8_t i = 0; i < 8; i++)
  {
    Temp = ASCII_8X8[Index][i];
    for(uint8_t j = 0; j < 8; j++)
    {
      if(((Temp >> j)  & 0x01))
      {
		  writeRGB(i2c_addr, i * 8 + j, R,G,B);
      }
   }
  } 
}

/*************************************************
Description : enable led ram
parameter:
    Input:      
         i2c_addr:I2C address             
Return:  
Others:        
*************************************************/
const uint8_t LED_ON_OFF_16_16[32] =//see HT16D33A datasheet page21~22
{
	0xF8, 0x7F,                                     //0x00,0x01
    0xF8, 0x7F,                                     //0x02,0x03
    0xF8, 0x7F,                                     //0x04,0x05
    0xF8, 0x7F,                                     //0x06,0x07
    0xCE, 0x7F,                                     //0x08,0x09
    0xCE, 0x7F,                                     //0x0a,0x0b
    0xCE, 0x7F,                                     //0x0c,0x0d
    0x7E, 0x7E,                                     //0x0e,0x0f
    0x7E, 0x7E,                                     //0x10,0x11
    0x7E, 0x7E,                                     //0x12,0x13
    0xFE, 0x73,                                     //0x14,0x15
    0xFE, 0x73,                                     //0x16,0x17
    0xFE, 0x73,                                     //0x18,0x19
    0xFE, 0x1F,                                     //0x1a,0x1b
    0xFE, 0x1F,                                     //0x1c,0x1d
    0xFE, 0x1F                                      //0x1e,0x1f  
};
void BMD26M088::setLedRAMEnable(uint8_t i2c_addr) 
{
	_i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x00);                
	for(uint8_t i = 0; i < 28; i++)
	{
		_i2cPort->write(LED_ON_OFF_16_16[i]); 
	} 
	_i2cPort->endTransmission();
	//Repeat transmission
    _i2cPort->beginTransmission(i2c_addr);//beacuse arduino I2C Maximum transmission 32byte
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(28);
	for(uint8_t i = 28; i < 32; i++) 
    {
		_i2cPort->write(LED_ON_OFF_16_16[i]);      
    }
	_i2cPort->endTransmission();
}
/*************************************************
Description : disable led ram
parameter:
    Input:      
         i2c_addr:I2C address             
Return:  
Others:        
*************************************************/
void BMD26M088::setLedRAMDisable(uint8_t i2c_addr) 
{
	_i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x00);                
	for(uint8_t i = 0; i < 28; i++)
	{
		_i2cPort->write(0); 
	} 
	_i2cPort->endTransmission();

    _i2cPort->beginTransmission(i2c_addr);//beacuse arduino I2C Maximum transmission 32byte
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x1C);
	for(uint8_t i = 28; i < 32; i++) 
    {
		_i2cPort->write(0);      
    }
	_i2cPort->endTransmission();
}

/*************************************************
Description : rest module
parameter:
    Input:      
         i2c_addr:I2C address             
Return:  
Others:        
*************************************************/
void BMD26M088::reset(uint8_t i2c_addr)
{
	_i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(RESET);
	_i2cPort->endTransmission();
}
/*************************************************
Description : rest module
parameter:
    Input:      
         i2c_addr:I2C address             
Return:  
Others:        
*************************************************/
void BMD26M088::_writeRegister(uint8_t i2c_addr, uint8_t registerAddress,uint8_t data)
{
	_i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(registerAddress);

		_i2cPort->write(data);

	_i2cPort->endTransmission() ;

}
/*************************************************
Description: Write data to the register
see the simHT16D33A_Bv132.pdf
parameter:
    Input:      
         i2c_addr:I2C address   
         cmd: 
            HT16D33_CMD_DISPLAY_MODE 0X30
            HT16D33_CMD_CONFIG_MODE 0x31
            HT16D33_CMD_CONNECT_MODE 0x34
            HT16D33_CMD_SYS_CONTROL 0x35
            HT16D33_CMD_SET_CURRENT 0x36
            HT16D33_CMD_SET_BRIGHTNESS 0x37
            HT16D33_CMD_SET_V_BLANK 0x39
         data:data					             
Return:   
*************************************************/
void BMD26M088::writeCmd(uint8_t i2c_addr,uint8_t cmd,uint8_t data)
{
    _writeRegister(i2c_addr, cmd, data);
}

/*************************************************
Description: set all RGB brightness
parameter:
    Input:      
         i2c_addr:I2C address   
         brightness:brightness,0~255,the max brightness is 255				             
Return:   
*************************************************/
void BMD26M088::setBrightness( uint8_t i2c_addr,uint8_t brightness)
{
    _writeRegister(i2c_addr,HT16D33_CMD_SET_BRIGHTNESS,brightness);
}
/*************************************************
Description: set current
parameter:
    Input:      
         i2c_addr:I2C address   
         brightness:level current = 1+3*level(mA) 					             
Return:   
*************************************************/
void BMD26M088::setCurrent( uint8_t i2c_addr,uint8_t level)
{
    _writeRegister(i2c_addr,HT16D33_CMD_SET_CURRENT,level);
}
