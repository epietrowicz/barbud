/*******************************************************************************
* File Name: ACC_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_ACC_ISR_H)
#define CY_ISR_ACC_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ACC_ISR_Start(void);
void ACC_ISR_StartEx(cyisraddress address);
void ACC_ISR_Stop(void);

CY_ISR_PROTO(ACC_ISR_Interrupt);

void ACC_ISR_SetVector(cyisraddress address);
cyisraddress ACC_ISR_GetVector(void);

void ACC_ISR_SetPriority(uint8 priority);
uint8 ACC_ISR_GetPriority(void);

void ACC_ISR_Enable(void);
uint8 ACC_ISR_GetState(void);
void ACC_ISR_Disable(void);

void ACC_ISR_SetPending(void);
void ACC_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ACC_ISR ISR. */
#define ACC_ISR_INTC_VECTOR            ((reg32 *) ACC_ISR__INTC_VECT)

/* Address of the ACC_ISR ISR priority. */
#define ACC_ISR_INTC_PRIOR             ((reg32 *) ACC_ISR__INTC_PRIOR_REG)

/* Priority of the ACC_ISR interrupt. */
#define ACC_ISR_INTC_PRIOR_NUMBER      ACC_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ACC_ISR interrupt. */
#define ACC_ISR_INTC_SET_EN            ((reg32 *) ACC_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ACC_ISR interrupt. */
#define ACC_ISR_INTC_CLR_EN            ((reg32 *) ACC_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ACC_ISR interrupt state to pending. */
#define ACC_ISR_INTC_SET_PD            ((reg32 *) ACC_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ACC_ISR interrupt. */
#define ACC_ISR_INTC_CLR_PD            ((reg32 *) ACC_ISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_ACC_ISR_H */


/* [] END OF FILE */
