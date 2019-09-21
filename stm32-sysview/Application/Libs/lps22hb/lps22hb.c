/**
 ******************************************************************************
 * @file    lps22hb.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    14-February-2017
 * @brief   This file provides a set of functions needed to manage the LPS22HB
 *          pressure and temperature devices
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "lps22hb.h"

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static I2C_HandleTypeDef *pI2C_LPS22HB = 0;

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
static void LPS22HB_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

/**
 * @brief  Reads a single data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @retval Data to be read
 */
static uint8_t LPS22HB_IO_Read(uint8_t Addr, uint8_t Reg);

//==============================================================================
// PUBLIC SOURCE CODE
//==============================================================================

static uint8_t LPS22HB_IO_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t read_value = 0;
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_LPS22HB, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &read_value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occured */
		DBG("ERRO I2C");
	}

	return read_value;
}

void LPS22HB_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(pI2C_LPS22HB, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		DBG("ERRO I2C");
	}
}

void LPS22HB_attach(I2C_HandleTypeDef *hI2Handler)
{
	pI2C_LPS22HB = hI2Handler;
}

void LPS22HB_P_Init(uint16_t DeviceAddr)
{
	LPS22HB_Init(DeviceAddr);
}

uint8_t LPS22HB_P_ReadID(uint16_t DeviceAddr)
{
	uint8_t ctrl = 0x00;

	/* Read value at Who am I register address */
	ctrl = LPS22HB_IO_Read(DeviceAddr, LPS22HB_WHO_AM_I_REG);

	return ctrl;
}

float LPS22HB_P_ReadPressure(uint16_t DeviceAddr)
{
	int32_t raw_press;
	uint8_t buffer[3];
	uint32_t tmp = 0;
	uint8_t i;

	for (i = 0; i < 3; i++)
	{
		buffer[i] = LPS22HB_IO_Read(DeviceAddr, (LPS22HB_PRESS_OUT_XL_REG + i));
	}

	/* Build the raw data */
	for (i = 0; i < 3; i++)
	{
		tmp |= (((uint32_t) buffer[i]) << (8 * i));
	}

	/* convert the 2's complement 24 bit to 2's complement 32 bit */
	if (tmp & 0x00800000)
	{
		tmp |= 0xFF000000;
	}

	raw_press = ((int32_t) tmp);

	raw_press = (raw_press * 100) / 4096;

	return (float) ((float) raw_press / 100.0f);
}

void LPS22HB_T_Init(uint16_t DeviceAddr)
{
	LPS22HB_Init(DeviceAddr);
}

float LPS22HB_T_ReadTemp(uint16_t DeviceAddr)
{
	int16_t raw_data;
	uint8_t buffer[2];
	uint16_t tmp;
	uint8_t i;

	for (i = 0; i < 2; i++)
	{
		buffer[i] = LPS22HB_IO_Read(DeviceAddr, (LPS22HB_TEMP_OUT_L_REG + i));
	}

	/* Build the raw tmp */
	tmp = (((uint16_t) buffer[1]) << 8) + (uint16_t) buffer[0];

	raw_data = (tmp * 10) / 100;

	return ((float) (raw_data / 10.0f));
}

void LPS22HB_Init(uint16_t DeviceAddr)
{
	uint8_t tmp;

	/* Set Power mode */
	tmp = LPS22HB_IO_Read(DeviceAddr, LPS22HB_RES_CONF_REG);

	tmp &= ~LPS22HB_LCEN_MASK;
	tmp |= (uint8_t) 0x01; /* Set low current mode */

	LPS22HB_IO_Write(DeviceAddr, LPS22HB_RES_CONF_REG, tmp);

	/* Read CTRL_REG1 */
	tmp = LPS22HB_IO_Read(DeviceAddr, LPS22HB_CTRL_REG1);

	/* Set default ODR */
	tmp &= ~LPS22HB_ODR_MASK;
	tmp |= (uint8_t) 0x30; /* Set ODR to 25Hz */

	/* Enable BDU */
	tmp &= ~LPS22HB_BDU_MASK;
	tmp |= ((uint8_t) 0x02);

	/* Apply settings to CTRL_REG1 */
	LPS22HB_IO_Write(DeviceAddr, LPS22HB_CTRL_REG1, tmp);
}
