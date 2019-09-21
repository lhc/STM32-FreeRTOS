/**
 ******************************************************************************
 * @file    lis3mdl.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    14-February-2017
 * @brief   This file provides a set of functions needed to manage the LIS3MDL
 *          magnetometer devices
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================
#include "lis3mdl.h"

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static I2C_HandleTypeDef *pI2C_LIS3MDL = 0;

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
static void LIS3MDL_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

/**
 * @brief  Reads a single data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @retval Data to be read
 */
static uint8_t LIS3MDL_IO_Read(uint8_t Addr, uint8_t Reg);

/**
 * @brief  Reads multiple data with I2C communication
 *         channel from TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval HAL status
 */
static uint16_t LIS3MDL_IO_ReadMultiple(uint8_t Addr, uint8_t Reg,
		uint8_t *Buffer, uint16_t Length);

//==============================================================================
// PRIVATE SOURCE CODE
//==============================================================================

static void LIS3MDL_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(pI2C_LIS3MDL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		DBG("ERRO I2C");
	}
}

static uint8_t LIS3MDL_IO_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t read_value = 0;
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_LIS3MDL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &read_value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occured */
		DBG("ERRO I2C");
	}

	return read_value;
}

static uint16_t LIS3MDL_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_LIS3MDL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length, 1000);

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

void LIS3MDL_myInit(void)
{
	if(pI2C_LIS3MDL != NULL)
	{
		MAGNETO_Init_t LIS3MDL_InitStructureMag;

		/* MEMS configuration ------------------------------------------------------*/
		/* Fill the MAGNETO magnetometer structure */
		LIS3MDL_InitStructureMag.Register1 = LIS3MDL_MAG_TEMPSENSOR_DISABLE | LIS3MDL_MAG_OM_XY_HIGH | LIS3MDL_MAG_ODR_40_HZ;
		LIS3MDL_InitStructureMag.Register2 = LIS3MDL_MAG_FS_4_GA | LIS3MDL_MAG_REBOOT_DEFAULT | LIS3MDL_MAG_SOFT_RESET_DEFAULT;
		LIS3MDL_InitStructureMag.Register3 = LIS3MDL_MAG_CONFIG_NORMAL_MODE | LIS3MDL_MAG_CONTINUOUS_MODE;
		LIS3MDL_InitStructureMag.Register4 = LIS3MDL_MAG_OM_Z_HIGH | LIS3MDL_MAG_BLE_LSB;
		LIS3MDL_InitStructureMag.Register5 = LIS3MDL_MAG_BDU_MSBLSB;

		LIS3MDL_MagInit(LIS3MDL_InitStructureMag);
	}
}
void LIS3MDL_attach(I2C_HandleTypeDef *hI2Handler)
{
	pI2C_LIS3MDL = hI2Handler;
}

void LIS3MDL_MagInit(MAGNETO_Init_t LIS3MDL_InitStruct)
{
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG1, LIS3MDL_InitStruct.Register1);
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG2, LIS3MDL_InitStruct.Register2);
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG3, LIS3MDL_InitStruct.Register3);
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG4, LIS3MDL_InitStruct.Register4);
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG5, LIS3MDL_InitStruct.Register5);
}

void LIS3MDL_MagDeInit(void)
{
	uint8_t ctrl = 0x00;

	/* Read control register 1 value */
	ctrl = LIS3MDL_IO_Read(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG3);

	/* Clear Selection Mode bits */
	ctrl &= ~(LIS3MDL_MAG_SELECTION_MODE);

	/* Set Power down */
	ctrl |= LIS3MDL_MAG_POWERDOWN2_MODE;

	/* write back control register */
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG3, ctrl);
}

uint8_t LIS3MDL_MagReadID(void)
{
	/* Read value at Who am I register address */
	return (LIS3MDL_IO_Read(LIS3MDL_MAG_I2C_ADDRESS_HIGH,
			LIS3MDL_MAG_WHO_AM_I_REG));
}

void LIS3MDL_MagLowPower(uint16_t status)
{
	uint8_t ctrl = 0;

	/* Read control register 1 value */
	ctrl = LIS3MDL_IO_Read(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG3);

	/* Clear Low Power Mode bit */
	ctrl &= ~(0x20);

	/* Set Low Power Mode */
	if (status)
	{
		ctrl |= LIS3MDL_MAG_CONFIG_LOWPOWER_MODE;
	}
	else
	{
		ctrl |= LIS3MDL_MAG_CONFIG_NORMAL_MODE;
	}

	/* write back control register */
	LIS3MDL_IO_Write(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG3, ctrl);
}

void LIS3MDL_MagReadXYZ(int16_t* pData)
{
	int16_t pnRawData[3];
	uint8_t ctrlm = 0;
	uint8_t buffer[6];
	uint8_t i = 0;
	float sensitivity = 0;

	/* Read the magnetometer control register content */
	ctrlm = LIS3MDL_IO_Read(LIS3MDL_MAG_I2C_ADDRESS_HIGH, LIS3MDL_MAG_CTRL_REG2);

	/* Read output register X, Y & Z acceleration */
	LIS3MDL_IO_ReadMultiple(LIS3MDL_MAG_I2C_ADDRESS_HIGH, (LIS3MDL_MAG_OUTX_L | 0x80), buffer, 6);

	for (i = 0; i < 3; i++)
	{
		pnRawData[i] = ((((uint16_t) buffer[2 * i + 1]) << 8) + (uint16_t) buffer[2 * i]);
	}

	/* Normal mode */
	/* Switch the sensitivity value set in the CRTL_REG2 */
	switch (ctrlm & 0x60)
	{
	case LIS3MDL_MAG_FS_4_GA:
		sensitivity = LIS3MDL_MAG_SENSITIVITY_FOR_FS_4GA;
		break;

	case LIS3MDL_MAG_FS_8_GA:
		sensitivity = LIS3MDL_MAG_SENSITIVITY_FOR_FS_8GA;
		break;

	case LIS3MDL_MAG_FS_12_GA:
		sensitivity = LIS3MDL_MAG_SENSITIVITY_FOR_FS_12GA;
		break;

	case LIS3MDL_MAG_FS_16_GA:
		sensitivity = LIS3MDL_MAG_SENSITIVITY_FOR_FS_16GA;
		break;
	}

	/* Obtain the mGauss value for the three axis */
	for (i = 0; i < 3; i++)
	{
		pData[i] = (int16_t) (pnRawData[i] * sensitivity);
	}
}
