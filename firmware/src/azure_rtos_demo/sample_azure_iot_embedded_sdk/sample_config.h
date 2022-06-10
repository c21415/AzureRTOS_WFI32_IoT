/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef SAMPLE_CONFIG_H
#define SAMPLE_CONFIG_H

#ifdef __cplusplus
extern   "C" {
#endif

/*** Azure IoT embedded C SDK Configuration ***/
#define MODULE_ID              ""
/* Required when DPS is used.  */
#define ENABLE_DPS_SAMPLE
#define ENDPOINT                       "global.azure-devices-provisioning.net"
#define ID_SCOPE                       "0ne00551F61"
#define REGISTRATION_ID                "wfi32-iot"
#define SAMPLE_MAX_BUFFER              (256)
            
    
#define DEVICE_SYMMETRIC_KEY           "nKnohy6EgHlz44fjdmgG5IhDWh9PjXUUPhmZFyPAoIQ="           
    
#define NX_AZURE_IOT_STACK_SIZE                (4096)
#define NX_AZURE_IOT_THREAD_PRIORITY           (4) 
#define SAMPLE_STACK_SIZE                      (4096)
#define SAMPLE_THREAD_PRIORITY                 (16)
#define MAX_PROPERTY_COUNT                     (2)

#ifdef __cplusplus
}
#endif
#endif /* SAMPLE_CONFIG_H */
