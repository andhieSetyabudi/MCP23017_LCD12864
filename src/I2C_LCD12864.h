#ifndef MCP23017_LCD12864_H_
#define MCP23017_LCD12864_H_


#include "Arduino.h"
#include "stdio.h"
#include "Wire.h"
#include "lcdBase.h"
#include "u8x8_font.h"


/**
 * @brief 
 * 
 * PIN-MAP of the module
 * MCP23017             LCD12864
 * GND                  GND
 * VCC                  VCC
 * Vo                   Vo
 * PA7                  D/I
 * PA6				R/W
 * PA5				E
 * PB0				DB0
 * PB1				DB1
 * PB2				DB2
 * PB3				DB3
 * PB4				DB4
 * PB5				DB5
 * PB6				DB6
 * PB7				DB7
 * PA4				PSB
 * PA3				NC
 * PA2				RST
 * Vout				VEE/NC => LCD drive voltage/NC
 * Vcc				VCC for led
 * PA1				gnd for led
 * 
 */


enum class MCP23_Port : uint8_t
{
	A = 0,
	B = 1
};

/**
 * Controls if the two interrupt pins mirror each other.
 * See "3.6 Interrupt Logic".
 */
enum class MCP23_InterruptMode : uint8_t
{
	Separated = 0,	///< Interrupt pins are kept independent
	Or = 0b01000000	///< Interrupt pins are mirrored
};

/**
 * Registers addresses.
 * The library use addresses for IOCON.BANK = 0.
 * See "3.2.1 Byte mode and Sequential mode".
 */
enum class MCP23_Register : uint8_t
{
	IODIR_A		= 0x00, 		///< Controls the direction of the data I/O for port A.
	IODIR_B		= 0x01,			///< Controls the direction of the data I/O for port B.
	IPOL_A		= 0x02,			///< Configures the polarity on the corresponding GPIO_ port bits for port A.
	IPOL_B		= 0x03,			///< Configures the polarity on the corresponding GPIO_ port bits for port B.
	GPINTEN_A	= 0x04,			///< Controls the interrupt-on-change for each pin of port A.
	GPINTEN_B	= 0x05,			///< Controls the interrupt-on-change for each pin of port B.
	DEFVAL_A	= 0x06,			///< Controls the default comparaison value for interrupt-on-change for port A.
	DEFVAL_B	= 0x07,			///< Controls the default comparaison value for interrupt-on-change for port B.
	INTCON_A	= 0x08,			///< Controls how the associated pin value is compared for the interrupt-on-change for port A.
	INTCON_B	= 0x09,			///< Controls how the associated pin value is compared for the interrupt-on-change for port B.
	IOCON		= 0x0A,			///< Controls the device.
	GPPU_A		= 0x0C,			///< Controls the pull-up resistors for the port A pins.
	GPPU_B		= 0x0D,			///< Controls the pull-up resistors for the port B pins.
	INTF_A		= 0x0E,			///< Reflects the interrupt condition on the port A pins.
	INTF_B		= 0x0F,			///< Reflects the interrupt condition on the port B pins.
	INTCAP_A	= 0x10,			///< Captures the port A value at the time the interrupt occured.
	INTCAP_B	= 0x11,			///< Captures the port B value at the time the interrupt occured.
	GPIO_A		= 0x12,			///< Reflects the value on the port A.
	GPIO_B		= 0x13,			///< Reflects the value on the port B.
	OLAT_A		= 0x14,			///< Provides access to the port A output latches.
	OLAT_B		= 0x15,			///< Provides access to the port B output latches.
};

inline MCP23_Register operator+(MCP23_Register a, MCP23_Port b) {
	return static_cast<MCP23_Register>(static_cast<uint8_t>(a) + static_cast<uint8_t>(b));
};

class MCP23017_LCD12864:public lcdBase{
    private :
        TwoWire *i2c_ = NULL;        uint8_t addr;
        uint16_t pinIO;     // MSB : GPIOB & LSB : GPIOA
        uint8_t readRegister(MCP23_Register reg);
        void readRegister(MCP23_Register reg, uint8_t& portA, uint8_t& portB);
        void writeRegister(MCP23_Register reg, uint8_t value);
        void writeRegister(MCP23_Register reg, uint8_t portA, uint8_t portB);
        void onParallelMode(void);
        void transfer(uint8_t type_, uint8_t data);
        uint8_t  bl_ = 0;
    public :
        MCP23017_LCD12864(TwoWire *wire = &Wire, uint8_t addr = 0x20)
        {
            this->i2c_  = wire;
            this->addr  = addr;
            this->bl_ = 1;
        };

        ~MCP23017_LCD12864();

        void init(void);

        void fillBuffer(bool fill_type);
        void clearBuffer(void);

        void clear(bool type_clear, bool fill_type);
        void sendBuffer(bool type_flush = 0);

        void setBL_on(void) ;
        void setBL_off(void);
};


#endif