/**
 * @file    app_wfi.c
 * @author  Jorge Guzman
 * @date    May 25, 2019
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa o modulo wifi
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "sensores.h"
#include "setup_hw.h"

#include "hts221/hts221.h"
#include "lps22hb/lps22hb.h"
#include "lsm6dsl/lsm6dsl.h"
#include "lis3mdl/lis3mdl.h"

#include <stdio.h>
#include <string.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// SOURCE CODE
//==============================================================================

void Sensores_Read(Sensors_t *sensors)
{
	sensors->HTS221_temp =  HTS221_T_ReadTemp(HTS221_I2C_ADDRESS);
	sensors->HTS221_humidity = HTS221_H_ReadHumidity(HTS221_I2C_ADDRESS);

	sensors->LPS22HB_pressure = LPS22HB_P_ReadPressure(LPS22HB_I2C_ADDRESS);
	sensors->LPS22HB_temp = LPS22HB_T_ReadTemp(LPS22HB_I2C_ADDRESS);

	LSM6DSL_GyroReadXYZAngRate(sensors->LSM6DL_GyroDataXYXZ);
	LSM6DSL_AccReadXYZ(sensors->LSM6DL_Acce);

	LIS3MDL_MagReadXYZ(sensors->LIS3ML_MagXYZ);
}

void Sensor_Print(Sensors_t *sensors)
{
	DBG("1-TEMPERATURE = %.2f C", sensors->HTS221_temp);
	DBG("1-HUMIDITY = %2.f %%", sensors->HTS221_humidity);
	DBG("2-Pressao: %.2f mBar", sensors->HTS221_humidity);
	DBG("2-Tempetarura: %.2f C", sensors->LPS22HB_temp );
	DBG("3-GYRO_X = %.2f", sensors->LSM6DL_GyroDataXYXZ[0]);
	DBG("3-GYRO_Y = %.2f", sensors->LSM6DL_GyroDataXYXZ[1]);
	DBG("3-GYRO_Z = %.2f",sensors->LSM6DL_GyroDataXYXZ[2]);
	DBG("3-ACCELERO_X = %d", sensors->LSM6DL_Acce[0]);
	DBG("3-ACCELERO_Y = %d", sensors->LSM6DL_Acce[1]);
	DBG("3-ACCELERO_Z = %d", sensors->LSM6DL_Acce[2]);
	DBG("4-MAGNETO_X = %d", sensors->LIS3ML_MagXYZ[0]);
	DBG("4-MAGNETO_Y = %d", sensors->LIS3ML_MagXYZ[1]);
	DBG("4-MAGNETO_Z = %d", sensors->LIS3ML_MagXYZ[2]);
}

void Sensor_Print_SerialPlot(Sensors_t *sensors) // total 13 signals
{
	DBG("%.2f,%.2f,"\
			"%.2f,%.2f,"\
			"%.2f,%.2f,%.2f,"\
			"%d,%d,%d,"\
			"%d,%d,%d",
			sensors->HTS221_temp,            sensors->HTS221_humidity,
			sensors->HTS221_humidity,        sensors->LPS22HB_temp,
			sensors->LSM6DL_GyroDataXYXZ[0], sensors->LSM6DL_GyroDataXYXZ[1], sensors->LSM6DL_GyroDataXYXZ[2],
			sensors->LSM6DL_Acce[0],         sensors->LSM6DL_Acce[1],         sensors->LSM6DL_Acce[2],
			sensors->LIS3ML_MagXYZ[0],       sensors->LIS3ML_MagXYZ[1],       sensors->LIS3ML_MagXYZ[2]);
}

/**
 * @brief  Test of LPS22HB pressure sensor.
 */
void Pressure_Test(Sensors_t *sensors)
{
	sensors->LPS22HB_pressure = LPS22HB_P_ReadPressure(LPS22HB_I2C_ADDRESS);
	sensors->LPS22HB_temp = LPS22HB_T_ReadTemp(LPS22HB_I2C_ADDRESS);

	DBG("2-Pressao: %.2f mBar", sensors->HTS221_humidity);
	DBG("2-Tempetarura: %.2f C", sensors->LPS22HB_temp );
}

/**
 * @brief  Test of HTS221 humidity sensor.
 */
void Humidity_Test(Sensors_t *sensors)
{
	DBG("1-HUMIDITY = %2.f %%", sensors->HTS221_humidity);
	sensors->HTS221_humidity = HTS221_H_ReadHumidity(HTS221_I2C_ADDRESS);
}

/**
 * @brief  Test of HTS221 and LPS22HB temperature sensors.
 */
void Temperature_Test(Sensors_t *sensors)
{
	sensors->HTS221_temp =  HTS221_T_ReadTemp(HTS221_I2C_ADDRESS);
	DBG("1-TEMPERATURE = %.2f C", sensors->HTS221_temp);
}

/**
 * @brief  Test of LSM6DSL accelerometer sensor.
 */
void Accelero_Test(Sensors_t *sensors)
{
	LSM6DSL_AccReadXYZ(sensors->LSM6DL_Acce);
	DBG("3-ACCELERO_X = %d", sensors->LSM6DL_Acce[0]);
	DBG("3-ACCELERO_Y = %d", sensors->LSM6DL_Acce[1]);
	DBG("3-ACCELERO_Z = %d", sensors->LSM6DL_Acce[2]);
}

/**
 * @brief  Test of LIS3MDL gyroscope sensor.
 */
void Gyro_Test(Sensors_t *sensors)
{
	LSM6DSL_GyroReadXYZAngRate(sensors->LSM6DL_GyroDataXYXZ);
	DBG("3-GYRO_X = %.2f", sensors->LSM6DL_GyroDataXYXZ[0]);
	DBG("3-GYRO_Y = %.2f", sensors->LSM6DL_GyroDataXYXZ[1]);
	DBG("3-GYRO_Z = %.2f",sensors->LSM6DL_GyroDataXYXZ[2]);
}

/**
 * @brief  Test of LIS3MDL magnetometer sensor.
 */
void Magneto_Test(Sensors_t *sensors)
{
	LIS3MDL_MagReadXYZ(sensors->LIS3ML_MagXYZ);

	DBG("3-MAGNETO_X = %d", sensors->LIS3ML_MagXYZ[0]);
	DBG("3-MAGNETO_Y = %d", sensors->LIS3ML_MagXYZ[1]);
	DBG("3-MAGNETO_Z = %d", sensors->LIS3ML_MagXYZ[2]);
}
