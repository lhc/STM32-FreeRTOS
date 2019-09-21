/**
 ******************************************************************************
 * @file    lsm6dsl.h
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    14-February-2017
 * @brief   LSM6DSL header driver file
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM6DSL__H
#define __LSM6DSL__H

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"

//==============================================================================
// PUBLIC DEFINES
//==============================================================================

/************** I2C Address *****************/

#define LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW    0xD4  // SAD[0] = 0
#define LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH   0xD6  // SAD[0] = 1

/************** Who am I  *******************/

#define LSM6DSL_ACC_GYRO_WHO_AM_I           0x6A

/************** Device Register  *******************/

#define LSM6DSL_ACC_GYRO_FUNC_CFG_ACCESS    0x01

#define LSM6DSL_ACC_GYRO_SENSOR_SYNC_TIME   0x04
#define LSM6DSL_ACC_GYRO_SENSOR_RES_RATIO   0x05

#define LSM6DSL_ACC_GYRO_FIFO_CTRL1         0x06
#define LSM6DSL_ACC_GYRO_FIFO_CTRL2         0x07
#define LSM6DSL_ACC_GYRO_FIFO_CTRL3         0x08
#define LSM6DSL_ACC_GYRO_FIFO_CTRL4         0x09
#define LSM6DSL_ACC_GYRO_FIFO_CTRL5         0x0A

#define LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_G   0x0B
#define LSM6DSL_ACC_GYRO_INT1_CTRL          0x0D
#define LSM6DSL_ACC_GYRO_INT2_CTRL          0x0E
#define LSM6DSL_ACC_GYRO_WHO_AM_I_REG       0x0F
#define LSM6DSL_ACC_GYRO_CTRL1_XL           0x10
#define LSM6DSL_ACC_GYRO_CTRL2_G            0x11
#define LSM6DSL_ACC_GYRO_CTRL3_C            0x12
#define LSM6DSL_ACC_GYRO_CTRL4_C            0x13
#define LSM6DSL_ACC_GYRO_CTRL5_C            0x14
#define LSM6DSL_ACC_GYRO_CTRL6_C            0x15
#define LSM6DSL_ACC_GYRO_CTRL7_G            0x16
#define LSM6DSL_ACC_GYRO_CTRL8_XL           0x17
#define LSM6DSL_ACC_GYRO_CTRL9_XL           0x18
#define LSM6DSL_ACC_GYRO_CTRL10_C           0x19

#define LSM6DSL_ACC_GYRO_MASTER_CONFIG      0x1A
#define LSM6DSL_ACC_GYRO_WAKE_UP_SRC        0x1B
#define LSM6DSL_ACC_GYRO_TAP_SRC            0x1C
#define LSM6DSL_ACC_GYRO_D6D_SRC            0x1D
#define LSM6DSL_ACC_GYRO_STATUS_REG         0x1E

#define LSM6DSL_ACC_GYRO_OUT_TEMP_L         0x20
#define LSM6DSL_ACC_GYRO_OUT_TEMP_H         0x21
#define LSM6DSL_ACC_GYRO_OUTX_L_G           0x22
#define LSM6DSL_ACC_GYRO_OUTX_H_G           0x23
#define LSM6DSL_ACC_GYRO_OUTY_L_G           0x24
#define LSM6DSL_ACC_GYRO_OUTY_H_G           0x25
#define LSM6DSL_ACC_GYRO_OUTZ_L_G           0x26
#define LSM6DSL_ACC_GYRO_OUTZ_H_G           0x27
#define LSM6DSL_ACC_GYRO_OUTX_L_XL          0x28
#define LSM6DSL_ACC_GYRO_OUTX_H_XL          0x29
#define LSM6DSL_ACC_GYRO_OUTY_L_XL          0x2A
#define LSM6DSL_ACC_GYRO_OUTY_H_XL          0x2B
#define LSM6DSL_ACC_GYRO_OUTZ_L_XL          0x2C
#define LSM6DSL_ACC_GYRO_OUTZ_H_XL          0x2D
#define LSM6DSL_ACC_GYRO_SENSORHUB1_REG     0x2E
#define LSM6DSL_ACC_GYRO_SENSORHUB2_REG     0x2F
#define LSM6DSL_ACC_GYRO_SENSORHUB3_REG     0x30
#define LSM6DSL_ACC_GYRO_SENSORHUB4_REG     0x31
#define LSM6DSL_ACC_GYRO_SENSORHUB5_REG     0x32
#define LSM6DSL_ACC_GYRO_SENSORHUB6_REG     0x33
#define LSM6DSL_ACC_GYRO_SENSORHUB7_REG     0x34
#define LSM6DSL_ACC_GYRO_SENSORHUB8_REG     0x35
#define LSM6DSL_ACC_GYRO_SENSORHUB9_REG     0x36
#define LSM6DSL_ACC_GYRO_SENSORHUB10_REG    0x37
#define LSM6DSL_ACC_GYRO_SENSORHUB11_REG    0x38
#define LSM6DSL_ACC_GYRO_SENSORHUB12_REG    0x39
#define LSM6DSL_ACC_GYRO_FIFO_STATUS1       0x3A
#define LSM6DSL_ACC_GYRO_FIFO_STATUS2       0x3B
#define LSM6DSL_ACC_GYRO_FIFO_STATUS3       0x3C
#define LSM6DSL_ACC_GYRO_FIFO_STATUS4       0x3D
#define LSM6DSL_ACC_GYRO_FIFO_DATA_OUT_L    0x3E
#define LSM6DSL_ACC_GYRO_FIFO_DATA_OUT_H    0x3F
#define LSM6DSL_ACC_GYRO_TIMESTAMP0_REG     0x40
#define LSM6DSL_ACC_GYRO_TIMESTAMP1_REG     0x41
#define LSM6DSL_ACC_GYRO_TIMESTAMP2_REG     0x42

#define LSM6DSL_ACC_GYRO_TIMESTAMP_L        0x49
#define LSM6DSL_ACC_GYRO_TIMESTAMP_H        0x4A

#define LSM6DSL_ACC_GYRO_STEP_COUNTER_L     0x4B
#define LSM6DSL_ACC_GYRO_STEP_COUNTER_H     0x4C

#define LSM6DSL_ACC_GYRO_SENSORHUB13_REG    0x4D
#define LSM6DSL_ACC_GYRO_SENSORHUB14_REG    0x4E
#define LSM6DSL_ACC_GYRO_SENSORHUB15_REG    0x4F
#define LSM6DSL_ACC_GYRO_SENSORHUB16_REG    0x50
#define LSM6DSL_ACC_GYRO_SENSORHUB17_REG    0x51
#define LSM6DSL_ACC_GYRO_SENSORHUB18_REG    0x52

#define LSM6DSL_ACC_GYRO_FUNC_SRC           0x53
#define LSM6DSL_ACC_GYRO_TAP_CFG1           0x58
#define LSM6DSL_ACC_GYRO_TAP_THS_6D         0x59
#define LSM6DSL_ACC_GYRO_INT_DUR2           0x5A
#define LSM6DSL_ACC_GYRO_WAKE_UP_THS        0x5B
#define LSM6DSL_ACC_GYRO_WAKE_UP_DUR        0x5C
#define LSM6DSL_ACC_GYRO_FREE_FALL          0x5D
#define LSM6DSL_ACC_GYRO_MD1_CFG            0x5E
#define LSM6DSL_ACC_GYRO_MD2_CFG            0x5F

#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_X_L    0x66
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_X_H    0x67
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Y_L    0x68
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Y_H    0x69
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Z_L    0x6A
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Z_H    0x6B

#define LSM6DSL_ACC_GYRO_X_OFS_USR          0x73
#define LSM6DSL_ACC_GYRO_Y_OFS_USR          0x74
#define LSM6DSL_ACC_GYRO_Z_OFS_USR          0x75

/************** Embedded functions register mapping  *******************/
#define LSM6DSL_ACC_GYRO_SLV0_ADD                     0x02
#define LSM6DSL_ACC_GYRO_SLV0_SUBADD                  0x03
#define LSM6DSL_ACC_GYRO_SLAVE0_CONFIG                0x04
#define LSM6DSL_ACC_GYRO_SLV1_ADD                     0x05
#define LSM6DSL_ACC_GYRO_SLV1_SUBADD                  0x06
#define LSM6DSL_ACC_GYRO_SLAVE1_CONFIG                0x07
#define LSM6DSL_ACC_GYRO_SLV2_ADD                     0x08
#define LSM6DSL_ACC_GYRO_SLV2_SUBADD                  0x09
#define LSM6DSL_ACC_GYRO_SLAVE2_CONFIG                0x0A
#define LSM6DSL_ACC_GYRO_SLV3_ADD                     0x0B
#define LSM6DSL_ACC_GYRO_SLV3_SUBADD                  0x0C
#define LSM6DSL_ACC_GYRO_SLAVE3_CONFIG                0x0D
#define LSM6DSL_ACC_GYRO_DATAWRITE_SRC_MODE_SUB_SLV0  0x0E
#define LSM6DSL_ACC_GYRO_CONFIG_PEDO_THS_MIN          0x0F

#define LSM6DSL_ACC_GYRO_SM_STEP_THS                  0x13
#define LSM6DSL_ACC_GYRO_PEDO_DEB_REG                 0x14
#define LSM6DSL_ACC_GYRO_STEP_COUNT_DELTA             0x15

#define LSM6DSL_ACC_GYRO_MAG_SI_XX                    0x24
#define LSM6DSL_ACC_GYRO_MAG_SI_XY                    0x25
#define LSM6DSL_ACC_GYRO_MAG_SI_XZ                    0x26
#define LSM6DSL_ACC_GYRO_MAG_SI_YX                    0x27
#define LSM6DSL_ACC_GYRO_MAG_SI_YY                    0x28
#define LSM6DSL_ACC_GYRO_MAG_SI_YZ                    0x29
#define LSM6DSL_ACC_GYRO_MAG_SI_ZX                    0x2A
#define LSM6DSL_ACC_GYRO_MAG_SI_ZY                    0x2B
#define LSM6DSL_ACC_GYRO_MAG_SI_ZZ                    0x2C
#define LSM6DSL_ACC_GYRO_MAG_OFFX_L                   0x2D
#define LSM6DSL_ACC_GYRO_MAG_OFFX_H                   0x2E
#define LSM6DSL_ACC_GYRO_MAG_OFFY_L                   0x2F
#define LSM6DSL_ACC_GYRO_MAG_OFFY_H                   0x30
#define LSM6DSL_ACC_GYRO_MAG_OFFZ_L                   0x31
#define LSM6DSL_ACC_GYRO_MAG_OFFZ_H                   0x32

/* Accelero Full_ScaleSelection */
#define LSM6DSL_ACC_FULLSCALE_2G          ((uint8_t)0x00) /*!< �2 g */
#define LSM6DSL_ACC_FULLSCALE_4G          ((uint8_t)0x08) /*!< �4 g */
#define LSM6DSL_ACC_FULLSCALE_8G          ((uint8_t)0x0C) /*!< �8 g */
#define LSM6DSL_ACC_FULLSCALE_16G         ((uint8_t)0x04) /*!< �16 g */

/* Accelero Full Scale Sensitivity */
#define LSM6DSL_ACC_SENSITIVITY_2G     ((float)0.061f)  /*!< accelerometer sensitivity with 2 g full scale  [mgauss/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_4G     ((float)0.122f)  /*!< accelerometer sensitivity with 4 g full scale  [mgauss/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_8G     ((float)0.244f)  /*!< accelerometer sensitivity with 8 g full scale  [mgauss/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_16G    ((float)0.488f)  /*!< accelerometer sensitivity with 12 g full scale [mgauss/LSB] */

/* Accelero Power Mode selection */
#define LSM6DSL_ACC_GYRO_LP_XL_DISABLED     ((uint8_t)0x00) /* LP disabled*/
#define LSM6DSL_ACC_GYRO_LP_XL_ENABLED      ((uint8_t)0x10) /* LP enabled*/

/* Output Data Rate */
#define LSM6DSL_ODR_BITPOSITION      ((uint8_t)0xF0)  /*!< Output Data Rate bit position */  
#define LSM6DSL_ODR_POWER_DOWN       ((uint8_t)0x00) /* Power Down mode       */
#define LSM6DSL_ODR_13Hz             ((uint8_t)0x10) /* Low Power mode        */
#define LSM6DSL_ODR_26Hz             ((uint8_t)0x20) /* Low Power mode        */ 
#define LSM6DSL_ODR_52Hz             ((uint8_t)0x30) /* Low Power mode        */
#define LSM6DSL_ODR_104Hz            ((uint8_t)0x40) /* Normal mode           */
#define LSM6DSL_ODR_208Hz            ((uint8_t)0x50) /* Normal mode           */
#define LSM6DSL_ODR_416Hz            ((uint8_t)0x60) /* High Performance mode */
#define LSM6DSL_ODR_833Hz            ((uint8_t)0x70) /* High Performance mode */
#define LSM6DSL_ODR_1660Hz           ((uint8_t)0x80) /* High Performance mode */
#define LSM6DSL_ODR_3330Hz           ((uint8_t)0x90) /* High Performance mode */
#define LSM6DSL_ODR_6660Hz           ((uint8_t)0xA0) /* High Performance mode */ 

/* Gyro Full Scale Selection */
#define LSM6DSL_GYRO_FS_245            ((uint8_t)0x00)  
#define LSM6DSL_GYRO_FS_500            ((uint8_t)0x04)  
#define LSM6DSL_GYRO_FS_1000           ((uint8_t)0x08)  
#define LSM6DSL_GYRO_FS_2000           ((uint8_t)0x0C)

/* Gyro Full Scale Sensitivity */ 
#define LSM6DSL_GYRO_SENSITIVITY_245DPS            ((float)8.750f) /**< Sensitivity value for 245 dps full scale  [mdps/LSB] */ 
#define LSM6DSL_GYRO_SENSITIVITY_500DPS            ((float)17.50f) /**< Sensitivity value for 500 dps full scale  [mdps/LSB] */ 
#define LSM6DSL_GYRO_SENSITIVITY_1000DPS           ((float)35.00f) /**< Sensitivity value for 1000 dps full scale [mdps/LSB] */ 
#define LSM6DSL_GYRO_SENSITIVITY_2000DPS           ((float)70.00f) /**< Sensitivity value for 2000 dps full scale [mdps/LSB] */ 

/* Gyro Power Mode selection */
#define LSM6DSL_ACC_GYRO_LP_G_DISABLED     ((uint8_t)0x00) /* LP disabled*/
#define LSM6DSL_ACC_GYRO_LP_G_ENABLED      ((uint8_t)0x80) /* LP enabled*/

/* Block Data Update */  
#define LSM6DSL_BDU_CONTINUOS               ((uint8_t)0x00)
#define LSM6DSL_BDU_BLOCK_UPDATE            ((uint8_t)0x40)

/* Auto-increment */
#define LSM6DSL_ACC_GYRO_IF_INC_DISABLED    ((uint8_t)0x00)
#define LSM6DSL_ACC_GYRO_IF_INC_ENABLED     ((uint8_t)0x04)

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

/** @brief GYRO_Config_structure  Gyroscope Configuration structure */
typedef struct
{
	uint8_t Power_Mode; /* Power-down/Sleep/Normal Mode */
	uint8_t Output_DataRate; /* OUT data rate */
	uint8_t Axes_Enable; /* Axes enable */
	uint8_t Band_Width; /* Bandwidth selection */
	uint8_t BlockData_Update; /* Block Data Update */
	uint8_t Endianness; /* Endian Data selection */
	uint8_t Full_Scale; /* Full Scale selection */
} GYRO_Init_t;

/** @brief ACCELERO struct */
typedef struct
{
	uint8_t Power_Mode; /* Power-down/Normal Mode */
	uint8_t AccOutput_DataRate; /* OUT data rate */
	uint8_t Axes_Enable; /* Axes enable */
	uint8_t High_Resolution; /* High Resolution enabling/disabling */
	uint8_t BlockData_Update; /* Block Data Update */
	uint8_t Endianness; /* Endian Data selection */
	uint8_t AccFull_Scale; /* Full Scale selection */
	uint8_t Communication_Mode;
} ACCELERO_Init_t;

/** @brief ACCELERO High Pass Filter struct */
typedef struct
{
	uint8_t HighPassFilter_Mode_Selection; /* Internal filter mode */
	uint8_t HighPassFilter_CutOff_Frequency; /* High pass filter cut-off frequency */
	uint8_t HighPassFilter_AOI1; /* HPF_enabling/disabling for AOI function on interrupt 1 */
	uint8_t HighPassFilter_AOI2; /* HPF_enabling/disabling for AOI function on interrupt 2 */
	uint8_t HighPassFilter_Data_Sel;
	uint8_t HighPassFilter_Stat;
} ACCELERO_FilterConfig_t;

/* GYRO High Pass Filter struct */
typedef struct
{
  uint8_t HighPassFilter_Mode_Selection;      /* Internal filter mode */
  uint8_t HighPassFilter_CutOff_Frequency;    /* High pass filter cut-off frequency */
}GYRO_FilterConfig_t;

/*GYRO Interrupt struct */
typedef struct
{
  uint8_t Latch_Request;                      /* Latch interrupt request into CLICK_SRC register */
  uint8_t Interrupt_Axes;                     /* X, Y, Z Axes Interrupts */
  uint8_t Interrupt_ActiveEdge;               /* Interrupt Active edge */
}GYRO_InterruptConfig_t;

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void LSM6DSL_attach(I2C_HandleTypeDef *hI2Handler);

//==============================================================================
// Sensor Accelerometer Functions
//==============================================================================

/**
 * @brief  Set LSM6DSL Accelerometer Initialization.
 * @param  InitStruct: Init parameters
 */
void LSM6DSL_AccInit(uint16_t InitStruct);

/**
 * @brief  LSM6DSL Accelerometer De-initialization.
 */
void LSM6DSL_AccDeInit(void);

/**
 * @brief  Read LSM6DSL ID.
 * @retval ID
 */
uint8_t LSM6DSL_AccReadID(void);

/**
 * @brief  Set/Unset Accelerometer in low power mode.
 * @param  status 0 means disable Low Power Mode, otherwise Low Power Mode is enabled
 */
void LSM6DSL_AccLowPower(uint16_t status);

/**
 * @brief  Read X, Y & Z Acceleration values
 * @param  pData: Data out pointer
 */
void LSM6DSL_AccReadXYZ(int16_t* pData);

//==============================================================================
// Sensor Configuration Functions
//==============================================================================

/**
 * @brief  Set LSM6DSL Gyroscope Initialization.
 * @param  InitStruct: pointer to a LSM6DSL_InitTypeDef structure
 *         that contains the configuration setting for the LSM6DSL.
 */
void LSM6DSL_GyroInit(uint16_t InitStruct);

/**
 * @brief LSM6DSL Gyroscope De-initialization
 */
void LSM6DSL_GyroDeInit(void);

/**
 * @brief  Read ID address of LSM6DSL
 * @retval ID
 */
uint8_t LSM6DSL_GyroReadID(void);
/**
 * @brief Set/Unset LSM6DSL Gyroscope in low power mode
 * @param  status 0 means disable Low Power Mode, otherwise Low Power Mode is enabled
 */
void LSM6DSL_GyroLowPower(uint16_t status);

/**
 * @brief  Calculate the LSM6DSL angular data.
 * @param  pfData: Data out pointer
 */
void LSM6DSL_GyroReadXYZAngRate(float *pfData);

void LSM6DSL_myInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __LSM6DSL__H */
