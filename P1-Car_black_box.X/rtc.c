/*
 * File:   rtc.c
 * Author: Niharika
 *
 * Created on 20 November, 2024, 5:39 PM
 */


#include <xc.h>
#include "ds1307.h"

extern unsigned char clock_reg[3], time[9];
void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);//12 hours format
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);//24 hours format
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);//to display miniute
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);//display seconds
	time[7] = '0' + (clock_reg[2] & 0x0F);
//	time[8] = '\0';
}

