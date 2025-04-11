/*
 * File:   main_menu.c
 * Author: Niharika
 *
 * Created on 14 November, 2024, 2:21 PM
 */


#include <xc.h>
#include "matrix_keypad.h"
#include "ds1307.h"


int storing_data;
extern int count, flag1, flag3, star, hours, min, sec, storing_data, stop = 1;
void main_menu(unsigned char key){
    // If the key pressed is MK_SW11, increment flag1 and handle actions based on its value
    if(key == MK_SW11){
        flag1++;// Increment flag1 to move between menu states
        
        // If flag1 exceeds 3, make flag1 as 3
        if(flag1 == 4){
            flag1 = 3;
            
            // If we're in the second menu and selected the first option, write the time to the DS1307
            if((flag3 == 2 && star == 1) || (flag3 == 1 && star == 2)){
                write_ds1307(HOUR_ADDR,((hours/10) << 4) | (hours % 10));// Write hours to DS1307
                write_ds1307(MIN_ADDR,((min/10) <<4) | (min%10));// Write minutes to DS1307
                write_ds1307(SEC_ADDR,((sec/10) <<4) | (sec%10)  );// Write seconds to DS1307
                flag1 = 1;// Reset flag1 to 1 after writing time
            }star = 1;
        }
        
        // If we are in the second menu, selected the second star, and in flag1=3 mode, reset storing_data
        if(flag3 == 2 && star == 2 && flag1 == 3){
            storing_data = 0;
        }   
    }
    // If the key pressed is MK_SW12, decrement flag1 and reset menu navigation variables
    if(key == MK_SW12){
        if(stop == 1){
        flag1--;// Decrement flag1 to move backward through the menu
        star = 1;// Reset star to 1 to reset the selection
        flag3 =1;// Reset flag3 to 1
        storing_data = 0;// Reset storing_data when moving back in the menu
        
        // If flag1 reaches 0, reset it to 1
        if(flag1 == 0){
            flag1 = 1;
            }
        }
    }
}
