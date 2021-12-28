#include "MCP23017_LCD12864.h"

uint8_t MCP23017_LCD12864::readRegister(MCP23_Register reg)
{
	this->i2c_->beginTransmission(this->addr);
	this->i2c_->write(static_cast<uint8_t>(reg));
	this->i2c_->endTransmission();
	this->i2c_->requestFrom(this->addr, (uint8_t)1);
	return this->i2c_->read();
}

void MCP23017_LCD12864::readRegister(MCP23_Register reg, uint8_t& portA, uint8_t& portB)
{
	this->i2c_->beginTransmission(this->addr);
	this->i2c_->write(static_cast<uint8_t>(reg));
	this->i2c_->endTransmission();
	this->i2c_->requestFrom(this->addr, (uint8_t)2);
	portA = this->i2c_->read();
	portB = this->i2c_->read();
}

void MCP23017_LCD12864::writeRegister(MCP23_Register reg, uint8_t value)
{
    this->i2c_->beginTransmission(this->addr);
	this->i2c_->write(static_cast<uint8_t>(reg));
	this->i2c_->write(value);
	this->i2c_->endTransmission();
}

void MCP23017_LCD12864::writeRegister(MCP23_Register reg, uint8_t portA, uint8_t portB)
{
	this->i2c_->beginTransmission(this->addr);
	this->i2c_->write(static_cast<uint8_t>(reg));
	this->i2c_->write(portA);
	this->i2c_->write(portB);
	this->i2c_->endTransmission();
}


void MCP23017_LCD12864::onParallelMode(void)
{
    byte val = 0x24;
    this->writeRegister(MCP23_Register::GPIO_A , val);
    delayMicroseconds(72);
    val = 0x34;
    this->writeRegister(MCP23_Register::GPIO_A , val);
    delayMicroseconds(72);
    val = 0x14;
    this->writeRegister(MCP23_Register::GPIO_A , val);
    delayMicroseconds(72); 
}

void MCP23017_LCD12864::transfer(uint8_t type_, uint8_t data)
{
    uint8_t ctrl=0x34;      // PSB & RST always pull-up, paralel mode, and enable hi
    if( !this->bl_ )
        ctrl |= 0x02;
    this->writeRegister(MCP23_Register::GPIO_A , ctrl);

    bitWrite(ctrl,7,type_>0?1:0);     // D/I pin
    bitClear(ctrl,6);           // R/W

    this->writeRegister(MCP23_Register::GPIO_A , ctrl);
    this->writeRegister(MCP23_Register::GPIO_B , data);
    delayMicroseconds(1);    
    bitClear(ctrl,5);           // clear bit enable
    this->writeRegister(MCP23_Register::GPIO_A , ctrl);  
    delayMicroseconds(1);
}


void MCP23017_LCD12864::fillBuffer(bool fill_type)
{
    memset(frame_buffer, fill_type ? 0xff : 0x0, 1024);
}

void MCP23017_LCD12864::clearBuffer(void)
{
    fillBuffer(0);
}

void MCP23017_LCD12864::clear(bool type_clear, bool fill_type)
{
    if (type_clear == 0)
    {
        for (byte i = 0; i < 32; i++) {
            this->transfer(0, 0x80 | i);
            this->transfer(0, 0x80);
            for (uint8_t i = 0; i < 32; i++) {
                this->transfer(1, fill_type ? 0xff : 0x00);
            }
        }
    }
    else if (type_clear == 1)
    {
        for(uint8_t count=0;count <= 8;count+=8){
            for (byte i = 0; i < 32; i++) {
                this->transfer(0, 0x80 | i);
                this->transfer(0, 0x80 | count);
                for (uint8_t i = 0; i < 16; i++) {
                    this->transfer(1, fill_type ? 0xff : 0x00);
                }
            }
        }
    }
}

void MCP23017_LCD12864::sendBuffer(bool type_flush)
{
    uint8_t temp_buffer = 0x0;
    if (type_flush == 0)
    {
        for (uint8_t row = 0; row < 32; row++) {
            this->transfer(0, 0x80 | row);
            this->transfer(0, 0x80);
            for (uint8_t ByteBuffer = 0 ; ByteBuffer < 16; ByteBuffer++) {
                temp_buffer = frame_buffer[row][ByteBuffer];
                this->transfer(1, temp_buffer);
                temp_buffer = 0x0;
            }
            for (uint8_t ByteBuffer = 0 ; ByteBuffer < 16; ByteBuffer++) {
                temp_buffer = frame_buffer[row + 32][ByteBuffer];
                this->transfer(1, temp_buffer);
                temp_buffer = 0x0;
            }
        }
    }
    else if (type_flush == 1)
    {
        for (uint8_t row = 0; row < 64; row++) {
            if ( row < 32) {
                this->transfer(0, 0x80 | row );
                this->transfer(0, 0x80);
            } else {
                this->transfer(0, 0x80 | row - 32);
                this->transfer(0, 0x80 | 8);
            }
            for (uint8_t ByteBuffer = 0 ; ByteBuffer < 16; ByteBuffer++) {
                temp_buffer = frame_buffer[row][ByteBuffer];
                this->transfer(1, temp_buffer );
                temp_buffer = 0x0;
            }
        }
    }
}

MCP23017_LCD12864::~MCP23017_LCD12864(){};

void MCP23017_LCD12864::init()
{
	//BANK = 	0 : sequential register addresses
	//MIRROR = 	0 : use configureInterrupt 
	//SEQOP = 	1 : sequential operation disabled, address pointer does not increment
	//DISSLW = 	0 : slew rate enabled
	//HAEN = 	0 : hardware address pin is always enabled on 23017
	//ODR = 	0 : open drain output
	//INTPOL = 	0 : interrupt active low
	this->writeRegister(MCP23_Register::IOCON, 0b00100000);

	//enable all pull up resistors (will be effective for input pins only)
	this->writeRegister(MCP23_Register::GPPU_A, 0xFF, 0xFF);
    
    // set port mode, 
    this->writeRegister(MCP23_Register::IODIR_A , 0x00);     // set all pin as output
	this->writeRegister(MCP23_Register::GPPU_A , 0xFF);
	this->writeRegister(MCP23_Register::IPOL_A , 0x00);

    this->writeRegister(MCP23_Register::IODIR_B , 0x00);     // set all pin as output
	this->writeRegister(MCP23_Register::GPPU_B , 0xFF);
	this->writeRegister(MCP23_Register::IPOL_B , 0x00);
    delayMicroseconds(10);
    this->writeRegister(MCP23_Register::GPIO_A , 0x10);     // set PSB to high
	this->writeRegister(MCP23_Register::GPIO_B , 0x00);
    delayMicroseconds(50);
    this->writeRegister(MCP23_Register::GPIO_A , 0x14);     // set PSB to high & Reset to high
    
    
    // init lcd
    this->onParallelMode();
    this->transfer(0, 0x34);
    delayMicroseconds(72);
    this->transfer(0, 0x30);
    delayMicroseconds(72);
    this->transfer(0, 0x0C);
    delayMicroseconds(72);
    this->transfer(0, 0x01);
    delayMicroseconds(72);
    this->transfer(0, 0x06);
    delayMicroseconds(1600);

    this->transfer(0, 0x34);
    delayMicroseconds(72);
    this->transfer(0, 0x36);
    delayMicroseconds(72);
    this->transfer(0, 0x80);
    delayMicroseconds(72);
    this->transfer(0, 0x80);
}


void MCP23017_LCD12864::setBL_off(void)
{
    // backlight pin on PA1, active low
    this-> bl_ = 1;
    uint8_t val = 0x14;
    this->writeRegister(MCP23_Register::GPIO_A,val);
}


void MCP23017_LCD12864::setBL_on(void)
{
    // backlight pin on PA1, active low
    this-> bl_ = 0;
    uint8_t val = 0x16;
    this->writeRegister(MCP23_Register::GPIO_A,val);
}