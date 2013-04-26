/*
 * sd_utils.c
 *
 *  Created on: Apr 25, 2013
 *      Author: Samuel
 */
//TODO ADD CHECKS. THIS IS A "PUERCÁ'"

#include "pff2a/src/diskio.h"
#include "pff2a/src/pff.h"
#include "../common/spi/spi.h"
#include <string.h>
#include <stdlib.h>
#include "sd.h"
#include <msp430fr5969.h>
#include <stdio.h>
#include <stdbool.h>

FRESULT res;
FATFS fs; // File system object
WORD s1; // unsigned short

void init_sd(DWORD initial_seek) {
	P1DIR |= BIT3;
	SELECT_PASS_MODE();
	spiInit(SD_DEVICE);

	for(;;) {
		res = (FRESULT) disk_initialize();
		if (res == FR_OK) {
			break;
		}
	}

	while(pf_mount(&fs) != FR_OK);
	while(pf_open("WRITE.TXT") != FR_OK);
	while(pf_lseek(initial_seek) != FR_OK);
	return;
}

DWORD write_sd(char *string) {
	//TODO ADD CHECKS
	WORD bytes_written;

	res = pf_write(string, strlen(string), &bytes_written);
	if (res != FR_OK) {
		//break;
		_nop();
	}

	return bytes_written;
}

DWORD finalize_write_sd(void) {
	WORD bytes_written;
	res = pf_write(0, 0, &bytes_written); // finalize the write process
	if (res != FR_OK) {
		// error
		_nop();
	}
	return bytes_written;
}

void unmount_file_sd(void) {
	while(pf_mount(NULL) != FR_OK);
	return;
}

//TODO ADD CHECKS
bool fillbuffer(char *buffer, char *buffer2, int *arr, bool final_array) {
	unsigned int buffer_length = strlen(buffer);
	unsigned int buffer2_length;
	unsigned int i, j, left;

	sprintf(buffer2, "%d, %d, %d%s", arr[0], arr[1], arr[2], final_array ? "\r\n" : "\t");
	buffer2_length = strlen(buffer2);


	if(buffer_length + buffer2_length > 512) {
		// we are over, pass whatever you can to buffer1 and then rotate buff2
		for(i = 0; buffer_length < 512; i++, buffer_length++) {
			buffer[buffer_length] = buffer2[i];
		}

		left = buffer2_length - i;
		for (j = 0; j < left; j++, i++) {
			buffer2[j] = buffer2[i];
		}
		buffer2[j] = '\0';
		return true;
	}
	else {
		// we are ok, just pass buffer2 to buffer1
		for(i = 0; i < buffer2_length; i++) {
			buffer[buffer_length + i] = buffer2[i];
		}
		buffer[buffer_length + i] = '\0';
		return false;
	}
}

void dump_sd(char *buffer, char *buffer2, int sector_count) {
	init_sd(sector_count * 512); // make the seek
	spi_select(SD_DEVICE);
	write_sd(buffer);
	finalize_write_sd();
	unmount_file_sd();
	spi_deselect(SD_DEVICE);
	unsigned int i;
	for(i = 0; buffer2[i] != '\0'; i++) {
		buffer[i] = buffer2[i];
	}
	buffer[i] = '\0';
	buffer2[0] = '\0';
}
