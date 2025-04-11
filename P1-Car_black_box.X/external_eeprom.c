/*
 * File:   external_eeprom.c
 * Author: Niharika
 *
 * Created on 16 November, 2024, 10:39 AM
 */


#include <xc.h>
#include "main.h"
#include "ds1307.h"
#include "i2c.h"
#include "external_eeprom.h"

// Function to write a byte of data to the external EEPROM
void write_external_eeprom(unsigned char address, unsigned char data){
	i2c_start();
	i2c_write(SLAVE_EEPROM_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(unsigned int wait = 3000; wait--;);
}
// Function to read a byte of data from the external EEPROM
unsigned char read_external_eeprom(unsigned char address){
	unsigned char data;
    i2c_start();
	i2c_write(SLAVE_EEPROM_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_EEPROM_READ);
	data = i2c_read();
	i2c_stop();
    return data;
}