/******************************************************************************
* vcnl4200.c
*
*  Created on: 2024-09-16
*      Author: GDR
*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*
*******************************************************************************/

#include "vcnl4200.h"

#define COM_TIMEOUT_MS	5U

/*I2C hardware must be initialised before use*/
extern cyhal_i2c_t I2C_scb3;

static _Bool VCNL4200_write(uint8_t reg, uint16_t data)
{
	cy_rslt_t result;
	uint8_t data_8[3] = {reg,data&0xFF,(data>>8)&0xFF};

	result = cyhal_i2c_master_write( &I2C_scb3, VCNL4200_ADDR, data_8, 3, COM_TIMEOUT_MS, true);
	if (result != CY_RSLT_SUCCESS)
	{
		return false;
	}

	return true;
}

static uint16_t VCNL4200_read(uint8_t reg)
{
	cy_rslt_t result;
	uint8_t data_8[2] = {0};
	uint16_t data_16 = 0;

	result = cyhal_i2c_master_write( &I2C_scb3, VCNL4200_ADDR, &reg, 1, COM_TIMEOUT_MS, false);
	if (result != CY_RSLT_SUCCESS)
	{
		return 0;
	}
	result = cyhal_i2c_master_read( &I2C_scb3, VCNL4200_ADDR, data_8, 2, COM_TIMEOUT_MS, true);
	if (result != CY_RSLT_SUCCESS)
	{
		return 0;
	}

	data_16 = data_8[0];
	data_16 |= data_8[1] << 8;

	return data_16;
}

_Bool VCNL4200_init(void)
{
	uint16_t read_reg;

	/*Check if it is VCNL4200 first*/
	read_reg = VCNL4200_read(VCNL4200_DEV_ID);
	if((read_reg&0x00FF) != VCNL4200_PRODUCT_ID)
	{
		return false;
	}

	if(!VCNL4200_write(VCNL4200_ALS_CONF, ALS_ON | ALS_INT_DIS | ALS_PERS_1 | ALS_INT_ALS | ALS_IT_50))
	{
		return false;
	}

	if(!VCNL4200_write(VCNL4200_PS_CONF1, PS_ON | ALS_IT_8T | PS_PERS_1 | PS_DUTY_640 | PS_INT_CLOS_AW | PS_HD_16))
	{
		return false;
	}

	if(!VCNL4200_write(VCNL4200_PS_CONF3, PS_SC_DIS | PS_SC_ADV1 | PS_TRIG_OFF | PS_AF_DIS | PS_SMART_DIS | PS_MPS_8 | LED_I_120 | PS_SPO_FF | PS_SP_1 | PS_MS_OUTPUT ))
	{
		return false;
	}

	return true;
}

uint16_t VCNL4200_Proximity(void)
{
	return VCNL4200_read(VCNL4200_PS_DATA);
}

uint16_t VCNL4200_Ambient(void)
{
	return VCNL4200_read(VCNL4200_AL_DATA);
}


