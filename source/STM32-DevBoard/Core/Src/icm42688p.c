#include "icm42688p.h"
#include <string.h>

extern TIM_HandleTypeDef htim2;

uint8_t hal_Spi1_ReadWriteByte(uint8_t txdata)
{
    uint8_t rxdata = 0;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 5); /*halstm32的spi读写函数*/
    return rxdata;
}
void Icm_Spi_ReadWriteNbytes(uint8_t* pBuffer, uint8_t len)
{
    uint8_t i = 0;
    for(i = 0; i < len; i ++)
    {
		*pBuffer = hal_Spi1_ReadWriteByte(*pBuffer);
        pBuffer++;
    }

}
uint8_t ICM42688P_ReadReg(uint8_t reg)
{
    uint8_t regval = 0;
    Icm_CS_LOW();
    reg |= 0x80;
    /* 写入要读的寄存器地址 */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* 读取寄存器数据 */
    Icm_Spi_ReadWriteNbytes(&regval, 1);
    Icm_CS_HIGH();
    return regval;
}

void ICM42688P_ReadRegs(uint8_t reg, uint8_t* buf, uint16_t len)
{
    reg |= 0x80;
    Icm_CS_LOW();
    /* 写入要读的寄存器地址 */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* 读取寄存器数据 */
    Icm_Spi_ReadWriteNbytes(buf, len);
    Icm_CS_HIGH();
}
uint8_t ICM42688P_WriteReg(uint8_t reg, uint8_t value)
{
	Icm_CS_LOW();
    /* 写入要读的寄存器地址 */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* 读取寄存器数据 */
    Icm_Spi_ReadWriteNbytes(&value, 1);
    Icm_CS_HIGH();
    return 0;
}

/**
 * @brief  Converts raw 20-bit accelerometer value to G's.
 * @param  raw_accel: Raw 20-bit accelerometer value.
 * @param  sensitivity: Full-scale range sensitivity in G's per LSB.
 * @return Accelerometer value in G's.
 * @brief  将20位加速度计原始值转换为重力加速度单位G。
 * @param  raw_accel: 20位加速度计原始值。
 * @param  sensitivity: 以G/LSB为单位的满量程范围灵敏度。
 * @return 以G为单位的加速度计值。
 */
float ICM42688P_ConvertAccel(int32_t raw_accel, float sensitivity) {
    return (float)raw_accel * sensitivity;
}

/**
 * @brief  Converts raw 20-bit gyroscope value to dps.
 * @param  raw_gyro: Raw 20-bit gyroscope value.
 * @param  sensitivity: Full-scale range sensitivity in dps per LSB.
 * @return Gyroscope value in dps.
 * @brief  将20位陀螺仪原始值转换为度/秒（dps）。
 * @param  raw_gyro: 20位陀螺仪原始值。
 * @param  sensitivity: 以dps/LSB为单位的满量程范围灵敏度。
 * @return 以dps为单位的陀螺仪值。
 */
float ICM42688P_ConvertGyro(int32_t raw_gyro, float sensitivity) {
    return (float)raw_gyro * sensitivity;
}

/**
 * @brief  Converts raw 16-bit temperature value to degrees Celsius.
 * @param  raw_temp: Raw 16-bit temperature value.
 * @return Temperature value in degrees Celsius.
 * @brief  将16位温度原始值转换为摄氏度。
 * @param  raw_temp: 16位温度原始值。
 * @return 摄氏度单位的温度值。
 */
float ICM42688P_ConvertTemp(int16_t raw_temp) {
    return ((float)raw_temp / ICM42688P_TEMPERATURE_SENSITIVITY) + ICM42688P_TEMPERATURE_OFFSET;
}

/**
 * @brief  Initializes the ICM42688P sensor with specified configurations for high precision.
 * @brief  初始化ICM42688P传感器，进行高精度配置。
 */
void ICM42688P_Init(void) {
    uint8_t who_am_i = 0;

    /*ָ  Bank0*/
    ICM42688P_WriteReg(0x76,0x00);

    HAL_Delay(50); // Wait for sensor to power up (等待传感器上电)
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_REG_BANK_SEL, ICM42688P_REG_BANK_SEL_0);

    // 1. Soft Reset: Soft Reset the sensor to ensure a known state.
    // 1. 软复位：对传感器进行软复位，以确保从已知状态开始。
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_DEVICE_CONFIG, BIT0);
    HAL_Delay(500); // Wait for reset to complete (等待复位完成)


    // 2. Check WHO_AM_I register to verify communication.
    // 2. 检查WHO_AM_I寄存器，确认通信正常。
    who_am_i = ICM42688P_ReadReg(ICM42688P_REG_BANK0_WHO_AM_I);
    if (who_am_i != ICM42688P_WHO_AM_I_VAL) {
        while(1){
      	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
      	  HAL_Delay(100);
      	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
      	  HAL_Delay(100);
        };
    }
    // 3. Register Bank 0 Configuration.
    // 3. 寄存器Bank0配置。
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_REG_BANK_SEL, ICM42688P_REG_BANK_SEL_0);

    // 4. Power Management: Enable Accel and Gyro in Low-Noise mode.
    // 4. 电源管理：启用加速度计和陀螺仪的低噪声模式。
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_PWR_MGMT0, ICM42688P_PWR_MGMT0_ACCEL_MODE_LOW_NOISE | ICM42688P_PWR_MGMT0_GYRO_MODE_LOW_NOISE);

    // 5. Gyroscope Configuration: Set 2000dps FSR and 1kHz ODR.
    // 5. 陀螺仪配置：设置2000dps满量程和1kHz输出数据速率。
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_GYRO_CONFIG0, ICM42688P_GYRO_FS_SEL_2000_DPS | ICM42688P_GYRO_ODR_1KHZ_SET);

    // 6. Accelerometer Configuration: Set 16g FSR and 1kHz ODR.
    // 6. 加速度计配置：设置16g满量程和1kHz输出数据速率。
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_ACCEL_CONFIG0, ICM42688P_ACCEL_FS_SEL_16G | ICM42688P_ACCEL_ODR_1KHZ_SET);

    // Switch back to Bank 0 (切换回Bank 0)
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_REG_BANK_SEL, ICM42688P_REG_BANK_SEL_0);

    // 7.FIFO stream
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_FIFO_CONFIG, 0x40);
    // 8.FIFO_CONFIG1: enable sensors + hires + resume partial ---
    uint8_t val = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);
    // accel+gyro+temp+tmst+hires+ wm + resume
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_FIFO_CONFIG1, val);

    // 9. Power ON sensors ---
    ICM42688P_WriteReg(ICM42688P_REG_BANK0_PWR_MGMT0, ICM42688P_PWR_MGMT0_ACCEL_MODE_LOW_NOISE | ICM42688P_PWR_MGMT0_GYRO_MODE_LOW_NOISE);
    HAL_Delay(100);
}


/**
 * @brief  Reads the entire FIFO buffer from the ICM42688P via DMA.
 * @param  pData: Pointer to the buffer where the data will be stored.
 * @param  size: The number of bytes to read.
 * @brief  通过DMA从ICM42688P读取整个FIFO缓冲区。
 * @param  pData: 存储数据的缓冲区指针。
 * @param  size: 要读取的字节数。
 */
void ICM42688P_Read_FIFO_DMA(void) {
	uint8_t cnt[2];
	    uint16_t fifo_count;

    ICM42688P_ReadRegs(ICM42688P_REG_BANK0_FIFO_COUNTH, cnt, 2);

    fifo_count =(uint16_t) ((cnt[0] << 8) | cnt[1]);


    //uint16_t fifo_count = (fifo_count_h << 8) | fifo_count_l;


}
HAL_StatusTypeDef icm_read_dma(uint8_t reg, uint8_t *rx, uint16_t len)
{
    static uint8_t tx_addr;  // must persist until DMA complete
    tx_addr = reg | 0x80;    // MSB=1 for read

    Icm_CS_LOW();
    // Transmit address
    if (HAL_SPI_Transmit(&hspi1, &tx_addr, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    // Start DMA read for payload
    return HAL_SPI_Receive_DMA(&hspi1, rx, len);
}
uint8_t fifo_buf[1024]; // Adjust size

void icm_read_fifo(uint16_t bytes)
{
    icm_read_dma(0x30, fifo_buf, bytes);  // 0x30 = FIFO_DATA
}

/**
 * @brief  Reads single-shot accelerometer, gyroscope, and temperature data from registers.
 * @param  data: Pointer to the data structure to store the converted data.
 * @brief  通过寄存器单次读取加速度计、陀螺仪和温度数据。
 * @param  data: 存储转换后数据的结构体指针。
 */
void ICM42688P_ReadData(ICM42688P_Data_t* data) {

    // Read Temperature data (16-bit)
    // 读取温度数据（16位）
    uint8_t temp_h = ICM42688P_ReadReg(ICM42688P_REG_BANK0_TEMP_DATA1);
    uint8_t temp_l = ICM42688P_ReadReg(ICM42688P_REG_BANK0_TEMP_DATA0);
    int16_t raw_temp = (int16_t)((temp_h << 8) | temp_l);
    data->temperature_C = ICM42688P_ConvertTemp(raw_temp);
}
int16_t az;

int icm42688_read_all(ICM42688P_Data_t *data) {
    uint8_t buf[16];

    ICM42688P_ReadRegs(ICM42688P_REG_BANK0_TEMP_DATA1, buf, 16);

    data->temperature_C = ICM42688P_ConvertTemp((int16_t)((buf[0] << 8) | buf[1]));

    data->accel_x = ICM42688P_ConvertAccel((int16_t)((buf[2] << 8) | buf[3]),ICM42688P_ACCEL_16G_SENSITIVITY);
    data->accel_y = ICM42688P_ConvertAccel((int16_t)((buf[4] << 8) | buf[5]),ICM42688P_ACCEL_16G_SENSITIVITY);
    data->accel_z = ICM42688P_ConvertAccel((int16_t)((buf[6] << 8) | buf[7]),ICM42688P_ACCEL_16G_SENSITIVITY);

    data->gyro_x = ICM42688P_ConvertGyro((int16_t)((buf[8] << 8) | buf[9]),ICM42688P_GYRO_2000DPS_SENSITIVITY);
    data->gyro_y = ICM42688P_ConvertGyro((int16_t)((buf[10] << 8) | buf[11]),ICM42688P_GYRO_2000DPS_SENSITIVITY);
    data->gyro_z = ICM42688P_ConvertGyro((int16_t)((buf[12] << 8) | buf[13]),ICM42688P_GYRO_2000DPS_SENSITIVITY);

    data->dt = htim2.Instance->CNT - data->timestamp;

    data->timestamp = htim2.Instance->CNT;

    ICM42688P_Read_FIFO_DMA();


    return 0;
}

