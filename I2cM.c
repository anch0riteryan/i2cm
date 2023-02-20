#include "I2cM.h"
#include "I2c.h"

I2cMasterHandler *initI2cMaster (Sercom *sercom, void (*init) (void )) {
	I2cMasterHandler *p;

	p = malloc (sizeof (I2cMasterHandler));
	p->instance = sercom;
	p->init = init;
	p->err = 0;
	
	p->init ();
	p->isr = i2c_isr;
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
	sercom->I2CM.CTRLB.bit.CMD = 0x03; //stop

	return p;
}

void i2c_isr (Sercom *instance) {
	if (instance->I2CM.INTFLAG.bit.ERROR) {
		if (instance->I2CM.STATUS.bit.LENERR) {
			//printf ("len err\n");
			instance->I2CM.STATUS.bit.LENERR = 1;
		}

		if (instance->I2CM.STATUS.bit.SEXTTOUT) {
			//printf ("slave scl time-out\n");
			instance->I2CM.STATUS.bit.SEXTTOUT = 1;
		}

		if (instance->I2CM.STATUS.bit.MEXTTOUT) {
			//printf ("master scl time-out\n");
			instance->I2CM.STATUS.bit.MEXTTOUT = 1;
		}

		if (instance->I2CM.STATUS.bit.LOWTOUT) {
			//printf ("low time-out\n");
			instance->I2CM.STATUS.bit.LOWTOUT = 1;
		}

		if (instance->I2CM.STATUS.bit.ARBLOST) {
			//printf ("arb lost\n");
			instance->I2CM.STATUS.bit.ARBLOST = 1;
		}

		if (instance->I2CM.STATUS.bit.BUSERR) {
			//printf ("bus err\n");
			instance->I2CM.STATUS.bit.BUSERR = 1;
		}
		
		i2c->err = instance->I2CM.STATUS.reg;

		instance->I2CM.INTFLAG.reg = 0x80;
	}

	if (instance->I2CM.INTFLAG.bit.SB) {

		//

		instance->I2CM.INTFLAG.reg = 0x02;
	}

	if (instance->I2CM.INTFLAG.bit.MB) {

		//

		instance->I2CM.INTFLAG.reg = 0x01;
	}
}
