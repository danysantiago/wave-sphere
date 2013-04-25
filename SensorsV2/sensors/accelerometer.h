/*
 * Accelerometer.h
 *
 *  Created on: Mar 23, 2013
 *      Author: Adrian
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

void initAcc();
void getAccData(int *dataArr);
void accStartST();
void accEndST();
void accSleep();
void accWakeup();


#endif /* ACCELEROMETER_H_ */
