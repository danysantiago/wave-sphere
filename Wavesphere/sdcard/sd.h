/*
 * sd.h
 *
 *  Created on: Apr 25, 2013
 *      Author: Samuel
 */

#ifndef SD_H_
#define SD_H_
#include <stdbool.h>
#include "pff2a/src/pff.h"
void init_sd(DWORD initial_seek, FATFS *fs);
DWORD write_sd(char *string);
DWORD finalize_write_sd(void);
DWORD write_sensor_data_sd(int *arr);
void unmount_file_sd(void);
void dump_sd(char *buffer, char *buffer2, int sector_count);
bool fillbuffer(char *buffer, char *buffer2, int *arr, unsigned long timestamp, bool final_array, bool writetimestamp);

#define SELECT_PASS_MODE()	P1OUT &= ~BIT3
#define DESELECT_PASS_MODE()	P1OUT |= BIT3
#define TOGGLE_PASS_MODE()	P1OUT ^= BIT3

#endif /* SD_H_ */
