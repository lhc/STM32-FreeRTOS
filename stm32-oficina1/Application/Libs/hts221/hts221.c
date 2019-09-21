/**
 ******************************************************************************
 * @file    hts221.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    14-February-2017
 * @brief   This file provides a set of functions needed to manage the HTS221
 *          humidity and temperature devices
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "hts221.h"

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static I2C_HandleTypeDef *pI2C_HTS221 = 0;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/**
 * @brief  Writes a single data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @param  Value: Data to be written
 * @retval None
 */
static void HTS221_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

/**
 * @brief  Reads a single data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @retval Data to be read
 */
static uint8_t HTS221_IO_Read(uint8_t Addr, uint8_t Reg);

/**
 * @brief  Reads multiple data with I2C communication
 *         channel from TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval HAL status
 */
static uint16_t HTS221_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);

//==============================================================================
// PRIVATE SOURCE CODE
//==============================================================================

static uint8_t HTS221_IO_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t read_value = 0;
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_HTS221, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &read_value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occured */
		DBG("ERRO I2C");
	}

	return read_value;
}

static void HTS221_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(pI2C_HTS221, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		DBG("ERRO I2C");
	}
}

static uint16_t HTS221_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_HTS221, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occured */
		DBG("ERRO I2C");
	}

	return status;
}

//==============================================================================
// PUBLIC SOURCE CODE
//==============================================================================

void HTS221_attach(I2C_HandleTypeDef *hI2Handler)
{
	pI2C_HTS221 = hI2Handler;
}

void HTS221_Init(uint16_t DeviceAddr)
{
	uint8_t tmp;

	/* Read CTRL_REG1 */
	tmp = HTS221_IO_Read(DeviceAddr, HTS221_CTRL_REG1);

	/* Enable BDU */
	tmp &= ~HTS221_BDU_MASK;
	tmp |= (1 << HTS221_BDU_BIT);

	/* Set default ODR */
	tmp &= ~HTS221_ODR_MASK;
	tmp |= (uint8_t) 0x01; /* Set ODR to 1Hz */

	/* Activate the device */
	tmp |= HTS221_PD_MASK;

	/* Apply settings to CTRL_REG1 */
	HTS221_IO_Write(DeviceAddr, HTS221_CTRL_REG1, tmp);
}

uint8_t HTS221_H_ReadID(uint16_t DeviceAddr)
{
	uint8_t ctrl = 0x00;

	/* Read value at Who am I register address */
	ctrl = HTS221_IO_Read(DeviceAddr, HTS221_WHO_AM_I_REG);

	return ctrl;
}

float HTS221_H_ReadHumidity(uint16_t DeviceAddr)
{
	int16_t H0_T0_out, H1_T0_out, H_T_out;
	int16_t H0_rh, H1_rh;
	uint8_t buffer[2];
	float tmp_f;

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_H0_RH_X2 | 0x80), buffer, 2);

	H0_rh = buffer[0] >> 1;
	H1_rh = buffer[1] >> 1;

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_H0_T0_OUT_L | 0x80), buffer, 2);

	H0_T0_out = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_H1_T0_OUT_L | 0x80), buffer, 2);

	H1_T0_out = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_HR_OUT_L_REG | 0x80), buffer, 2);

	H_T_out = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];

	tmp_f = (float) (H_T_out - H0_T0_out) * (float) (H1_rh - H0_rh) / (float) (H1_T0_out - H0_T0_out) + H0_rh;

	tmp_f *= 10.0f;

	tmp_f = (tmp_f > 1000.0f) ? 1000.0f : (tmp_f < 0.0f) ? 0.0f : tmp_f;

	return (tmp_f / 10.0f);
}

float HTS221_T_ReadTemp(uint16_t DeviceAddr)
{
	int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
	int16_t T0_degC, T1_degC;
	uint8_t buffer[4], tmp;
	float tmp_f;

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_T0_DEGC_X8 | 0x80), buffer, 2);
	tmp = HTS221_IO_Read(DeviceAddr, HTS221_T0_T1_DEGC_H2);

	T0_degC_x8_u16 = (((uint16_t) (tmp & 0x03)) << 8) | ((uint16_t) buffer[0]);
	T1_degC_x8_u16 = (((uint16_t) (tmp & 0x0C)) << 6) | ((uint16_t) buffer[1]);
	T0_degC = T0_degC_x8_u16 >> 3;
	T1_degC = T1_degC_x8_u16 >> 3;

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_T0_OUT_L | 0x80), buffer, 4);

	T0_out = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	T1_out = (((uint16_t) buffer[3]) << 8) | (uint16_t) buffer[2];

	HTS221_IO_ReadMultiple(DeviceAddr, (HTS221_TEMP_OUT_L_REG | 0x80), buffer, 2);

	T_out = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];

	tmp_f = (float) (T_out - T0_out) * (float) (T1_degC - T0_degC) / (float) (T1_out - T0_out) + T0_degC;

	return tmp_f;
}
