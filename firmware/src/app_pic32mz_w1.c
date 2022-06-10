/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_pic32mz_w1.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdio.h>
#include "definitions.h"

#include "system/console/sys_console.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw_sta.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw_authctx.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw_bssctx.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw_bssfind.h"
#include "driver/wifi/pic32mzw1/include/wdrv_pic32mzw_common.h"
#include "driver/wifi/pic32mzw1/include/drv_pic32mzw1_crypto.h"

#include "app_pic32mz_w1.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
typedef enum 
{
    OPEN = 1,
    WPAWPA2MIXED,
    WEP,
    WPA2WPA3MIXED,
    WIFI_AUTH_MAX
} WIFI_AUTH;

typedef struct wifiConfiguration {
    WDRV_PIC32MZW_AUTH_CONTEXT authCtx;
    WDRV_PIC32MZW_BSS_CONTEXT bssCtx;
} wifiConfig;
wifiConfig g_wifiConfig;

typedef struct wifiCredentials {
    uint8_t ssid[WDRV_PIC32MZW_MAX_SSID_LEN];
    uint8_t auth;
    uint8_t key[WDRV_PIC32MZW_MAX_PSK_PASSWORD_LEN];
} wifiCred;
wifiCred wifi;


/* Default STA credentials */
#define APP_STA_DEFAULT_SSID        "wsn-2g"
#define APP_STA_DEFAULT_PASSPHRASE  "brucenegley"
#define APP_STA_DEFAULT_AUTH        WPAWPA2MIXED

/* Wi-Fi credentials status*/
#define CREDENTIALS_UNINITIALIZED       0
#define CREDENTIALS_VALID               1
#define CREDENTIALS_INVALID             2

#define SET_WIFI_CREDENTIALS(val)   appData.validCrednetials = val
#define CHECK_WIFI_CREDENTIALS()     appData.validCrednetials
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_PIC32MZ_W1_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_PIC32MZ_W1_DATA app_pic32mz_w1Data;

extern uint8_t* _WDRV_PIC32MZW_Task_Stk_Ptr;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

/* Wi-Fi connect callback */
static void wifiConnectCallback(DRV_HANDLE handle, WDRV_PIC32MZW_ASSOC_HANDLE assocHandle, WDRV_PIC32MZW_CONN_STATE currentState)
{
    switch (currentState) {
        case WDRV_PIC32MZW_CONN_STATE_DISCONNECTED:
            SYS_CONSOLE_MESSAGE("WiFi Reconnecting\r\n");            
            app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_RECONNECT;
            break;
        case WDRV_PIC32MZW_CONN_STATE_CONNECTED:
            SYS_CONSOLE_MESSAGE("WiFi Connected\r\n");
            break;
        case WDRV_PIC32MZW_CONN_STATE_FAILED:
            SYS_CONSOLE_MESSAGE("WiFi connection failed\r\n");
            app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_RECONNECT;        
            break;
        case WDRV_PIC32MZW_CONN_STATE_CONNECTING:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
/* Store Wi-Fi configurations to global g_wifiConfig struct */
bool APP_WifiConfig(char *ssid, char *pass, WIFI_AUTH auth, uint8_t channel)
{
    bool ret = true;
    uint8_t ssidLength = strlen((const char *) ssid);
    uint8_t pskLength = strlen(pass);

    WDRV_PIC32MZW_BSSCtxSetChannel(&g_wifiConfig.bssCtx, (WDRV_PIC32MZW_CHANNEL_ID)channel);
    //SYS_CONSOLE_PRINT("SSID is %s \r\n", ssid);

    if (WDRV_PIC32MZW_STATUS_OK == WDRV_PIC32MZW_BSSCtxSetSSID(&g_wifiConfig.bssCtx, (uint8_t * const) ssid, ssidLength)) {
        switch (auth) {
            case OPEN:
            {
                if (WDRV_PIC32MZW_STATUS_OK != WDRV_PIC32MZW_AuthCtxSetOpen(&g_wifiConfig.authCtx)) {
                    SYS_CONSOLE_MESSAGE("APP_WFI: ERROR- Unable to set Authentication\r\n");
                    ret = false;
                }
                break;
            }

            case WPAWPA2MIXED:
            {
                if (WDRV_PIC32MZW_STATUS_OK != WDRV_PIC32MZW_AuthCtxSetPersonal(&g_wifiConfig.authCtx, (uint8_t *) pass, pskLength, WDRV_PIC32MZW_AUTH_TYPE_WPAWPA2_PERSONAL)) {
                    SYS_CONSOLE_MESSAGE("ERROR - Unable to set authentication to WPAWPA2 MIXED\r\n");
                    ret = false;
                }
                break;
            }
            
            case WEP:
            {
                //TODO
                break;
            }
            default:
            {
                SYS_CONSOLE_MESSAGE("ERROR-Set Authentication type");
                ret = false;
            }
        }
    }
    return ret;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_PIC32MZ_W1_Initialize ( void )

  Remarks:
    See prototype in app_pic32mz_w1.h.
 */

void APP_PIC32MZ_W1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_PIC32MZ_W1_Tasks ( void )

  Remarks:
    See prototype in app_pic32mz_w1.h.
 */

void APP_PIC32MZ_W1_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_pic32mz_w1Data.wlanTaskState )
    {
        /* Application's initial state. */
        case APP_PIC32MZ_W1_STATE_INIT:
        {               
            //SYS_CONSOLE_MESSAGE("APP_WIFI Ready\r\n");
            SYS_STATUS status = WDRV_PIC32MZW_Status(sysObj.drvWifiPIC32MZW1);
            if (SYS_STATUS_READY == status) {
                app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_INIT_READY;                
            }
            else
            {
                //SYS_CONSOLE_PRINT("[state = %d]\r\n", status);
            }
            break;
        }
        
        case APP_PIC32MZ_W1_STATE_INIT_READY:
        {            
            app_pic32mz_w1Data.wdrvHandle = WDRV_PIC32MZW_Open(0, 0);
            if (DRV_HANDLE_INVALID != app_pic32mz_w1Data.wdrvHandle) {
                SYS_CONSOLE_MESSAGE("Check credentials!\r\n");
                app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_CHECK_CREDENTIALS;
            }
            break;
        }

        /* Check on Wi-Fi from USB MSD */
        case APP_PIC32MZ_W1_STATE_CHECK_CREDENTIALS:
        {
            SYS_CONSOLE_MESSAGE("Go to normal mode\r\n"); 
#if 0        
bufferIp.data = (const uint8_t *)"abc";
bufferIp.data_len = 3;

            for(int i = 0; i < 32; i++)
                SYS_CONSOLE_PRINT("%02X ", digest[i]);
            SYS_CONSOLE_MESSAGE("\r\n");
            DRV_PIC32MZW1_Crypto_HMAC((const uint8_t *)"secret", 8, (const buffer_t *)&bufferIp, 1, (uint8_t *)digest);
            for(int i = 0; i < 32; i++)
                SYS_CONSOLE_PRINT("%02X ", digest[i]);
            SYS_CONSOLE_MESSAGE("\r\n");
#endif            
            app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_CONFIG;              
            break; 
        }
        
        /* Configure and connect */
        case APP_PIC32MZ_W1_STATE_CONFIG:
        {
            SYS_CONSOLE_PRINT("pic32mzwTaskStack = %X\n", _WDRV_PIC32MZW_Task_Stk_Ptr);
            SYS_CONSOLE_PRINT("Connecting to Wi-Fi (%s)\r\n",APP_STA_DEFAULT_SSID);            
            if (APP_WifiConfig((char*)APP_STA_DEFAULT_SSID, 
                                (char*)APP_STA_DEFAULT_PASSPHRASE, 
                                (WIFI_AUTH)APP_STA_DEFAULT_AUTH, 
                                WDRV_PIC32MZW_CID_ANY)) 
            {                
                if (WDRV_PIC32MZW_STATUS_OK == WDRV_PIC32MZW_BSSConnect(app_pic32mz_w1Data.wdrvHandle, 
                                                                        &g_wifiConfig.bssCtx, 
                                                                        &g_wifiConfig.authCtx, 
                                                                        wifiConnectCallback)) 
                {
                    app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_IDLE;
                    break;
                }
        
            }
            SYS_CONSOLE_MESSAGE("Failed connecting to Wi-Fi\r\n");
            app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_ERROR;
            break;
        }
     
        
        /* Idle */
        case APP_PIC32MZ_W1_STATE_IDLE:
        {
            break;
        }
        
        /* Reconnect */
        case APP_PIC32MZ_W1_STATE_RECONNECT:
        {            
            WDRV_PIC32MZW_Close(app_pic32mz_w1Data.wdrvHandle);
            app_pic32mz_w1Data.wlanTaskState = APP_PIC32MZ_W1_STATE_INIT;
            break;
        }      
        
        /* Error */
        case APP_PIC32MZ_W1_STATE_ERROR:
        {
            break;
        }
        
        default:
        {
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
