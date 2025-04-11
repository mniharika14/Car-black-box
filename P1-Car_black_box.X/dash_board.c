/*
 * File:   dash_board.c
 * Author: Niharika
 *
 * Created on 11 November, 2024, 3:01 PM
 */


#include <xc.h>
#include "matrix_keypad.h"

int data_lim;
extern int count, flag1, flag2, flag3, star, start_dec, clear, storing_data = 0, hours, min, sec, set=1;
void dash_board(unsigned char key) {
    static int start_dec = 0, flag = 1;
    // Increment gear or adjust settings based on key press
    if(key == MK_SW1 && flag1 == 1){
        clear = 1;//clear log should not be cleared
        start_dec = 1;//decrement is allowed for the dashboard
        
        if(flag){  
            count++;//increment gear
            if(count<=7){
                flag2 =1;// Indicate that data should be stored
            }
            if(count >= 8){
                count = 7;// Limit gear to a maximum of 7
            }
        }else{
            flag2 = 1;// Store data
            flag=1;// Reset flag for normal operation
            count = 1; // Reset to the first gear
        }
    }else if(key == MK_SW1 && flag1 == 3){
        if(star == 1 && flag3 == 1){
            storing_data -= 10;// Move back 10 entries in the log
            if(storing_data <= 0){
                storing_data = 0;// Ensure no negative log index
                }
            }
         // Increment time fields like hours, minutes, seconds
        if((flag3 == 2 && star == 1) || (flag3 == 1 && star == 2)){
            if(set == 1){
                hours++;
                if(hours >= 24){
                    hours = 0;// Reset hours after 23
                }
            }
            if(set == 2){
                min++;
                if(min >= 60){
                    min = 0;// Reset minutes after 59
                }
            }
            if(set == 3){
                sec++;
                if(sec >= 60){
                    sec = 0;// Reset seconds after 59
                }
            }
        }
    }
    // Navigate the main menu
    if(key == MK_SW1 && flag1 == 2){
        star--;// Move the selection up
        if(star == 0){
            flag3--;// Move to the previous menu option
            star = 1;// Wrap selection to the bottom
        }

        if(flag3 == 0){
            flag3 = 1;// if flag 3 counts beyond 0 set flag to 1
        }
    }
    // Handle decrement or reverse actions
    if(start_dec == 1 || flag1 == 2 || flag1 == 3){
        if(key == MK_SW2 && flag1 == 1){
            // Decrement gear
            if(flag){
                count--;
                if(count>=0){
                    flag2 = 1;// Indicate data change
                }
                if(count == 0){
                    count = 1;// Ensure gear does not go below 1
                }
            }else{
                flag2 = 1;// Store data
                flag=1;// Reset to normal operation
                count = 1;// Reset to the first gear
            }
        }
        else if(key == MK_SW2 && flag1 == 3){
            // Navigate logs or modify time settings
            if(star == 1 && flag3 == 1){
                storing_data = storing_data + 10;// Move forward 10 entries in the log
                if(storing_data >= data_lim){
                    storing_data = storing_data - 10;// Prevent exceeding log limit
                    }
                }
            if((flag3 == 2 && star == 1) || (flag3 == 1 && star == 2)){
                set++;// Move to the next time field
                if(set >= 4){
                    set = 1;// Wrap back to the first time field
                }
            }
        }else if(key == MK_SW2 && flag1 == 2){
            // Move the menu selection down
            star++;
            if(star == 3){
                flag3++;// Move to the next menu option
                star = 2;// Wrap selection to the top
            }
            if(flag3 == 4){
                flag3 = 3;// Wrap menu option to the last
            }
        }
    }
    // Handle special action to select the highest gear
    if(key == MK_SW3 && flag1 == 1){
        flag2 = 1;// Indicate data change
        count = 8;// Set gear to the highest value
        flag = 0; // Indicate special mode
        start_dec = 1;// Allow decrement
    } 
}
