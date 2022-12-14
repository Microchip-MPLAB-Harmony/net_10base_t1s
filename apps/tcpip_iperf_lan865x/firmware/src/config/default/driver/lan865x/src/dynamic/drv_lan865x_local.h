/*******************************************************************************
  LAN865X Driver Local Interface

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan865x_local.h

  Summary:
    Local variables used in LAN865x driver

  Description:
    Defines structures holding local variables
    
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*****************************************************************************
 Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.

Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software 
and any derivatives exclusively with Microchip products. It is your 
responsibility to comply with third party license terms applicable to your 
use of third party software (including open source software) that may 
accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE 
FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN 
ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************/

// DOM-IGNORE-END
#ifndef DRV_LAN865X_LOCAL_H_
#define DRV_LAN865X_LOCAL_H_

/******************************************************************************
*  INCLUDES
******************************************************************************/
#include "configuration.h"
#include "osal/osal.h"
#include "system_config.h"
#include "tc6/tc6.h"
#include "tcpip/src/tcpip_private.h"
#include "driver/lan865x/drv_lan865x.h"

/******************************************************************************
*  STRUCTURE
******************************************************************************/

typedef struct _DRV_LAN865X_RX_PACKET_INFO {
    //DRV_LAN865X_RX_PACKET_STATES state;
    struct _DRV_LAN865X_RX_PACKET_INFO* next;
    TCPIP_MAC_PACKET* macPkt;
    struct _DRV_LAN865X_DriverInfo* pDrvInst;
} DRV_LAN865X_RX_PACKET_INFO;

typedef enum _DRV_LAN865X_DRIVER_INIT_STATE
{
    DRV_LAN865X_INITSTATE_RESET,
    DRV_LAN865X_INITSTATE_MEMMAP,
    DRV_LAN865X_INITSTATE_TRIM,
    DRV_LAN865X_INITSTATE_SETREGS,
    /* Shall always be last: */
    DRV_LAN865X_INITSTATE_FINISHED
} DRV_LAN865X_DRIVER_INIT_STATE;

typedef struct _DRV_LAN865X_DriverInfo {
    /* The mutex to protect the */
    OSAL_MUTEX_HANDLE_TYPE drvMutex;
    /* A copy of the driver configuration*/
    DRV_LAN865X_Configuration drvCfg;
    /* A copy of the stack configuration*/
    TCPIP_MAC_MODULE_CTRL stackCfg;
    
    TCPIP_MAC_RX_STATISTICS rxStats;
    TCPIP_MAC_TX_STATISTICS txStats;

    // SPI Driver info
    DRV_HANDLE spiClientHandle;
    uintptr_t spiTransferHandle;

    SYS_STATUS state;

    TCPIP_MAC_PARAMETERS stackParameters;
    TCPIP_MAC_EVENT eventMask;
    TCPIP_MAC_EVENT currentEvents;

    DRV_LAN865X_RX_PACKET_INFO rxDescriptors;

    PROTECTED_SINGLE_LIST rxFreePackets;
    PROTECTED_SINGLE_LIST rxWaitingForPickupPackets;

    DRV_LAN865X_DRIVER_INIT_STATE initState;

    uint32_t magic; /** Enables checking if structure is valid */
    uint32_t initTimer;
    uint32_t plcaTimer;
    uint32_t initReadVal;
    uint32_t unlockExtTime;

    TC6_t* drvTc6;
    int8_t initEfuseA4;
    int8_t initEfuseA8;
    uint8_t initSubState;
    uint8_t drvTc6Inst;

    uint8_t numClients;
    uint8_t index;
    bool exclusiveMode;

    bool rxPacketInvalid;
    bool inUse;
    bool needService;
    bool stackCfgReady;
    bool spiBusy;
    bool plcaStatus;
    bool extBlock;
} DRV_LAN865X_DriverInfo;


typedef struct _DRV_LAN865X_ClientInfo {
    //Enables checking if structure is valid
    uint32_t clientMagic;
    DRV_LAN865X_DriverInfo* pDrvInst;
} DRV_LAN865X_ClientInfo;

/******************************************************************************
*  FUNCTION PROTOTYPES
******************************************************************************/
// LAN865X debugging
void DRV_LAN865X_Assert(bool cond, const char* message, int lineNo);

#endif