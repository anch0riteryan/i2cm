#include "I2cM.h"
#include "I2c.h"

I2cMasterHandler *initI2cMaster (Sercom *sercom, void (*init) (void )) {
	I2cMasterHandler *p;
	
	p = malloc (sizeof (I2cMasterHandler));
	p->instance = sercom;
	p->init = init;
	
	p->init ();
	p->write = i2c_write;
	p->read = i2c_read;
	
	//setup sercom
	sercom->I2CM.CTRLA.bit.SWRST = 1;
	while (sercom->I2CM.CTRLA.bit.SWRST);
	
	//sercom->I2CM.CTRLB.bit.QCEN = 1;
	//sercom->I2CM.CTRLB.bit.SMEN = 1;
	
	sercom->I2CM.BAUD.bit.BAUD = 64;
	sercom->I2CM.BAUD.bit.BAUDLOW = 48;
	
	sercom->I2CM.CTRLA.bit.MODE = 5; //i2c master
	sercom->I2CM.CTRLA.bit.LOWTOUTEN = 1;
	//sercom->I2CM.CTRLA.bit.INACTOUT = 0;
	//sercom->I2CM.CTRLA.bit.SCLSM = 1;
	sercom->I2CM.CTRLA.bit.SPEED = 0;
	sercom->I2CM.CTRLA.bit.SEXTTOEN = 1;
	sercom->I2CM.CTRLA.bit.MEXTTOEN = 1;
	sercom->I2CM.CTRLA.bit.SDAHOLD = 2;
	//sercom->I2CM.CTRLA.bit.PINOUT = 0;
	
	sercom->I2CM.INTENSET.bit.ERROR = 1;

	sercom->I2CM.CTRLA.bit.ENABLE = 1;
	while (sercom->I2CM.SYNCBUSY.bit.ENABLE);
	
	sercom->I2CM.STATUS.bit.BUSSTATE = 1;
	
	return p;
}
