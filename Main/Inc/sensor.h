/*
 * sensor.h
 *
 *  Created on: 2026. 6. 24.
 *      Author: kth59
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

void Sensor_Start(void);
void Sensor_Stop(void);

void Sensor_Calibration(void);
void Sensor_Test_Raw(void);
void Sensor_Test_Normalized(void);
void Sensor_Test_State(void);



#endif /* INC_SENSOR_H_ */
