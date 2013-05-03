/*
 * main.h
 *
 *  Created on: Mar 28, 2013
 *      Author: Samuel
 */

#ifndef _MAIN_H
#define _MAIN_H
#include <stdbool.h>

/**
 * Flow Services
 */
void diagnostic_service(void);
void retrieval_service(void);
void sampling_service(bool diagnostic);
void location_service(void);
void status_service(void);

/**
 * XBee Mode Codes
 */

/**
 * System Mode Constants
 */
#define M_DIAGNOSTIC_CLEAR 0
#define M_DIAGNOSTIC_SET 1
#define M_RETRIEVAL_SET 2
#define M_SAMPLING_SET 3
#define M_SAMPLING_CLEAR 4
#define M_SHUTDOWN_SET 5
#define M_LOCATE_SET 6

/**
 * Flags for System Flow
 */
struct SYSTEM_FLAG {
	bool diagnostic_flag;
	bool retrieval_flag;
	bool sampling_flag;
	bool shutdown_flag;
	bool locate_flag;
};
extern volatile struct SYSTEM_FLAG system_flags;
extern const char id[9];

// Function declarations
void setup_rfwakeup(void);
void shutdown_xbee(void);
void shutdown_gps(void);
void wakeup_gps(void);
void initialize_xbee(void);


#endif
