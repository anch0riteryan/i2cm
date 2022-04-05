#include <stdint.h>
#include <samc21.h>

//
void test (Sercom *instance);
void i2c_write (Sercom *instance, const uint8_t address, const uint8_t *data, const uint8_t size);
void i2c_read  (Sercom *instance, const uint8_t address, const uint8_t reg, uint8_t *dest, const uint8_t size);

//
uint8_t i2cGetAck (Sercom *instance);
void i2cSendAck (Sercom *instacne, uint8_t ack);
void i2cWait (Sercom *instance);
void i2cCmd (Sercom *instance, const uint8_t cmd);
void i2cStart (Sercom *instance, const uint8_t address);
void i2cReStart (Sercom *instance, const uint8_t address);
void i2cStop (Sercom *instance);
void i2cWriteByte (Sercom *instance, uint8_t data);
uint8_t i2cReadByte (Sercom *instance);
