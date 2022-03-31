#include "i2c.h"

void i2c_write (Sercom *instance, const uint8_t address, const uint8_t *data, const uint8_t size) {
	while (instance->I2CM.STATUS.bit.BUSSTATE != 0x01);
	
	i2cStart (instance, address);
	if (i2cGetAck (instance)) { //nack
		i2cStop (instance);
		
		return;
	}
	
	for (uint8_t index = 0; index < size; index++) {
		i2cWriteByte (instance, *(data + index));
		
		if (i2cGetAck (instance)) {
			
			break;
		}
	}
	
	i2cStop (instance);
}

void i2c_read  (Sercom *instance, const uint8_t address, const uint8_t reg, uint8_t *dest, const uint8_t size) {
	while (instance->I2CM.STATUS.bit.BUSSTATE != 0x01);
	
	i2cStart (instance, address);
	if (i2cGetAck (instance)) { //nack
		i2cStop (instance);
		
		return;
	}
	
	i2cWriteByte (instance, reg);
	if (i2cGetAck (instance)) { //nack
		i2cStop (instance);
		
		return;
	}
	
	i2cReStart (instance, address | 0x01);
	if (i2cGetAck (instance)) { //nack
		i2cStop (instance);
		
		return;
	}
	
	for (uint8_t index = 0; index < size; index++) {
		//*(dest + index) = i2cReadByte (instance);
		*(dest + index) = instance->I2CM.DATA.bit.DATA;
		while (instance->I2CM.INTFLAG.bit.SB == 0);
		
		if ((index + 1) == size) {
			instance->I2CM.CTRLB.bit.ACKACT = 1;
		} else {
			instance->I2CM.CTRLB.bit.ACKACT = 0; //ACK
			i2cCmd (instance, 0x02);
		}
	}
	
	i2cStop (instance);
	
}

uint8_t i2cGetAck (Sercom *instance) { //0:ACK 1:NACK
	while (instance->I2CM.STATUS.bit.CLKHOLD == 0);
	
	return instance->I2CM.STATUS.bit.RXNACK;
}

void i2cSendAck (Sercom *instance, uint8_t ack) {
	
}

void i2cWait (Sercom *instance) {
	while (instance->I2CM.SYNCBUSY.bit.SYSOP);
}

void i2cCmd (Sercom *instance, const uint8_t cmd) {
	instance->I2CM.CTRLB.bit.CMD = cmd;
	i2cWait (instance);
}

void i2cStart (Sercom *instance, const uint8_t address) {
	instance->I2CM.ADDR.bit.ADDR = address;
	while (instance->I2CM.INTFLAG.bit.MB == 0);
}

void i2cReStart (Sercom *instance, const uint8_t address) {
	instance->I2CM.ADDR.bit.ADDR = address;
	while (instance->I2CM.INTFLAG.bit.SB == 0);
}

void i2cStop (Sercom *instance) {
	instance->I2CM.CTRLB.bit.CMD = 0x03;
	i2cWait (instance);
}

void i2cWriteByte (Sercom *instance, uint8_t data) {
	instance->I2CM.DATA.bit.DATA = data;
	while (instance->I2CM.INTFLAG.bit.MB == 0);
}

uint8_t i2cReadByte (Sercom *instance) {
	uint8_t b = 0;
	
	b = instance->I2CM.DATA.bit.DATA;
	while (instance->I2CM.INTFLAG.bit.SB == 0);
	
	return b;
}

