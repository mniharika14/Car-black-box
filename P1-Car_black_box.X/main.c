
/*
 * Microcontoller Project - Car black box
 * File:   main.c
 * Name: Niharika
 * Created on 11 November, 2024, 3:01 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "dash_board.h"
#include "adc.h"
#include "store_data.h"
#include "main_menu.h"
#include "external_eeprom.h"
#include "i2c.h"
#include "rtc.h"
#include "ds1307.h"
#include "uart.h"

// Global variables
int data_lim, count = 0, flag1 = 1, flag2 = 0, flag3 = 1, star = 1, flag4 = 1, clear = 0, storing_data, clearing_log , set, hours, min, sec, stop;
unsigned char clock_reg[3], time[9];
static void init_config(void) {
    // Initialize peripherals and modules
    init_clcd();
    init_adc();
    init_matrix_keypad();
    init_i2c();
    init_ds1307();
    init_uart();
}

void main(void) {
    unsigned char key;

    init_config();
    char store[10], gear[9][3] = {"ON", "GN", "GR", "G1", "G2", "G3", "G4", "G5", "C "};
    int adc;
    unsigned int adc_reg_val;
    unsigned char write[10];
    static int stop_download = 1;
    while (1) {
        static int delay;
        static int blink = 1;
        get_time();// Read the current time
        adc_reg_val = read_adc(CHANNEL4);// Read ADC value and scale to 0-99
        adc = (adc_reg_val) / (10.23);
        if (adc > 99) {
            adc = 99;
        }

        key = read_switches(STATE_CHANGE); //read the switch
        // Handle dashboard display and main menu navigation
        dash_board(key);
        main_menu(key);

        if (flag1 == 1) {
            // Display current time, event, speed, and gear on the LCD
            CLEAR_DISP_SCREEN;
            clcd_print("TIME", LINE1(2));
            clcd_print("EV", LINE1(10));
            clcd_print("SP", LINE1(14));
            clcd_putch(time[0], LINE2(0));
            clcd_putch(time[1], LINE2(1));
            clcd_putch(time[2], LINE2(2));
            clcd_putch(time[3], LINE2(3));
            clcd_putch(time[4], LINE2(4));
            clcd_putch(time[5], LINE2(5));
            clcd_putch(time[6], LINE2(6));
            clcd_putch(time[7], LINE2(7));
            clcd_print(gear[count], LINE2(10));
            clcd_putch(adc / 10 + '0', LINE2(14));
            clcd_putch(adc % 10 + '0', LINE2(15));
            
            // Parse hours, minutes, and seconds from the time array
            hours = (time[0] - 48)*10   + time[1] - 48;
            min = (time[3] - 48)*10 + time[4] - 48;
            sec = (time[6] - 48)*10 + time[7] - 48;
        }

        if (flag2 == 1) {
            // Store current log data to EEPROM
            flag2 = 0;
            write[0] = time[0];
            write[1] = time[1];
            write[2] = time[3];
            write[3] = time[4];
            write[4] = time[6];
            write[5] = time[7];
            write[6] = gear[count][0];
            write[7] = gear[count][1];
            write[8] = adc / 10 + '0';
            write[9] = adc % 10 + '0';
            write[10] = '\0';
            store_data(write);
        }
        if (flag1 == 2) {
            // Main menu navigation logic
            CLEAR_DISP_SCREEN;
            if (star == 1) {
                clcd_putch('*', LINE1(0));
            }
            if (star == 2) {
                clcd_putch('*', LINE2(0));
            }
            // Display menu options based on flag3
            if (flag3 == 1) {
                clcd_print("VIEW LOG", LINE1(1));
                clcd_print("SET TIME", LINE2(1));
            }

            if (flag3 == 2) {
                clcd_print("SET TIME", LINE1(1));
                clcd_print("DOWNLOAD LOG", LINE2(1));
            }
            if (flag3 == 3) {
                clcd_print("DOWNLOAD LOG", LINE1(1));
                clcd_print("CLEAR LOG", LINE2(1));
            }
        }
        if (flag1 == 3) {
            // Handle different sub-menu actions
            CLEAR_DISP_SCREEN;
            if (flag3 == 1 && star == 1) {
                if (clear == 0) {
                    clcd_print("NO LOGS", LINE1(1));
                    clcd_print("TO DISPLAY :(", LINE2(1));
                    stop = 0;
                    if(delay++ > 400){
                    flag1 = 2;
                    stop = 1;
                    delay = 0;
                    }
                } else {
                    int j = 0;
                    for (int i = storing_data; i < storing_data + 10; i++) {
                        store[j++] = read_external_eeprom(i);
                    }
                    clcd_print("#   VIEW LOG:", LINE1(0));
                    store[10] = '\0';
                    clcd_putch(((storing_data/10) + '0'), LINE2(0));
                    clcd_putch(store[0], LINE2(2));
                    clcd_putch(store[1], LINE2(3));
                    clcd_putch(':', LINE2(4));
                    clcd_putch(store[2], LINE2(5));
                    clcd_putch(store[3], LINE2(6));
                    clcd_putch(':', LINE2(7));
                    clcd_putch(store[4], LINE2(8));
                    clcd_putch(store[5], LINE2(9));
                    clcd_putch(store[6], LINE2(11));
                    clcd_putch(store[7], LINE2(12));
                    clcd_putch(store[8], LINE2(14));
                    clcd_putch(store[9], LINE2(15));
                    clcd_putch(store[10], LINE2(16));
                }
            }
            // Handle set time
            if ((flag3 == 2 && star == 1) || (flag3 == 1 && star == 2)) {
                if(delay++ > 150){//delay for blinking
                    delay = 0;
                    blink = !blink;// Toggle the blink flag
                }
                clcd_print("HH:MM:SS", LINE1(1));// Display the time format
                
                // Display and blink the hour digits
                if(set == 1)
                {
                    if(blink)
                    {
                        clcd_putch((hours/10) + '0', LINE2(1));// Display the tens digit of hours
                        clcd_putch((hours%10) + '0', LINE2(2));// Display the units digit of hours
                        
                    }else{
                        clcd_putch(' ', LINE2(1));// Clear tens digit during blink
                        clcd_putch(' ', LINE2(2));// Clear units digit during blink
                    }
                    clcd_putch(':', LINE2(3));// Display colon separator
                    clcd_putch((min/10) + '0', LINE2(4));// Display minutes
                    clcd_putch((min%10) + '0', LINE2(5));
                    clcd_putch(':', LINE2(6));// Display colon separator
                    clcd_putch((sec/10) + '0', LINE2(7));// Display seconds
                    clcd_putch((sec%10) + '0', LINE2(8));
                } 
                // Display and blink the minute digits
                if(set == 2){
                    clcd_putch((hours/10) + '0', LINE2(1));// Display the hours normally
                    clcd_putch((hours%10) + '0', LINE2(2));
                    clcd_putch(':', LINE2(3));
                    if(blink){
                        clcd_putch((min/10) + '0', LINE2(4));// Display the tens digit of minute
                        clcd_putch((min%10) + '0', LINE2(5));// Display the units digit of minutes
                    }else{
                        clcd_putch(' ', LINE2(4));// Clear tens digit during blink
                        clcd_putch(' ', LINE2(5));// Clear units digit during blink
                    }
                    clcd_putch(':', LINE2(6));// Display seconds normally
                    clcd_putch((sec/10) + '0', LINE2(7));
                    clcd_putch((sec%10) + '0', LINE2(8));
                }
                // Display and blink the second digits
                if(set == 3){
                    clcd_putch((hours/10) + '0', LINE2(1));// Display hours normally
                    clcd_putch((hours%10) + '0', LINE2(2));
                    clcd_putch(':', LINE2(3));
                    clcd_putch((min/10) + '0', LINE2(4));// Display minutes normally
                    clcd_putch((min%10) + '0', LINE2(5));
                    clcd_putch(':', LINE2(6));
                    if(blink){
                        clcd_putch((sec/10) + '0', LINE2(7));// Display the tens digit of seconds
                        clcd_putch((sec%10) + '0', LINE2(8));// Display the units digit of seconds
                    }else{
                        clcd_putch(' ', LINE2(7));// Clear tens digit during blink
                        clcd_putch(' ', LINE2(8));// Clear units digit during blink
                    } 
                }
            }
            // Handle downloading logs
            if((flag3 == 2 && star == 2)||(flag3 == 3 &&  star == 1)){
                if(stop_download){
                char download[10][18];// Array to store log data for download
                int a=0;// Counter for log entries
                int b=0;// EEPROM address counter
                
                // Read data from EEPROM and store it in the download array
                for( int i=0;i<(data_lim/10);i++){
                    for(int j=0;j<17;j++){
                        if(j==1 || j==2 || j==11 || j==14){
                            download[i][j] = ' ';// Insert spaces at specific positions for formatting
                        }else if(j==5 || j==8){
                            download[i][j] = ':';// Insert colons for time formatting
                        }else if(j==0){
                            download[i][j]='0' + a;// Add entry number at the beginning of each log
                            a++;// Increment log entry number
                        }else{
                            download[i][j]= read_external_eeprom(b++);// Read data from EEPROM and store in download array
                        }
                    }
                    download[i][17] = '\0';// Null-terminate each log entry string
                }
                // Print downloaded logs if clear flag is set
                if(clear){
                    char download1[18]= "NO.  TIME   EV SP";
                    puts(download1);// Print header
                    puts("\n\r");// New line
                    
                    // Print each log entry
                    for(int i=0;i<(data_lim/10);i++){
                        puts(download[i]);// Print each log entry
                        puts("\n\r");// New line after each entry
                    }
                }
                stop_download = 0;
            }
                clcd_print("DOWNLOADING ", LINE1(0));// Display downloading status on LCD
                clcd_print("LOGS...", LINE2(8));
                stop = 0;
                if(delay++ > 400){//delay for download log 
                flag1 = 2;
                flag3 = 1;
                star = 1;
                delay = 0;
                stop = 1;
                stop_download = 1;
                }
            }
            // Handle clearing logs
            if (flag3 == 3 && star == 2){
                clear = 0;// Reset the clear flag
                clcd_print("CLEARING LOGS", LINE1(1));
                clcd_print("PLEASE WAIT...", LINE2(1));
                stop = 0;
                if(delay++ > 400){//delay to clear the log
                flag1 = 2;// Return to menu
                flag3 =1;
                star = 1;
                delay = 0;
                clearing_log = 1;// Set clearing log flag
                store_data(write);
                clearing_log = 0;// Clear log in EEPROM
                stop = 1;
                }
            }
        }
    }
} 