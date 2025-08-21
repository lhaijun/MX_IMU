#ifndef __ICM42688P_H__
#define __ICM42688P_H__

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx_hal.h"

// --- Bit definitions (位定义)
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)

// INTF_CONFIG0 (0x4C)
#define BIT_FIFO_COUNT_REC   (1u<<6)  // count in records (incl. 20B when HIRES)
#define BIT_FIFO_COUNT_BE    (1u<<5)  // 0=little, 1=big (default big)
#define BIT_SENSOR_DATA_BE   (1u<<4)  // 0=little, 1=big (default big)

// FIFO_CONFIG (0x16)  FIFO_MODE[1:0]: 0=BYPASS, 1=STREAM, 2=STOP-ON-FULL
#define FIFO_MODE_STREAM   0x01

// FIFO_CONFIG1 (0x5F)
#define BIT_FIFO_HIRES_EN      (1u<<4)
#define BIT_FIFO_TMST_FSYNC_EN (1u<<3)
#define BIT_FIFO_TEMP_EN       (1u<<2)
#define BIT_FIFO_GYRO_EN       (1u<<1)
#define BIT_FIFO_ACCEL_EN      (1u<<0)


// --- Constants (常量)
#define ICM42688P_SPI_SPEED               24000000UL
#define ICM42688P_DIR_READ                0x80
#define ICM42688P_WHO_AM_I_VAL            0x47
#define ICM42688P_TEMPERATURE_SENSITIVITY 132.48f
#define ICM42688P_TEMPERATURE_OFFSET      25.0f

// FSR sensitivity values (根据数据手册计算的FSR灵敏度值)
#define ICM42688P_ACCEL_16G_SENSITIVITY   (1.0f / 2048.0f) // LSB per g for 16G FSR (16G量程下，每g的LSB值)
#define ICM42688P_ACCEL_8G_SENSITIVITY    (1.0f / 4096.0f)
#define ICM42688P_ACCEL_4G_SENSITIVITY    (1.0f / 8192.0f)
#define ICM42688P_ACCEL_2G_SENSITIVITY    (1.0f / 16384.0f)
#define ICM42688P_GYRO_2000DPS_SENSITIVITY (1.0f / 16.4f)  // LSB per dps for 2000dps FSR
#define ICM42688P_GYRO_1000DPS_SENSITIVITY (1.0f / 32.8f)
#define ICM42688P_GYRO_500DPS_SENSITIVITY  (1.0f / 65.5f)
#define ICM42688P_GYRO_250DPS_SENSITIVITY  (1.0f / 131.0f)
#define ICM42688P_GYRO_125DPS_SENSITIVITY  (1.0f / 262.0f)

// --- Register Addresses by Bank (按Bank划分的寄存器地址)
// Bank 0 Registers
#define    ICM42688P_REG_BANK0_DEVICE_CONFIG       0x11
#define    ICM42688P_REG_BANK0_INT_CONFIG          0x14
#define    ICM42688P_REG_BANK0_FIFO_CONFIG         0x16
#define    ICM42688P_REG_BANK0_TEMP_DATA1          0x1D
#define    ICM42688P_REG_BANK0_TEMP_DATA0          0x1E
#define    ICM42688P_REG_BANK0_INT_STATUS          0x2D
#define    ICM42688P_REG_BANK0_FIFO_COUNTH         0x2E
#define    ICM42688P_REG_BANK0_FIFO_COUNTL         0x2F
#define    ICM42688P_REG_BANK0_FIFO_DATA           0x30
#define    ICM42688P_REG_BANK0_SIGNAL_PATH_RESET   0x4B
#define    ICM42688P_REG_BANK0_INTF_CONFIG0        0x4C
#define    ICM42688P_REG_BANK0_INTF_CONFIG1        0x4D
#define    ICM42688P_REG_BANK0_PWR_MGMT0           0x4E
#define    ICM42688P_REG_BANK0_GYRO_CONFIG0        0x4F
#define    ICM42688P_REG_BANK0_ACCEL_CONFIG0       0x50
#define    ICM42688P_REG_BANK0_GYRO_CONFIG1        0x51
#define    ICM42688P_REG_BANK0_GYRO_ACCEL_CONFIG0  0x52
#define    ICM42688P_REG_BANK0_ACCEL_CONFIG1       0x53
#define    ICM42688P_REG_BANK0_TMST_CONFIG         0x54
#define    ICM42688P_REG_BANK0_FIFO_CONFIG1        0x5F
#define    ICM42688P_REG_BANK0_FIFO_CONFIG2        0x60
#define    ICM42688P_REG_BANK0_FIFO_CONFIG3        0x61
#define    ICM42688P_REG_BANK0_INT_CONFIG0         0x63
#define    ICM42688P_REG_BANK0_INT_CONFIG1         0x64
#define   ICM42688P_REG_BANK0_INT_SOURCE0          0x65
#define   ICM42688P_REG_BANK0_SELF_TEST_CONFIG     0x70
#define    ICM42688P_REG_BANK0_WHO_AM_I            0x75
#define    ICM42688P_REG_BANK0_REG_BANK_SEL        0x76

// Bank 1 Registers

#define    ICM42688P_REG_BANK1_GYRO_CONFIG_STATIC2   0x0B
#define    ICM42688P_REG_BANK1_GYRO_CONFIG_STATIC3   0x0C
#define    ICM42688P_REG_BANK1_GYRO_CONFIG_STATIC4   0x0D
#define    ICM42688P_REG_BANK1_GYRO_CONFIG_STATIC5   0x0E
#define    ICM42688P_REG_BANK1_INTF_CONFIG5          0x7B

// Bank 2 Registers
#define    ICM42688P_REG_BANK2_ACCEL_CONFIG_STATIC2  0x03
#define    ICM42688P_REG_BANK2_ACCEL_CONFIG_STATIC3  0x04
#define    ICM42688P_REG_BANK2_ACCEL_CONFIG_STATIC4  0x05

// --- Bank 0 Register Bits (Bank 0 寄存器位定义)
// DEVICE_CONFIG
#define ICM42688P_DEVICE_CONFIG_SOFT_RESET_CONFIG BIT0

// INT_CONFIG
#define ICM42688P_INT_CONFIG_INT1_MODE           BIT2
#define ICM42688P_INT_CONFIG_INT1_DRIVE_CIRCUIT  BIT1
#define ICM42688P_INT_CONFIG_INT1_POLARITY       BIT0

// FIFO_CONFIG
#define ICM42688P_FIFO_MODE_STOP_ON_FULL (BIT7 | BIT6)

// INT_STATUS
typedef enum {
    ICM42688P_INT_STATUS_RESET_DONE_INT = BIT4,
    ICM42688P_INT_STATUS_DATA_RDY_INT   = BIT3,
    ICM42688P_INT_STATUS_FIFO_THS_INT   = BIT2,
    ICM42688P_INT_STATUS_FIFO_FULL_INT  = BIT1,
} icm42688p_int_status_bit_t;

// SIGNAL_PATH_RESET
#define ICM42688P_SIGNAL_PATH_RESET_ABORT_AND_RESET BIT3
#define ICM42688P_SIGNAL_PATH_RESET_FIFO_FLUSH      BIT1

// INTF_CONFIG0
#define ICM42688P_INTF_CONFIG0_FIFO_HOLD_LAST_DATA_EN  BIT7
#define ICM42688P_INTF_CONFIG0_FIFO_COUNT_REC          BIT6
#define ICM42688P_INTF_CONFIG0_FIFO_COUNT_ENDIAN       BIT5
#define ICM42688P_INTF_CONFIG0_SENSOR_DATA_ENDIAN      BIT4
#define ICM42688P_INTF_CONFIG0_UI_SIFS_CFG_DISABLE_I2C (BIT1 | BIT0)

// INTF_CONFIG1
#define ICM42688P_INTF_CONFIG1_ACCEL_LPF_CFG_SET(x)  ((x) << 4)
#define ICM42688P_INTF_CONFIG1_GYRO_LPF_CFG_SET(x)   ((x) << 1)
#define ICM42688P_INTF_CONFIG1_LPF_BW_33_2HZ_ODR_1K  0x05

// PWR_MGMT0
#define ICM42688P_PWR_MGMT0_GYRO_MODE_LOW_NOISE  (BIT3 | BIT2)
#define ICM42688P_PWR_MGMT0_ACCEL_MODE_LOW_NOISE (BIT1 | BIT0)

// GYRO_CONFIG0
typedef enum {
    ICM42688P_GYRO_FS_SEL_2000_DPS = 0,
    ICM42688P_GYRO_FS_SEL_1000_DPS = BIT5,
    ICM42688P_GYRO_FS_SEL_500_DPS  = BIT6,
    ICM42688P_GYRO_FS_SEL_250_DPS  = BIT6 | BIT5,
    ICM42688P_GYRO_FS_SEL_125_DPS  = BIT7,
    ICM42688P_GYRO_ODR_1KHZ_SET    = BIT2 | BIT1,
} icm42688p_gyro_config0_bit_t;

// ACCEL_CONFIG0
typedef enum {
    ICM42688P_ACCEL_FS_SEL_16G = 0,
    ICM42688P_ACCEL_FS_SEL_8G  = BIT5,
    ICM42688P_ACCEL_FS_SEL_4G  = BIT6,
    ICM42688P_ACCEL_FS_SEL_2G  = BIT6 | BIT5,
    ICM42688P_ACCEL_ODR_1KHZ_SET = BIT2 | BIT1,
} icm42688p_accel_config0_bit_t;

// INT_SOURCE0
typedef enum {
    ICM42688P_INT_SOURCE0_UI_FSYNC_INT1_EN   = BIT6,
    ICM42688P_INT_SOURCE0_PLL_RDY_INT1_EN    = BIT5,
    ICM42688P_INT_SOURCE0_RESET_DONE_INT1_EN = BIT4,
    ICM42688P_INT_SOURCE0_UI_DRDY_INT1_EN    = BIT3,
    ICM42688P_INT_SOURCE0_FIFO_THS_INT1_EN   = BIT2,
    ICM42688P_INT_SOURCE0_FIFO_FULL_INT1_EN  = BIT1,
    ICM42688P_INT_SOURCE0_UI_AGC_RDY_INT1_EN = BIT0,
} icm42688p_int_source0_bit_t;

// REG_BANK_SEL
typedef enum {
    ICM42688P_REG_BANK_SEL_0 = 0,
    ICM42688P_REG_BANK_SEL_1 = BIT0,
    ICM42688P_REG_BANK_SEL_2 = BIT1,
    ICM42688P_REG_BANK_SEL_3 = BIT1 | BIT0,
    ICM42688P_REG_BANK_SEL_4 = BIT2,
} icm42688p_reg_bank_sel_bit_t;

// --- FIFO Data Structure (FIFO数据结构)
#define ICM42688P_FIFO_SIZE 2048
#define ICM42688P_FIFO_DATA_SIZE_20B 20 // 20-byte data packet size
#define FIFO_WATERMARK_SIZE_HIGH_RES 0x1 // watermark for fifo

// FIFO_DATA layout when high precision mode is enabled
typedef struct {
    uint8_t FIFO_Header;
    uint8_t TEMP_DATA_H;
    uint8_t TEMP_DATA_L;
    uint8_t ACCEL_DATA_X_H;
    uint8_t ACCEL_DATA_X_L;
    uint8_t ACCEL_DATA_Y_H;
    uint8_t ACCEL_DATA_Y_L;
    uint8_t ACCEL_DATA_Z_H;
    uint8_t ACCEL_DATA_Z_L;
    uint8_t GYRO_DATA_X_H;
    uint8_t GYRO_DATA_X_L;
    uint8_t GYRO_DATA_Y_H;
    uint8_t GYRO_DATA_Y_L;
    uint8_t GYRO_DATA_Z_H;
    uint8_t GYRO_DATA_Z_L;
    uint8_t TIMESTAMP_H;
    uint8_t TIMESTAMP_L;
    uint8_t ACCEL_DATA_X_EXTRA; // Accel X [3:0] in bits [7:4]
    uint8_t ACCEL_DATA_Y_EXTRA; // Accel Y [3:0] in bits [7:4]
    uint8_t ACCEL_DATA_Z_EXTRA; // Accel Z [3:0] in bits [7:4]
} icm42688p_fifo_high_res_data_t;

// With FIFO_ACCEL_EN and FIFO_GYRO_EN header should be 8’b_0110_10xx
typedef enum {
    ICM42688P_FIFO_HEADER_MSG             = BIT7,
    ICM42688P_FIFO_HEADER_ACCEL           = BIT6,
    ICM42688P_FIFO_HEADER_GYRO            = BIT5,
    ICM42688P_FIFO_HEADER_20              = BIT4,
    ICM42688P_FIFO_HEADER_TIMESTAMP_FSYNC = BIT3 | BIT2,
    ICM42688P_FIFO_HEADER_ODR_ACCEL       = BIT1,
    ICM42688P_FIFO_HEADER_ODR_GYRO        = BIT0,
} icm42688p_fifo_header_bit_t;

//
// Sensor Data Structure (传感器数据结构体)
//
typedef struct {
    uint32_t timestamp;
    uint32_t dt;
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float temperature_C;
} ICM42688P_Data_t;

//
// Hardware Abstraction Layer for CS pin (CS引脚的硬件抽象层)
//
#define Icm_CS_HIGH() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)
#define Icm_CS_LOW()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)
#define ICM42688P_INT1_Pin  GPIO_PIN_0
//
// External variables and pointers (外部变量和指针)
//
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern uint8_t rx_dma_buffer[];
extern ICM42688P_Data_t sensor_data[];

//
// Driver Function Prototypes (驱动函数原型声明)
//
void ICM42688P_Init(void);
uint8_t ICM42688P_WriteReg(uint8_t reg, uint8_t value);
uint8_t ICM42688P_ReadReg(uint8_t reg);
void ICM42688P_Read_FIFO_DMA(void);
int icm42688_read_all(ICM42688P_Data_t *data);
// 新增：单次读取所有传感器数据的函数
void ICM42688P_ReadData(ICM42688P_Data_t* data);
float ICM42688P_ConvertTemp(int16_t raw_temp);
float ICM42688P_ConvertAccel(int32_t raw_accel, float sensitivity);
float ICM42688P_ConvertGyro(int32_t raw_gyro, float sensitivity);

// HAL Callback Prototypes
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);

#endif /* __ICM42688P_H__ */
