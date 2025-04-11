/*
 * File:   matrix_keypad.c
 * Author: Niharika
 *
 * Created on 11 November, 2024, 8:27 AM
 */


#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad(void)
{
	/* Config PORTB as digital */
	ADCON1 = 0x0F;

	/* Set Rows (RB7 - RB5) as Outputs and Columns (RB4 - RB1) as Inputs */
	TRISB = 0x1E;

	/* Set PORTB input as pull up for columns */
	RBPU = 0;

	MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}

unsigned char scan_key(void)
{
    //selecting row1
	ROW1 = LO;//make it 0
	ROW2 = HI;
	ROW3 = HI;

	if (COL1 == LO)//switch1 is pressed
	{
		return 1;
	}
	else if (COL2 == LO)
	{
		return 4;
	}
	else if (COL3 == LO)
	{
		return 7;
	}
	else if (COL4 == LO)
	{
		return 10;
	}

    //for row 2
	ROW1 = HI;
	ROW2 = LO;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 2;
	}
	else if (COL2 == LO)
	{
		return 5;
	}
	else if (COL3 == LO)
	{
		return 8;
	}
	else if (COL4 == LO)
	{
		return 11;
	}

    //select row 3
	ROW1 = HI;
	ROW2 = HI;
	ROW3 = LO;
	/* TODO: Why more than 2 times? */
	ROW3 = LO;//it is taking extra time t assigning the value so we are giving extra one instruction cycle to assign the value

	if (COL1 == LO)
	{
		return 3;
	}
	else if (COL2 == LO)
	{
		return 6;
	}
	else if (COL3 == LO)
	{
		return 9;
	}
	else if (COL4 == LO)
	{
		return 12;
	}

	return 0xFF;
}

unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;

	if (detection_type == STATE_CHANGE)
	{
		key = scan_key();
		if(key != 0xFF && once  )
		{
			once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{
		return scan_key();
	}

	return 0xFF;
}
