/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "interrupts.h"
#include "definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void CORE_TIMER_InterruptHandler( void );
void UART3_FAULT_InterruptHandler( void );
void UART3_RX_InterruptHandler( void );
void UART3_TX_InterruptHandler( void );
void WDRV_PIC32MZW_TasksRFSMCISR( void );
void WDRV_PIC32MZW_TasksRFMACISR( void );
void WDRV_PIC32MZW_TasksRFTimer0ISR( void );
void DRV_BA414E_InterruptHandler( void );
void DRV_BA414E_ErrorInterruptHandler( void );

void enable_interrupts(void)
{
    SYS_INT_SourceEnable(INT_SOURCE_CORE_TIMER);
    SYS_INT_SourceEnable(INT_SOURCE_UART3_FAULT);
    SYS_INT_SourceEnable(INT_SOURCE_UART3_RX);
    SYS_INT_SourceEnable(INT_SOURCE_UART3_TX);
    SYS_INT_SourceEnable(INT_SOURCE_RFTM0);
    SYS_INT_SourceEnable(INT_SOURCE_RFMAC);
    SYS_INT_SourceEnable(INT_SOURCE_RFTM0);    
    SYS_INT_SourceEnable(INT_SOURCE_CRYPTO1);
    SYS_INT_SourceEnable(INT_SOURCE_CRYPTO1_FAULT);
    
    
}

void disable_interrupts(void)
{
    SYS_INT_SourceDisable(INT_SOURCE_CORE_TIMER);
    SYS_INT_SourceDisable(INT_SOURCE_UART3_FAULT);
    SYS_INT_SourceDisable(INT_SOURCE_UART3_RX);
    SYS_INT_SourceDisable(INT_SOURCE_UART3_TX);
    SYS_INT_SourceDisable(INT_SOURCE_RFTM0);
    SYS_INT_SourceDisable(INT_SOURCE_RFMAC);
    SYS_INT_SourceDisable(INT_SOURCE_RFTM0);
    SYS_INT_SourceDisable(INT_SOURCE_CRYPTO1);
    SYS_INT_SourceDisable(INT_SOURCE_CRYPTO1_FAULT);
}

/* All the handlers are defined here.  Each will call its PLIB-specific function. */
void __ISR(_CORE_TIMER_VECTOR, ipl1SAVEALL) CORE_TIMER_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    CORE_TIMER_InterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}


void __ISR(_UART3_FAULT_VECTOR, ipl1SAVEALL) UART3_FAULT_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    UART3_FAULT_InterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_UART3_RX_VECTOR, ipl1SAVEALL) UART3_RX_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    UART3_RX_InterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_UART3_TX_VECTOR, ipl1SAVEALL) UART3_TX_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    UART3_TX_InterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_RFSMC_VECTOR, ipl1SAVEALL) RFSMC_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    WDRV_PIC32MZW_TasksRFSMCISR();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_RFMAC_VECTOR, ipl1SAVEALL) RFMAC_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    WDRV_PIC32MZW_TasksRFMACISR();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_RFTM0_VECTOR, ipl1SAVEALL) RFTM0_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    WDRV_PIC32MZW_TasksRFTimer0ISR();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_CRYPTO1_VECTOR, ipl1SAVEALL) CRYPTO1_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    DRV_BA414E_InterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}

void __ISR(_CRYPTO1_FAULT_VECTOR, ipl1SAVEALL) CRYPTO1_FAULT_Handler (void)
{
    /* Call ThreadX context save. */
    _tx_thread_context_save();

    DRV_BA414E_ErrorInterruptHandler();

    /* Call ThreadX context restore. */
    _tx_thread_context_restore();
}




/*******************************************************************************
 End of File
*/
