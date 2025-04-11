/*
 * File:   i2c.c
 * Author: Admin
 *
 * Created on 16 November, 2024, 10:19 AM
 */



#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;// Configure RC3 (SCL) as input for I2C clock line
	TRISC4 = 1;// Configure RC4 (SDA) as input for I2C data line
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;// Set the I2C clock speed by setting the baud rate register
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}

void i2c_idle(void)//check weather the instruction has been executed or not
{
	while (!SSPIF);// Wait until SSPIF flag is set, indicating operation completion
	SSPIF = 0;// Clear SSPIF to be ready for next operation
}

void i2c_ack(void)//not nessesary
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}

void i2c_start(void)
{
	SEN = 1;
	i2c_idle();
}

void i2c_stop(void)
{
	PEN = 1;
	i2c_idle();
}

void i2c_rep_start(void)
{
	RSEN = 1;
	i2c_idle();
}

void i2c_write(unsigned char data)
{
	SSPBUF = data;
	i2c_idle();
}

void i2c_rx_mode(void)
{
	RCEN = 1;
	i2c_idle();
}

void i2c_no_ack(void)
{
	ACKDT = 1;
	ACKEN = 1;
}

unsigned char i2c_read(void)
{
	i2c_rx_mode();
	i2c_no_ack();

	return SSPBUF;
}

