/******************************************************************************
* vcnl4200.h
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

/*PSoC platform includes*/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

/*VCNL4200 Registers*/
#define VCNL4200_ADDR 			0x51 /* VCNL4200 I2C Address */
#define VCNL4200_ALS_CONF		0x00 /* Ambient light configuration */
#define VCNL4200_ALS_THDH_LM	0x01 /* Ambient light threshold high */
#define VCNL4200_ALS_THDL_LM	0x02 /* Ambient light threshold low */
#define VCNL4200_PS_CONF1		0x03 /* Proximity configuration */
#define VCNL4200_PS_CONF3		0x04 /* Proximity configuration */
#define VCNL4200_PS_CANC_REG 	0x05 /* PS cancellation level setting */
#define VCNL4200_PS_THDL_LM		0x06 /* Proximity threshold low */
#define VCNL4200_PS_THDH_LM		0x07 /* Proximity threshold high */
#define VCNL4200_PS_DATA		0x08 /* Proximity data */
#define VCNL4200_AL_DATA		0x09 /* Ambient light data */
#define VCNL4200_WHITE_REG 		0x0A /* White output data */
#define VCNL4200_INT_FLAGS		0x0D /* Interrupt register */
#define VCNL4200_DEV_ID			0x0E /* Device ID, slave address and version */
#define VCNL4200_PRODUCT_ID		0x58 /* VCNL4200 Product ID value*/

/* ALS_CONF Bits*/
#define ALS_SD					(1 << 0) /* ALS shut down */
#define ALS_ON					(0 << 0) /* ALS power on, */
#define ALS_INT_EN				(1 << 1) /* ALS interrupt enable */
#define ALS_INT_DIS				(0 << 1) /* ALS interrupt disable */
#define ALS_PERS_1				(0 << 2)|(0 << 3) /*ALS interrupt persistence setting 1*/
#define ALS_PERS_2				(1 << 2)|(0 << 3) /*ALS interrupt persistence setting 2*/
#define ALS_PERS_4				(0 << 2)|(1 << 3) /*ALS interrupt persistence setting 4*/
#define ALS_PERS_8				(1 << 2)|(1 << 3) /*ALS interrupt persistence setting 8*/
#define ALS_INT_ALS				(0 << 5) /* ALS channel interrupt */
#define ALS_INT_WHT				(1 << 5) /* White channel interrupt */
#define ALS_IT_50				(0 << 6)|(0 << 7) /*ALS integration time setting 50 ms*/
#define ALS_IT_100				(1 << 6)|(0 << 7) /*ALS integration time setting 100 ms*/
#define ALS_IT_200				(0 << 6)|(1 << 7) /*ALS integration time setting 200 ms*/
#define ALS_IT_400				(1 << 6)|(1 << 7) /*ALS integration time setting 400 ms*/

/* PS_CONF1 and PS_CONF2 Bits */
#define PS_SD					(1 << 0) /* PS shut down */
#define PS_ON					(0 << 0) /* PS power on */
#define ALS_IT_1T				(0 << 1)|(0 << 2)|(0 << 3) /* PS integration time 1T */
#define ALS_IT_1_5T				(1 << 1)|(0 << 2)|(0 << 3) /* PS integration time 1.5T */
#define ALS_IT_2T				(0 << 1)|(1 << 2)|(0 << 3) /* PS integration time 2T */
#define ALS_IT_4T				(1 << 1)|(1 << 2)|(0 << 3) /* PS integration time 4T */
#define ALS_IT_8T				(0 << 1)|(0 << 2)|(1 << 3) /* PS integration time 8T */
#define ALS_IT_9T				(0 << 1)|(0 << 2)|(0 << 3) /* PS integration time 9T */
#define PS_PERS_1				(0 << 4)|(0 << 5) /*PS interrupt persistence setting 1*/
#define PS_PERS_2				(1 << 4)|(0 << 5) /*PS interrupt persistence setting 2*/
#define PS_PERS_3				(0 << 4)|(1 << 5) /*PS interrupt persistence setting 3*/
#define PS_PERS_4				(1 << 4)|(1 << 5) /*PS interrupt persistence setting 4*/
#define PS_DUTY_160				(0 << 6)|(0 << 7) /*PS IRED on/off duty ratio setting 1/160*/
#define PS_DUTY_320				(1 << 6)|(0 << 7) /*PS IRED on/off duty ratio setting 1/320*/
#define PS_DUTY_640				(0 << 6)|(1 << 7) /*PS IRED on/off duty ratio setting 1/640*/
#define PS_DUTY_1280			(1 << 6)|(1 << 7) /*PS IRED on/off duty ratio setting 1/1280*/
#define PS_INT_DIS				(0 << 8)|(0 << 9) /* Proximity interrupt configuration: interrupt disable */
#define PS_INT_CLOSING			(1 << 8)|(0 << 9) /* Proximity interrupt configuration: trigger by closing */
#define PS_INT_AWAY				(0 << 8)|(1 << 9) /* Proximity interrupt configuration: trigger by away */
#define PS_INT_CLOS_AW			(1 << 8)|(1 << 9) /* Proximity interrupt configuration: trigger by closing and away */
#define PS_HD_12				(0 << 11) /* PS output is 12 bits */
#define PS_HD_16				(1 << 11) /* PS output is 16 bits */

/* PS_CONF3 and PS_MS Bits */
#define PS_SC_EN					(1 << 0) /* Sunlight cancellation function enable */
#define PS_SC_DIS					(0 << 0) /* PS sunlight cancel enable setting */
#define PS_SC_ADV1					(0 << 1) /* Typical sunlight immunity */
#define PS_SC_ADV2					(1 << 1) /* 2 x typical sunlight immunity */
#define PS_TRIG_OFF					(0 << 2) /* No PS active force mode trigger */
#define PS_TRIG_ONE					(1 << 2) /* Trigger one time cycle */
#define PS_AF_DIS					(0 << 3) /* Active force mode disable (normal mode) */
#define PS_AF_EN					(1 << 3) /* Active force mode enable */
#define PS_SMART_DIS				(0 << 4) /* Proximity sensor smart persistence disable */
#define PS_SMART_EN					(1 << 4) /* Proximity sensor smart persistence enable */
#define PS_MPS_1					(0 << 5)|(0 << 6) /* Proximity multi pulse number 1 */
#define PS_MPS_2					(1 << 5)|(0 << 6) /* Proximity multi pulse number 2 */
#define PS_MPS_4					(0 << 5)|(1 << 6) /* Proximity multi pulse number 4 */
#define PS_MPS_8					(1 << 5)|(1 << 6) /* Proximity multi pulse number 8 */
#define LED_I_50					(0 << 8)|(0 << 9)|(0 << 10) /*IR LED current 50 mA */
#define LED_I_75					(1 << 8)|(0 << 9)|(0 << 10) /*IR LED current 75 mA */
#define LED_I_100					(0 << 8)|(1 << 9)|(0 << 10) /*IR LED current 100 mA */
#define LED_I_120					(1 << 8)|(1 << 9)|(0 << 10) /*IR LED current 120 mA */
#define LED_I_140					(0 << 8)|(0 << 9)|(1 << 10) /*IR LED current 140 mA */
#define LED_I_160					(1 << 8)|(0 << 9)|(1 << 10) /*IR LED current 160 mA */
#define LED_I_180					(0 << 8)|(1 << 9)|(1 << 10) /*IR LED current 180 mA */
#define LED_I_200					(1 << 8)|(1 << 9)|(1 << 10) /*IR LED current 200 mA */
#define PS_SPO_00					(0 << 11) /* Output is 00h in sunlight protect mode */
#define PS_SPO_FF					(1 << 11) /* Output is FFh in sunlight protect mode */
#define PS_SP_1						(0 << 12) /* Typical sunlight capability */
#define PS_SP_1_5					(1 << 12) /* 1.5 x typical sunlight capability */
#define PS_MS_NORMAL				(0 << 13) /* Proximity normal operation with interrupt function */
#define PS_MS_OUTPUT				(1 << 13) /* Proximity detection logic output mode enable */

_Bool VCNL4200_init(void);
uint16_t VCNL4200_Proximity(void);
uint16_t VCNL4200_Ambient(void);
