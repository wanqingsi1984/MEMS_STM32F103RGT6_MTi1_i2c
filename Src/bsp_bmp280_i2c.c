/**
  ******************************************************************************
  * @file    BMP280.c
  * @author  Waveshare Team
  * @version V1.0
  * @date    12-12-2016
  * @brief   This file includes the BMP280 driver functions
  ******************************************************************************
  */
#include "bsp_bmp280.h"

uint16_t dig_T1;  
int16_t dig_T2; 
int16_t dig_T3;  
uint16_t dig_P1; 
int16_t dig_P2;  
int16_t dig_P3; 
int16_t dig_P4; 
int16_t dig_P5; 
int16_t dig_P6; 
int16_t dig_P7; 
int16_t dig_P8; 
int16_t dig_P9; 
int32_t t_fine; 

int32_t gs32Pressure0 = MSLP; 

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD, _oss;  
uint16_t AC4, AC5, AC6;
int32_t B5, UT, UP, Pressure0 = MSLP; 
int32_t PressureVal = 0, TemperatureVal = 0, AltitudeVal = 0;
int BMP280_COUNTER = 0;
double PRESSURE_FILTER[8] = {0.0};

#ifdef BUS_I2C

void BMP280_ReadReg_i2c(uint8_t RegAddr, uint8_t Num, uint8_t *pBuffer) 
{
  HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, (uint16_t)RegAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, Num, 100);
	
}
void BMP280_WriteReg_i2c(uint8_t RegAddr, uint8_t Val) 
{
	HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR,(uint16_t)RegAddr, I2C_MEMADD_SIZE_8BIT, &Val, 1, 100);
}

void BMP280_Read_Calibration_i2c(void)
{
	uint8_t lsb, msb; 
	
	/* read the temperature calibration parameters */ 

	BMP280_ReadReg_i2c(BMP280_DIG_T1_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_T1_MSB_REG, 1, &msb);
	dig_T1 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_T2_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_T2_MSB_REG, 1, &msb);
	dig_T2 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_T3_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_T3_MSB_REG, 1, &msb);
	dig_T3 = msb << 8 | lsb;  
	
	/* read the pressure calibration parameters */  
	BMP280_ReadReg_i2c(BMP280_DIG_P1_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P1_MSB_REG, 1, &msb);    
	dig_P1 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P2_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P2_MSB_REG, 1, &msb);      
	dig_P2 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P3_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P3_MSB_REG, 1, &msb);
	dig_P3 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P4_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P4_MSB_REG, 1, &msb);    
	dig_P4 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P5_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P5_MSB_REG, 1, &msb);     
	dig_P5 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P6_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P6_MSB_REG, 1, &msb);     
	dig_P6 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P7_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P7_MSB_REG, 1, &msb);      
	dig_P7 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P8_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P8_MSB_REG, 1, &msb);     
	dig_P8 = msb << 8 | lsb;  
	BMP280_ReadReg_i2c(BMP280_DIG_P9_LSB_REG, 1, &lsb);
	BMP280_ReadReg_i2c(BMP280_DIG_P9_MSB_REG, 1, &msb);      
	dig_P9 = msb << 8 | lsb; 

}
/* Returns temperature in DegC, double precision. Output value of "1.23"equals 51.23 DegC. */  
double BMP280_Compensate_Temperature_i2c(int32_t adc_T)  
{  
	double var1, var2, temperature;  
	var1 = (((double) adc_T) / 16384.0 - ((double) dig_T1) / 1024.0) * ((double) dig_T2); 
	var2 = ((((double) adc_T) / 131072.0 - ((double) dig_T1) / 8192.0)  * (((double) adc_T) / 131072.0  
					- ((double) dig_T1) / 8192.0)) * ((double) dig_T3);  
	t_fine = (int32_t) (var1 + var2);  
	temperature = (var1 + var2) / 5120.0;  
	
	return temperature;  
}  
  
  
/* Returns pressure in Pa as double. Output value of "6386.2"equals 96386.2 Pa = 963.862 hPa */  
double BMP280_Compensate_Pressure_i2c(int32_t adc_P)  
{  
	double var1, var2, pressure;  

	var1 = ((double)t_fine / 2.0) - 64000.0; 
	var2 = var1 * var1 * ((double) dig_P6) / 32768.0;  
	var2 = var2 + var1 * ((double) dig_P5) * 2.0;  
	var2 = (var2 / 4.0) + (((double) dig_P4) * 65536.0);  
	var1 = (((double) dig_P3) * var1 * var1 / 524288.0  + ((double) dig_P2) * var1) / 524288.0;  
	var1 = (1.0 + var1 / 32768.0) * ((double) dig_P1);  

	if (var1 < 1.0E-5) {  
		return 0; // avoid exception caused by division by zero  
	}  

	pressure = 1048576.0 - (double) adc_P;  
	pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;  
	var1 = ((double) dig_P9) * pressure * pressure / 2147483648.0;  
	var2 = pressure * ((double) dig_P8) / 32768.0;  
	pressure = pressure + (var1 + var2 + ((double) dig_P7)) / 16.0;  

	return pressure;  
}  
void BMP280_Get_Temperature_And_Pressure_i2c(double *temperature, double *pressure)  
{  
	uint8_t lsb, msb, xlsb;  
	int32_t adc_P,adc_T;
    
	BMP280_ReadReg_i2c(BMP280_TEMP_XLSB_REG, 1, &xlsb);
	BMP280_ReadReg_i2c(BMP280_TEMP_LSB_REG,  1, &lsb);
	BMP280_ReadReg_i2c(BMP280_TEMP_MSB_REG,  1, &msb);
	adc_T = (msb << 12) | (lsb << 4) | (xlsb >> 4); 
	//adc_T = 415148;
	* temperature = BMP280_Compensate_Temperature_i2c(adc_T);

	BMP280_ReadReg_i2c(BMP280_PRESS_XLSB_REG, 1, &xlsb);
	BMP280_ReadReg_i2c(BMP280_PRESS_LSB_REG,  1, &lsb);
	BMP280_ReadReg_i2c(BMP280_PRESS_MSB_REG,  1, &msb);
	adc_P = (msb << 12) | (lsb << 4) | (xlsb >> 4); 
	//adc_P = 51988;
	* pressure = BMP280_Compensate_Pressure_i2c(adc_P);     
} 

void BMP280_CalAvgValue_i2c(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *pOutVal)
{	
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
	*pIndex &= 0x07;
  	
	*pOutVal = 0;
	for(i = 0; i < 8; i ++) 
	{
		*pOutVal += *(pAvgBuffer + i);
	}
	*pOutVal >>= 3;
}

void BMP280_CalculateAbsoluteAltitude_i2c(int32_t *pAltitude, int32_t PressureVal)
{
	*pAltitude = 4433000 * (1 - pow((PressureVal / (float)gs32Pressure0), 0.1903)); 
}

uint8_t BMP280_Initi_i2c(void)
{
	uint8_t u8Ret = BMP280_RET_OK;
	uint8_t u8ChipID, u8CtrlMod, u8Status;
	
	BMP280_ReadReg_i2c(BMP280_REGISTER_CHIPID, 1, &u8ChipID);
	BMP280_ReadReg_i2c(BMP280_REGISTER_CONTROL, 1, &u8CtrlMod);
	BMP280_ReadReg_i2c(BMP280_REGISTER_STATUS, 1, &u8Status);

	if(u8ChipID == 0x58)
	{
		//printf("\r\nBMP280 init successful : ChipID [0x%x] CtrlMod [0x%x] Status [0x%x] \r\n", u8ChipID,u8CtrlMod,u8Status);
		BMP280_WriteReg_i2c(BMP280_REGISTER_CONTROL, 0xFF); 
		BMP280_WriteReg_i2c(BMP280_REGISTER_CONFIG, 0x14); 
		BMP280_Read_Calibration_i2c();

	}
	else
	{
		u8Ret = BMP280_RET_NG;
	}
	return  u8Ret;
	
	
}


void BMP280_CalTemperatureAndPressureAndAltitude_i2c(int32_t *temperature, int32_t *pressure, int32_t *Altitude)
{
	double CurPressure, CurTemperature;
	int32_t CurAltitude;
	double sum = 0;
  static BMP280_AvgTypeDef BMP280_Filter[3];
      
	BMP280_Get_Temperature_And_Pressure_i2c(&CurTemperature, &CurPressure);
  BMP280_CalAvgValue_i2c(&BMP280_Filter[0].Index, BMP280_Filter[0].AvgBuffer, (int32_t)(CurPressure), pressure);

//	if(CurPressure>1.0)
//	{
//		PRESSURE_FILTER[BMP280_COUNTER % 8] = CurPressure;
//	}
//	BMP280_COUNTER++;
//   
//	//   printf("\r\n");
//	for(int i=0;i<8;i++)
//	{
//		sum += PRESSURE_FILTER[i];
//  //      printf("%12.6f ", PRESSURE_FILTER[i]);
//	}
//	*pressure = (int32_t) (sum/8.0);
//  //     printf("\r\n");  
  
  BMP280_CalculateAbsoluteAltitude_i2c(&CurAltitude, (*pressure));
  BMP280_CalAvgValue_i2c(&BMP280_Filter[1].Index, BMP280_Filter[1].AvgBuffer, CurAltitude, Altitude);
  BMP280_CalAvgValue_i2c(&BMP280_Filter[2].Index, BMP280_Filter[2].AvgBuffer, (int32_t)CurTemperature*10, temperature);
    
  return;
	
}

#endif
