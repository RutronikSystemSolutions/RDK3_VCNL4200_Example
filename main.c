/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the RDK3 VCNL4200 Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*  Created on: 2024-09-17
*  Company: Rutronik Elektronische Bauelemente GmbH
*  Address: Jonavos g. 30, Kaunas 44262, Lithuania
*  Author: GDR
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "vcnl4200.h"

#define I2C_TIMEOUT_MS	10

void handle_error(void);

cyhal_i2c_t I2C_scb3;
cyhal_i2c_cfg_t I2C_cfg;

int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
    	handle_error();
    }

    /*Enable debug output via KitProg UART*/
    result = cy_retarget_io_init( KITPROG_TX, KITPROG_RX, CY_RETARGET_IO_BAUDRATE);
    if (result != CY_RSLT_SUCCESS)
    {
        handle_error();
    }
    printf("\x1b[2J\x1b[;H");

    /*Initialise LEDs*/
    result = cyhal_gpio_init( LED1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {handle_error();}
    result = cyhal_gpio_init( LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {handle_error();}
    result = cyhal_gpio_init( LED3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {handle_error();}

    /*Initialise Charger Control Pin*/
    result = cyhal_gpio_init( CHR_DIS, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    if (result != CY_RSLT_SUCCESS)
    {CY_ASSERT(0);}

    /*Enable interrupts*/
    __enable_irq();

    /*I2C Peripheral Configuration*/
    I2C_cfg.is_slave = false;
    I2C_cfg.address = 0;
    I2C_cfg.frequencyhal_hz = 400000UL;
    result = cyhal_i2c_init(&I2C_scb3, ARDU_SDA, ARDU_SCL, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
    	handle_error();
    }
    result = cyhal_i2c_configure(&I2C_scb3, &I2C_cfg);
    if (result != CY_RSLT_SUCCESS)
    {
        handle_error();
    }
    printf("I2C peripheral ready.\r\n");
    CyDelay(100);

    /* Initialise the VCNL4200 */
    if(!VCNL4200_init())
    {
    	printf("VCNL4200 initialisation failure.\r\n");
    	CY_ASSERT(0);
    }

    uint16_t prox = 0;
    uint16_t amb = 0;
    for(;;)
    {
    	CyDelay(120);

    	cyhal_gpio_write(LED2, CYBSP_LED_STATE_ON);
    	prox = VCNL4200_Proximity();
    	amb = VCNL4200_Ambient();
    	cyhal_gpio_write(LED2, CYBSP_LED_STATE_OFF);

    	printf("Proximity: %d, Ambient: %d\r\n", (int)prox, (int)amb);

    }
}

void handle_error(void)
{
     /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
}
/* [] END OF FILE */
