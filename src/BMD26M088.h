/*************************************************
File:       	    BMD26M088.h
Author:            	BESTMODULES
Description:        Define classes and required variables
History：			
V1.0.1	 -- initial version；2023-01-20；Arduino IDE : ≥v1.8.16
**************************************************/
#ifndef _BMD26M088_H
#define _BMD26M088_H

#include <Arduino.h>
#include <Wire.h>
#include <FONT.h>


const uint8_t RAM_INDEX_TABLE[8][8] = //8x8RGB RAM address index
{//you can set data to this address to set led on or off 
//such as set number RGB1,write R G B value to 0x03 0x04 0x05.
	{0x03,0x06,0x09,0x0C,0x13,0x16,0x19,0x1C},	//column 1
	{0x23,0x26,0x29,0x2C,0x33,0x36,0x39,0x3C},	//column 2
	{0x41,0x46,0x49,0X4C,0x51,0x56,0x59,0X5C},	//column 3
	{0x61,0x66,0x69,0X6C,0x71,0x74,0x79,0X7C},	//column 4
	{0x81,0x84,0x89,0X8C,0x91,0x94,0x99,0X9C},	//column 5
	{0xA1,0xA4,0xA7,0XAC,0xB1,0xB4,0xB7,0XBC},	//column 6
	{0xC1,0xC4,0xC7,0XCC,0xD1,0xD4,0xD7,0XDA},	//column 7
	{0xE1,0xE4,0xE7,0XEA,0xF1,0xF4,0xF7,0XFA}	//column 8
};



#define	BMD26M088_CCR_3MA 0
#define	BMD26M088_CCR_6MA 1
#define	BMD26M088_CCR_9MA 2
#define	BMD26M088_CCR_12MA 3
#define	BMD26M088_CCR_15MA 4
#define	BMD26M088_CCR_18MA 5
#define	BMD26M088_CCR_21MA 6
#define	BMD26M088_CCR_24MA 7
#define	BMD26M088_CCR_27MA 8
#define	BMD26M088_CCR_30MA 9
#define	BMD26M088_CCR_33MA 10
#define	BMD26M088_CCR_36MA 11
#define	BMD26M088_CCR_39MA 12
#define	BMD26M088_CCR_42MA 13
#define	BMD26M088_CCR_45MA 14
#define	BMD26M088_CCR_48MA 15

#define BMD26M088_DISPLAY_STATIC 0 
#define BMD26M088_DISPLAY_DYNAMICS 1 
#define BMD26M088_DISPLAY_VALUE 0x00
#define BMD26M088_DISPLAY_BIN 0x80
#define BMD26M088_MATRIX_9x10 1
#define BMD26M088_MATRIX_12x12 2
#define BMD26M088_MATRIX_16x16 3

#define BMD26M088_SYS_STANDBY 0
#define BMD26M088_SYS_OSC_ON 2
#define BMD26M088_SYS_DIS_ON 3

#define BMD26M088_I2C_ADDRESS_VDD 0X67
#define BMD26M088_I2C_ADDRESS_GND 0x64
#define BMD26M088_I2C_ADDRESS_SCL 0X65
#define BMD26M088_I2C_ADDRESS_SDA 0X66
#define BMD26M088_I2C_ADDRESS_BOARDCAST 0X2E

#define HT16D33_CMD_DISPLAY_MODE 0X30
#define HT16D33_CMD_CONFIG_MODE 0x31
#define HT16D33_CMD_CONNECT_MODE 0x34
#define HT16D33_CMD_SYS_CONTROL 0x35
#define HT16D33_CMD_SET_CURRENT 0x36
#define HT16D33_CMD_SET_BRIGHTNESS 0x37
#define HT16D33_CMD_SET_V_BLANK 0x39
#define HT16D33_CMD_WRITE_LED_DATA 0x80
#define HT16D33_CMD_SET_LED_ON_OFF 0x84
#define HT16D33_CMD_RESET 0xcc


class BMD26M088
{
	public:	
	    BMD26M088(TwoWire *theWire = &Wire);
		void begin(uint8_t i2c_addr=0x2e,uint32_t i2c_Clock=400000);
		void reset(uint8_t  i2c_addr=0x2E);
		bool isConnected(uint8_t i2c_addr);
	
		void writeCmd(uint8_t i2c_addr,uint8_t cmd,uint8_t data);

 		void setLedRAMEnable(uint8_t i2c_addr);
		void setLedRAMDisable(uint8_t i2c_addr);	 
		void setBrightness( uint8_t i2c_addr,uint8_t brightness);
		void setCurrent(uint8_t i2c_addr, uint8_t level);
		
        void clearAll(uint8_t i2c_addr);
    	void clearRGB(uint8_t i2c_addr, uint8_t RGB_Number);
		void clearRow(uint8_t i2c_addr, uint8_t RowIndex);
		void clearColumn(uint8_t i2c_addr, uint8_t ColumnIndex);
		

		void writeRGB(uint8_t i2c_addr,uint8_t RGB_Number, uint8_t R, uint8_t G, uint8_t B);
        void writeAllRGB(uint8_t i2c_addr, uint8_t R, uint8_t G, uint8_t B);
		void writeColumn(uint8_t i2c_addr, uint8_t ColumnIndex, uint8_t R, uint8_t G, uint8_t B);
		void writeRow(uint8_t i2c_addr, uint8_t RawIndex, uint8_t R, uint8_t G, uint8_t B);
		void DrawAsciiChar(uint8_t i2c_addr, char Ch, uint8_t R, uint8_t G, uint8_t B);
	
	private:
		TwoWire *_i2cPort = NULL;
        void _writeRegister(uint8_t i2c_addr, uint8_t registerAddress,uint8_t data);
};
#endif
