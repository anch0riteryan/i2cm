#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <samc21.h>

typedef struct _i2c_master_handler {
	Sercom *instance;
	
	void (*init) (void ); //
	void (*isr) (Sercom *instance);
	
	void (*write) (Sercom *instance, const uint8_t address, const uint8_t *data, const uint8_t size);
	void (*read)  (Sercom *instance, const uint8_t address, const uint8_t reg, uint8_t *dest, const uint8_t size);
	
	
} I2cMasterHandler;

I2cMasterHandler *initI2cMaster (Sercom *sercom, void (*init) (void ));
