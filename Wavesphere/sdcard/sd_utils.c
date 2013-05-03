/*
 * sd_utils.c
 *
 *  Created on: Apr 25, 2013
 *      Author: Samuel
 */
//TODO ADD CHECKS. THIS IS A "PUERC�'"

#include "pff2a/src/diskio.h"
#include "pff2a/src/pff.h"
#include "../common/spi/spi.h"
#include <string.h>
#include <stdlib.h>
#include "sd.h"
#include <msp430fr5969.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

FRESULT res;
WORD s1; // unsigned short

void init_sd(DWORD initial_seek, FATFS *fs) {
	P1DIR |= BIT3;
	SELECT_PASS_MODE();
	spiInit(SD_DEVICE);

	for(;;) {
		res = (FRESULT) disk_initialize();
		if (res == FR_OK) {
			break;
		}
	}

	while(pf_mount(fs));
	while(pf_open("WRITE.TXT"));
//	while(pf_lseek(initial_seek) != FR_OK);
	return;
}

DWORD write_sd(char *string) {
	//TODO ADD CHECKS
	WORD bytes_written;
	int len = strlen(string);
	while(pf_write(string, len, &bytes_written)); // pig
//	if (res != FR_OK) {
//		//break;
//		_nop();
//	}

	return bytes_written;
}

DWORD finalize_write_sd(void) {
	WORD bytes_written;
	while(pf_write(0, 0, &bytes_written)); // finalize the write process
	if (res != FR_OK) {
		// error
		_nop();
	}
	return bytes_written;
}

void unmount_file_sd(void) {
	while(pf_mount(NULL));
	return;
}

//TODO ADD CHECKS
bool fillbuffer(char *buffer, char *buffer2, int *arr, unsigned long timestamp, bool final_array, bool writetimestamp) {
	unsigned int buffer_length = strlen(buffer);
	unsigned int buffer2_length;
	unsigned int i, j, left;
	char timestampString[13];

	ltoa(timestamp, timestampString);
	i = 0;
	while(timestampString[i++]);
	i--; // super puerco
	timestampString[i++] = '\t';
	timestampString[i++] = '\0';
	timestampString[i] = '\0';
	sprintf(buffer2, "%s%d, %d, %d%s", writetimestamp ? timestampString : "", arr[0], arr[1], arr[2], final_array ? "\r\n" : "\t");
	buffer2_length = strlen(buffer2);


	if(buffer_length + buffer2_length > 512) {
		// we are over, pass whatever you can to buffer1 and then rotate buff2
		for(i = 0; buffer_length < 512; i++, buffer_length++) {
			buffer[buffer_length] = buffer2[i];
		}

		//left = buffer2_length - i;
		j = 0;
		while(buffer2[j++] = buffer2[i++]);
		//for (j = 0; j < left; j++, i++) {
		//	buffer2[j] = buffer2[i];
		//}
		buffer2[j] = '\0';
		buffer[512] = '\0';
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
	//init_sd(sector_count * 512); // make the seek
	//spi_select(SD_DEVICE);
	write_sd(buffer);
	//finalize_write_sd();
	//unmount_file_sd();
	//spi_deselect(SD_DEVICE);
	unsigned int i;
	for(i = 0; buffer2[i] != '\0'; i++) {
		buffer[i] = buffer2[i];
	}
	buffer[i] = '\0';
	buffer2[0] = '\0';
}
