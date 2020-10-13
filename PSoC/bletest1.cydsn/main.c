/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "accelerometer.h"
#include <stdio.h>

uint8 clickNotify = 0;
uint8 timerNotify = 0;
uint8 bleConnected = 0;
uint8 clicked = 1;
uint8 count = 0;
CYBLE_CONN_HANDLE_T connectionHandle;
static volatile uint32_t millisecondTimerCount = 0u;

/*******************************************************************************
* Function Name: _write
********************************************************************************
*
* Summary: 
*   This version of _write sends text via UART. printf is redirected to 
*   this function when GCC compiler is used to print data to terminal using UART. 
*   Note that depending on your compiler you may need to rewrite a different
*   function that is linked to printf.
* 
* Parameters:
*   file: This variable is not used.
*   *ptr: Pointer to the data which will be transfered through UART.
*   len: Length of the data to be transfered through UART.
* 
* Returns: 
*   returns the number of characters transferred using UART.
*
*******************************************************************************/
#if defined (__GNUC__) 
    
    /* Add an explicit reference to the floating point printf library to allow 
       use of floating point conversion specifier. */     
    asm (".global _printf_float"); 
 
    /* For GCC compiler revise _write() function for printf functionality */     
	int _write(int file, char *ptr, int len) 
    {       
        /* Warning suppression for unused function parameter */
		file = file;
		
		int i; 
        /* Character is sent via UART */
        for (i = 0; i < len; i++) 
        { 
            UART_UartPutChar(*(ptr++)); 
        }          
        return(len); 
    } 
#endif /* (__GNUC__) */



/************************************************************ 
 * This function is the handler for BLE stack events
 *
 * Arguments:
 * eventCode is the type of event that triggered the callback
 * eventParam contains parameters related to the event
 ************************************************************/
void Stack_Handler( uint32 eventCode, void *eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReq;
    
    switch ( eventCode )
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            /* Start advertising when the stack first turns on or on a disconnect event */
            /* Start advertising when the stack first turns on or on a disconnect event */    
            CyBle_GappStartAdvertisement( CYBLE_ADVERTISING_FAST );
            bleConnected = 0;
            break;
            
        case CYBLE_EVT_GATT_CONNECT_IND:
            /* When the connection event happens, save the connection handle to be used later */    
            connectionHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
            bleConnected = 1;
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
        /* If the client sends a write request to the CapSense Slider CCCD to enable or */
        /* disable notifications, read that value and set capsenseNotify appropriately */
        wrReq = (CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam;
            if( wrReq->handleValPair.attrHandle == CYBLE_TIMERSERVICE_TIMER_TIMERCCCD_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue( &wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED );
                /* Set notifications based on whether the client turned them on or off */
                timerNotify = wrReq->handleValPair.value.val[0];
            }
            /* Send a response so that the client can verify that the notification was set as requested */
            CyBle_GattsWriteRsp( connectionHandle );
            break;
            
        default:
            break;
    }
}

CY_ISR(timer1InterruptHandler)
{
    /* Read Status register in order to clear the sticky Terminal Count (TC) bit
     * in the status register. Note that the function is not called, but rather
     * the status is read directly.
     */
    //Timer_1_ReadStatus();
    Timer_1_ClearInterrupt(Timer_1_INTR_MASK_TC);

    // Add 1ms since this interrupt only happens every 1ms
    millisecondTimerCount += 1;
}

CY_ISR( Acc_Int_Pin_Handler ){
    UART_UartPutString("double click detected!\r\n");
    clicked = 1;
    Acc_Int_Pin_ClearInterrupt();
}

int main(void)
{
    
    ACC_ISR_StartEx( Acc_Int_Pin_Handler );
    
    /* Start BLE component and register the stack callback  event handler */
    
    UART_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */

    
    /* Enable the Interrupt component connected to Timer interrupt */
    Timer_1_Interrupt_StartEx(timer1InterruptHandler);

    /* Start the components */
    Timer_1_Start();

    // Reset count
    millisecondTimerCount = 0;
    CyBle_Start( Stack_Handler );
    
    uint32_t previousTimeInSeconds = millisecondTimerCount;
    uint32_t interval = 500;

    for(;;)
    {
        uint32_t currentTimeInSeconds = millisecondTimerCount;
        // Process any pending BLE events (both stack and IAS events) 
        CyBle_ProcessEvents();
        if(bleConnected){
            if (currentTimeInSeconds - previousTimeInSeconds > interval){
                previousTimeInSeconds = currentTimeInSeconds;
                count++;
                    CYBLE_GATTS_HANDLE_VALUE_IND_T timerHandle;
                    timerHandle.attrHandle = CYBLE_TIMERSERVICE_TIMER_CHAR_HANDLE;
                    // Point the finger position value 
                    timerHandle.value.val = &count;
                    // The finger postion is 1 byte 
                    timerHandle.value.len = 1;
                    
                if(timerNotify){
                    CyBle_GattsNotification( connectionHandle , &timerHandle );
                }
            }
        }
        
        
        /*
        if(clicked){
            if(bleConnected){
                CYBLE_GATTS_HANDLE_VALUE_IND_T clickHandle;
                clickHandle.attrHandle = CYBLE_DOUBLECLICK_CLICK_CHAR_HANDLE;
                // Point the finger position value 
                clickHandle.value.val = &clicked;
                // The finger postion is 1 byte 
                clickHandle.value.len = 1;
                
                if(clickNotify){
                    CyBle_GattsNotification( connectionHandle , &clickHandle );
                }
            }
            
            clicked = 0;
        }
        */
    }
}

/* [] END OF FILE */
