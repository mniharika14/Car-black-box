/*
 * File:   store_data.c
 * Author: Niharika
 *
 * Created on 14 November, 2024, 8:30 AM
 */


#include <xc.h>
#include "store_data.h"
#include "external_eeprom.h"

extern int data_lim = 0, clearing_log=0;

// Function to store data in external EEPROM
void store_data(unsigned char write[10]) {
    int shift_index = 10, clr = 0;
    // If clearing_log flag is set, clear the stored data in EEPROM
    if(clearing_log == 1){
        data_lim = 0;// Reset the data limit pointer
        for(clr=0;clr<100;clr++){
            write_external_eeprom(clr,0);// Write 0 to all EEPROM locations to clear data
        }
    }
    
    // If the data limit reaches 100, reset it and shift the data to make space
    if (data_lim == 100) {
        data_lim = 0;// Reset data limit pointer
        clearing_log = 2;
    }
        // Shift data in EEPROM from index 10 to 100-1 to make space for new data
        if(clearing_log == 2){
        for (int i = 0; i < 90; i++) {
            unsigned char temp = read_external_eeprom(shift_index++);// Read data from EEPROM
            write_external_eeprom(data_lim++, temp);// Write it back starting from the beginning
        }
        for (int i = 0; i < 10; i++) {
        write_external_eeprom(data_lim++, write[i]);// Write each byte from 'write' array to EEPROM
    }
}
    
    // Write the new data to the EEPROM starting from the current data limit
    if(clearing_log == 0){
        for (int i = 0; i < 10; i++) {
        write_external_eeprom(data_lim++, write[i]);// Write each byte from 'write' array to EEPROM
        }
    }
}