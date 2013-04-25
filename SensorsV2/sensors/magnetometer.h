/*
 * magnetometer.h
 *
 *  Created on: Apr 12, 2013
 *      Author: Adrian
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#define SENS_250dps 1
#define SENS_500dps 2
#define SENS_2000dps 3

void initMag();
void initMagAcc();
void getMagData(int *dataArray);
void getMagAccData(int *dataArray);

#endif /* MAGNETOMETER_H_ */
