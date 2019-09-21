/**
 ******************************************************************************
 * @file    lsm6dsl.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    14-February-2017
 * @brief   This file provides a set of functions needed to manage the LSM6DSL
 *          accelero and gyro devices
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "lsm6dsl.h"

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static I2C_HandleTypeDef *pI2C_LSM6DSL = 0;

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
static void LSM6DSL_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

/**
 * @brief  Reads a single data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @retval Data to be read
 */
static uint8_t LSM6DSL_IO_Read(uint8_t Addr, uint8_t Reg);

/**
 * @brief  Reads multiple data with I2C communication
 *         channel from TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval HAL status
 */
static uint16_t LSM6DSL_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);

//==============================================================================
// PRIVATE SOURCE CODE
//==============================================================================

static void LSM6DSL_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(pI2C_LSM6DSL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &Value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		DBG("ERRO I2C");
	}
}

static uint8_t LSM6DSL_IO_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t read_value = 0;
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_LSM6DSL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) &read_value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occured */
		DBG("ERRO I2C");
	}

	return read_value;
}

static uint16_t LSM6DSL_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(pI2C_LSM6DSL, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length, 1000);

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

void LSM6DSL_attach(I2C_HandleTypeDef *hI2Handler)
{
	pI2C_LSM6DSL = hI2Handler;
}

void LSM6DSL_myInit(void)
{
	uint16_t ctrl = 0x0000;
	GYRO_Init_t LSM6DSL_InitStructure;
	ACCELERO_Init_t aLSM6DSL_InitStructure;

	/* Inicializa Giroscopio */
	LSM6DSL_InitStructure.Power_Mode = 0;
	LSM6DSL_InitStructure.Output_DataRate = LSM6DSL_ODR_52Hz;
	LSM6DSL_InitStructure.Axes_Enable = 0;
	LSM6DSL_InitStructure.Band_Width = 0;
	LSM6DSL_InitStructure.BlockData_Update = LSM6DSL_BDU_BLOCK_UPDATE;
	LSM6DSL_InitStructure.Endianness = 0;
	LSM6DSL_InitStructure.Full_Scale = LSM6DSL_GYRO_FS_2000;

	/* Configure MEMS: data rate, full scale  */
	ctrl = (LSM6DSL_InitStructure.Full_Scale | LSM6DSL_InitStructure.Output_DataRate);

	/* Configure MEMS: BDU and Auto-increment for multi read/write */
	ctrl |= ((LSM6DSL_InitStructure.BlockData_Update | LSM6DSL_ACC_GYRO_IF_INC_ENABLED) << 8);

	/* Initialize component */
	LSM6DSL_GyroInit(ctrl);

	/* Inicializa Acelerometro */
	aLSM6DSL_InitStructure.AccOutput_DataRate = LSM6DSL_ODR_52Hz;
	aLSM6DSL_InitStructure.Axes_Enable = 0;
	aLSM6DSL_InitStructure.AccFull_Scale = LSM6DSL_ACC_FULLSCALE_2G;
	aLSM6DSL_InitStructure.BlockData_Update = LSM6DSL_BDU_BLOCK_UPDATE;
	aLSM6DSL_InitStructure.High_Resolution = 0;
	aLSM6DSL_InitStructure.Communication_Mode = 0;

	/* Configure MEMS: data rate, full scale  */
	ctrl =  (aLSM6DSL_InitStructure.AccOutput_DataRate | aLSM6DSL_InitStructure.AccFull_Scale);

	/* Configure MEMS: BDU and Auto-increment for multi read/write */
	ctrl |= ((aLSM6DSL_InitStructure.BlockData_Update | LSM6DSL_ACC_GYRO_IF_INC_ENABLED) << 8);
	LSM6DSL_AccInit(ctrl);
}

void LSM6DSL_AccInit(uint16_t InitStruct)
{
	uint8_t ctrl = 0x00;
	uint8_t tmp;

	/* Read CTRL1_XL */
	tmp = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);

	/* Write value to ACC MEMS CTRL1_XL register: FS and Data Rate */
	ctrl = (uint8_t) InitStruct;
	tmp &= ~(0xFC);
	tmp |= ctrl;
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, tmp);

	/* Read CTRL3_C */
	tmp = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C);

	/* Write value to ACC MEMS CTRL3_C register: BDU and Auto-increment */
	ctrl = ((uint8_t) (InitStruct >> 8));
	tmp &= ~(0x44);
	tmp |= ctrl;
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C, tmp);
}

void LSM6DSL_AccDeInit(void)
{
	uint8_t ctrl = 0x00;

	/* Read control register 1 value */
	ctrl = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);

	/* Clear ODR bits */
	ctrl &= ~(LSM6DSL_ODR_BITPOSITION);

	/* Set Power down */
	ctrl |= LSM6DSL_ODR_POWER_DOWN;

	/* write back control register */
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, ctrl);
}

uint8_t LSM6DSL_AccReadID(void)
{
	/* Read value at Who am I register address */
	return (LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_WHO_AM_I_REG));
}

void LSM6DSL_AccLowPower(uint16_t status)
{
	uint8_t ctrl = 0x00;

	/* Read CTRL6_C value */
	ctrl = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL6_C);

	/* Clear Low Power Mode bit */
	ctrl &= ~(0x10);

	/* Set Low Power Mode */
	if (status)
	{
		ctrl |= LSM6DSL_ACC_GYRO_LP_XL_ENABLED;
	}
	else
	{
		ctrl |= LSM6DSL_ACC_GYRO_LP_XL_DISABLED;
	}

	/* write back control register */
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL6_C, ctrl);
}

void LSM6DSL_AccReadXYZ(int16_t* pData)
{
	int16_t pnRawData[3];
	uint8_t ctrlx = 0;
	uint8_t buffer[6];
	uint8_t i = 0;
	float sensitivity = 0;

	/* Read the acceleration control register content */
	ctrlx = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);

	/* Read output register X, Y & Z acceleration */
	LSM6DSL_IO_ReadMultiple(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_OUTX_L_XL, buffer, 6);

	for (i = 0; i < 3; i++)
	{
		pnRawData[i] = ((((uint16_t) buffer[2 * i + 1]) << 8) + (uint16_t) buffer[2 * i]);
	}

	/* Normal mode */
	/* Switch the sensitivity value set in the CRTL1_XL */
	switch (ctrlx & 0x0C)
	{
	case LSM6DSL_ACC_FULLSCALE_2G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_2G;
		break;

	case LSM6DSL_ACC_FULLSCALE_4G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_4G;
		break;

	case LSM6DSL_ACC_FULLSCALE_8G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_8G;
		break;

	case LSM6DSL_ACC_FULLSCALE_16G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_16G;
		break;
	}

	/* Obtain the mg value for the three axis */
	for (i = 0; i < 3; i++)
	{
		pData[i] = (int16_t) (pnRawData[i] * sensitivity);
	}
}

void LSM6DSL_GyroInit(uint16_t InitStruct)
{
	uint8_t ctrl = 0x00;
	uint8_t tmp;

	/* Read CTRL2_G */
	tmp = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,
			LSM6DSL_ACC_GYRO_CTRL2_G);

	/* Write value to GYRO MEMS CTRL2_G register: FS and Data Rate */
	ctrl = (uint8_t) InitStruct;
	tmp &= ~(0xFC);
	tmp |= ctrl;
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G, tmp);

	/* Read CTRL3_C */
	tmp = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C);

	/* Write value to GYRO MEMS CTRL3_C register: BDU and Auto-increment */
	ctrl = ((uint8_t) (InitStruct >> 8));
	tmp &= ~(0x44);
	tmp |= ctrl;
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C, tmp);
}

void LSM6DSL_GyroDeInit(void)
{
	uint8_t ctrl = 0x00;

	/* Read control register 1 value */
	ctrl = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G);

	/* Clear ODR bits */
	ctrl &= ~(LSM6DSL_ODR_BITPOSITION);

	/* Set Power down */
	ctrl |= LSM6DSL_ODR_POWER_DOWN;

	/* write back control register */
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G, ctrl);
}

uint8_t LSM6DSL_GyroReadID(void)
{
	/* Read value at Who am I register address */
	return LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_WHO_AM_I_REG);
}

void LSM6DSL_GyroLowPower(uint16_t status)
{
	uint8_t ctrl = 0x00;

	/* Read CTRL7_G value */
	ctrl = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL7_G);

	/* Clear Low Power Mode bit */
	ctrl &= ~(0x80);

	/* Set Low Power Mode */
	if (status)
	{
		ctrl |= LSM6DSL_ACC_GYRO_LP_G_ENABLED;
	}
	else
	{
		ctrl |= LSM6DSL_ACC_GYRO_LP_G_DISABLED;
	}

	/* write back control register */
	LSM6DSL_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL7_G, ctrl);
}

void LSM6DSL_GyroReadXYZAngRate(float *pfData)
{
	int16_t pnRawData[3];
	uint8_t ctrlg = 0;
	uint8_t buffer[6];
	uint8_t i = 0;
	float sensitivity = 0;

	/* Read the gyro control register content */
	ctrlg = LSM6DSL_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G);

	/* Read output register X, Y & Z acceleration */
	LSM6DSL_IO_ReadMultiple(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_OUTX_L_G, buffer, 6);

	for (i = 0; i < 3; i++)
	{
		pnRawData[i] = ((((uint16_t) buffer[2 * i + 1]) << 8) + (uint16_t) buffer[2 * i]);
	}

	/* Normal mode */
	/* Switch the sensitivity value set in the CRTL2_G */
	switch (ctrlg & 0x0C)
	{
	case LSM6DSL_GYRO_FS_245:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_245DPS;
		break;

	case LSM6DSL_GYRO_FS_500:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_500DPS;
		break;

	case LSM6DSL_GYRO_FS_1000:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_1000DPS;
		break;

	case LSM6DSL_GYRO_FS_2000:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_2000DPS;
		break;
	}

	/* Obtain the mg value for the three axis */
	for (i = 0; i < 3; i++)
	{
		pfData[i] = (float) (pnRawData[i] * sensitivity);
	}
}
