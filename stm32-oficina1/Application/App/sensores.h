/**
 * @file    app_wfi.c
 * @author  Jorge Guzman
 * @date    May 25, 2019
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa o modulo wifi
 */


#ifndef _APP_SENSORS_H_
#define _APP_SENSORS_H_

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include <stdint.h>

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

typedef struct
{
	float HTS221_temp;
	float HTS221_humidity;
	float LPS22HB_pressure;
	float LPS22HB_temp;
	float LSM6DL_GyroDataXYXZ[3];
	int16_t LSM6DL_Acce[3];
	int16_t LIS3ML_MagXYZ[3];
}Sensors_t;

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void Sensores_Read(Sensors_t *sensors);
void Sensor_Print(Sensors_t *sensors);
void Sensor_Print_SerialPlot(Sensors_t *sensors);

void Temperature_Test(Sensors_t *sensors);
void Humidity_Test(Sensors_t *sensors);
void Pressure_Test(Sensors_t *sensors);
void Gyro_Test(Sensors_t *sensors);
void Magneto_Test(Sensors_t *sensors);
void Accelero_Test(Sensors_t *sensors);


#endif /* _APP_SENSORS_H_ */
