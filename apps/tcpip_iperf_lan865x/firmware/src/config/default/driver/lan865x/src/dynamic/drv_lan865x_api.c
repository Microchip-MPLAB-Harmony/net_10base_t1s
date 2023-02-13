//DOM-IGNORE-BEGIN
/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END
/*******************************************************************************
  LAN865X Driver Public Interface

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan865x_api.c

  Summary:
    Interface driver for binding LAN865X MAC-Phy to Microchip TCP/IP stack

  Description:
    This file combine the TC6 low level driver with all the necessary parts to be compatible with the MCHP TCP/IP stack

*******************************************************************************/

#include "configuration.h"
#include "definitions.h"
#include "drv_lan865x_local.h"
#include "drv_lan865x_regs.h"
#include "driver/lan865x/drv_lan865x.h"
#include "tc6/tc6.h"

/******************************************************************************
*  DEFINES & MACROS
******************************************************************************/
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_MAC_LAN865X
#define LAN865X_MAGIC           (0x72162537ul)
#define LAN865X_CLIENT_MAGIC    (0x23213423ul)
#define RESET_LOW_TIME_MS       (1u)
#define RESET_HIGH_TIME_MS      (7u)
#define PLCA_TIMER_DELAY        (1000u)
#define DELAY_UNLOCK_EXT        (100u)
#define EFUSE_DELAY             (1u)
#define CONTROL_PROTECTION      (true)
#ifdef SYS_CONSOLE_PRINT
#define PRINT(...)              SYS_CONSOLE_PRINT(__VA_ARGS__);
#else
#define PRINT(...)              /* No printing */
#endif

/******************************************************************************
*  VARIABLES
******************************************************************************/

// Data needed by the TCPIP Stack
const TCPIP_MAC_OBJECT DRV_LAN865X_MACObject = {
    .macId                               = TCPIP_MODULE_MAC_LAN865X,
    .macType                             = TCPIP_MAC_TYPE_ETH,
    .macName                             = "LAN865X",
    .TCPIP_MAC_Initialize                = DRV_LAN865X_StackInitialize,
#if (TCPIP_STACK_MAC_DOWN_OPERATION)
    .TCPIP_MAC_Deinitialize              = DRV_LAN865X_Deinitialize,
    .TCPIP_MAC_Reinitialize              = DRV_LAN865X_Reinitialize,
#else
    .TCPIP_MAC_Deinitialize              = 0,
    .TCPIP_MAC_Reinitialize              = 0,
#endif // (TCPIP_STACK_DOWN_OPERATION)
    .TCPIP_MAC_Status                    = DRV_LAN865X_Status,
    .TCPIP_MAC_Tasks                     = DRV_LAN865X_Tasks,
    .TCPIP_MAC_Open                      = DRV_LAN865X_Open,
    .TCPIP_MAC_Close                     = DRV_LAN865X_Close,
    .TCPIP_MAC_LinkCheck                 = DRV_LAN865X_LinkCheck,
    .TCPIP_MAC_RxFilterHashTableEntrySet = DRV_LAN865X_RxFilterHashTableEntrySet,
    .TCPIP_MAC_PowerMode                 = DRV_LAN865X_PowerMode,
    .TCPIP_MAC_PacketTx                  = DRV_LAN865X_PacketTx,
    .TCPIP_MAC_PacketRx                  = DRV_LAN865X_PacketRx,
    .TCPIP_MAC_Process                   = DRV_LAN865X_Process,
    .TCPIP_MAC_StatisticsGet             = DRV_LAN865X_StatisticsGet,
    .TCPIP_MAC_ParametersGet             = DRV_LAN865X_ParametersGet,
    .TCPIP_MAC_RegisterStatisticsGet     = DRV_LAN865X_RegisterStatisticsGet,
    .TCPIP_MAC_ConfigGet                 = DRV_LAN865X_ConfigGet,
    .TCPIP_MAC_EventMaskSet              = DRV_LAN865X_EventMaskSet,
    .TCPIP_MAC_EventAcknowledge          = DRV_LAN865X_EventAcknowledge,
    .TCPIP_MAC_EventPendingGet           = DRV_LAN865X_EventPendingGet,
};

// Local information
static DRV_LAN865X_DriverInfo drvLAN865XDrvInst[DRV_LAN865X_INSTANCES_NUMBER];
static DRV_LAN865X_ClientInfo drvLAN865XClntInst[DRV_LAN865X_CLIENT_INSTANCES_IDX0];

extern const DRV_LAN865X_Configuration drvLan865xInitData[DRV_LAN865X_INSTANCES_NUMBER];

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

static inline void _Lock(OSAL_MUTEX_HANDLE_TYPE *drvMutex);
static inline void _Unlock(OSAL_MUTEX_HANDLE_TYPE *drvMutex);
static DRV_LAN865X_DriverInfo *_Dereference(const void *tag);
static void _Initialize(DRV_LAN865X_DriverInfo *pDrvInstance);
static bool _InitReset(DRV_LAN865X_DriverInfo * pDrvInstance);
static bool _InitMemMap(DRV_LAN865X_DriverInfo * pDrvInst);
static void _OnEFuseOp(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag);
static bool _ReadEFuseReg(DRV_LAN865X_DriverInfo * pDrvInst, uint32_t addr, uint32_t *pVal, uint8_t subState);
static int8_t _GetSignedVal(uint32_t val);
static uint32_t _CalculateValueAndMask(uint8_t start, uint8_t end, uint32_t newValue, uint32_t *mask);
static bool _InitTrim(DRV_LAN865X_DriverInfo * pDrvInst);
static bool _InitUserSettings(DRV_LAN865X_DriverInfo * pDrvInst);
static int32_t _OpenInterface(DRV_LAN865X_DriverInfo * pDrvInstance);
static void _EventHandlerSPI(DRV_SPI_TRANSFER_EVENT event, DRV_SPI_TRANSFER_HANDLE transferHandle, uintptr_t context);
static void _OnSoftResetCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag);
static void _OnInitialRegisterCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag);
static void _OnRegisterDoneCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag);
static void _OnExtendedBlock(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _OnClearStatus1(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _OnStatus1(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _OnClearStatus0(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _OnStatus0(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _OnPlcaStatus(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag);
static void _TxDone(TC6_t *pInst, const uint8_t *pTx, uint16_t len, void *pTag, void *pGlobalTag);
static void _RxPacketAck(TCPIP_MAC_PACKET* pkt, const void* param);

/******************************************************************************
*  Public Function Implementations
******************************************************************************/

// *****************************************************************************
/* LAN865X Initialization

    Summary:
      Initializes the LAN865X Driver Instance, with the configuration data.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function initializes the LAN865X Driver with configuration data
      passed into it by either the system_init function or by the
      DRV_LAN865X_StackInitialize function. Calling this function alone is
      not enough to initialize the driver,  DRV_LAN865X_SetMacCtrlInfo must
      be called with valid data before the driver is ready to be opened.

    Preconditions:
      None.

    Parameters
      index   - This is the index of the driver instance to be initialized.
                The definition DRV_LAN865X_NUM_DRV_INSTANCES controls how many instances
                are available.
      init    - This is a pointer to a DRV_LAN865X_CONFIG structure.

    Returns
      - Valid handle to the driver instance       - If successful
      - SYS_MODULE_OBJ_INVALID                    - If unsuccessful
*/
SYS_MODULE_OBJ DRV_LAN865X_Initialize(SYS_MODULE_INDEX index, SYS_MODULE_INIT * init)
{
    (void)init;
    DRV_LAN865X_DriverInfo *pDrvInst = NULL;
    if (index < DRV_LAN865X_INSTANCES_NUMBER) {
        SYS_ASSERT(index < (sizeof(drvLAN865XDrvInst) / sizeof(DRV_LAN865X_DriverInfo)), "DRV_LAN865X_Initialize (1) index out of bounce");
        SYS_ASSERT(index < (sizeof(drvLan865xInitData) / sizeof(DRV_LAN865X_Configuration)), "DRV_LAN865X_Initialize (2) index out of bounce");
        pDrvInst = &(drvLAN865XDrvInst[index]);
        if (!pDrvInst->inUse) {
            (void)memset(pDrvInst, 0, sizeof(*pDrvInst));
            pDrvInst->magic = LAN865X_MAGIC;
            pDrvInst->index = index;
            pDrvInst->inUse = true;
            OSAL_RESULT res;

            res = OSAL_MUTEX_Create(&(pDrvInst->drvMutex));
            (void)res;
            SYS_ASSERT(res == OSAL_RESULT_TRUE, "Could not create the OSAL Mutex for driver protection");

            (void)memcpy(&(pDrvInst->drvCfg), &drvLan865xInitData[index], sizeof(DRV_LAN865X_Configuration));
            TCPIP_Helper_ProtectedSingleListInitialize(&pDrvInst->rxFreePackets);
            TCPIP_Helper_ProtectedSingleListInitialize(&pDrvInst->rxWaitingForPickupPackets);
        }
    }
    return (NULL != pDrvInst) ?  (SYS_MODULE_OBJ)pDrvInst : SYS_MODULE_OBJ_INVALID;
}

// *****************************************************************************
/* LAN865X Deinitialization
    Summary:
      Deinitializes the LAN865X Driver Instance.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function deallocates any resources allocated by the initialization function.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize.

    Parameters:
      Object    - the valid object returned from DRV_LAN865X_Initialize

    Returns:
      None.
*/
void DRV_LAN865X_Deinitialize(SYS_MODULE_OBJ object)
{
    DRV_LAN865X_DriverInfo *pDrvInst = (DRV_LAN865X_DriverInfo*) object;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    OSAL_MUTEX_Delete(&pDrvInst->drvMutex);
}

// *****************************************************************************
/* LAN865X Reinitialization

    Summary:
      Reinitializes the instance of the LAN865X driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function will deinitialize and initialize the driver instance.  As with
      DRV_LAN865X_Initialize DRV_LAN865X_SetMacCtrlInfo must be called for
      the driver to be useful.
      Note: This function is not planned to be implemented for the first release.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize.

    Parameters:
      - object    - The object valid passed back to DRV_LAN865X_Initialize
      - init    - The new initialization structure.

    Returns:
      None

    */
void DRV_LAN865X_Reinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT * const init)
{
    (void)object;
    (void)init;
}

// *****************************************************************************
/* LAN865X Status

    Summary:
      Gets the current status of the driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function will get the status of the driver instance.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize().

    Parameters:
      object    - The object valid passed back to DRV_LAN865X_Initialize()

    Returns:
      - SYS_STATUS_ERROR            - if an invalid handle has been passed in
      - SYS_STATUS_UNINITIALIZED    - if the driver has not completed initialization
      - SYS_STATUS_BUSY             - if the driver is closing and moving to the closed state
      - SYS_STATUS_READY            - if the driver is ready for client commands
*/
SYS_STATUS DRV_LAN865X_Status(SYS_MODULE_OBJ object)
{
    DRV_LAN865X_DriverInfo *pDrvInst = (DRV_LAN865X_DriverInfo*) object;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    return pDrvInst->state;
}

// *****************************************************************************
/* LAN865X Tasks

    Summary:
      Main task function for the driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function will execute the main state machine for the LAN865X driver.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize.

    Parameters:
      object    - The object valid passed back to DRV_LAN865X_Initialize

    Returns:
      None.
*/
void DRV_LAN865X_Tasks(SYS_MODULE_OBJ object)
{
    DRV_LAN865X_DriverInfo *pDrvInst = (DRV_LAN865X_DriverInfo*) object;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    if (SYS_STATUS_UNINITIALIZED == pDrvInst->state) {
        _Lock(&pDrvInst->drvMutex);
        _Initialize(pDrvInst);
        _Unlock(&pDrvInst->drvMutex);
    }
    if (SYS_STATUS_READY == pDrvInst->state) {
        if (true == pDrvInst->drvCfg.plcaEnable) {
            uint32_t now = SYS_TIME_CounterGet();
            if (SYS_TIME_CountToMS(now - pDrvInst->plcaTimer) >= PLCA_TIMER_DELAY) {
                pDrvInst->plcaTimer = now;
                _Lock(&pDrvInst->drvMutex);
                (void)TC6_ReadRegister(pDrvInst->drvTc6, 0x0004CA03 /* PLCA_STATUS_REGISTER */, CONTROL_PROTECTION, _OnPlcaStatus, NULL);
                 _Unlock(&pDrvInst->drvMutex);
            }
        }
        if (0u != pDrvInst->unlockExtTime) {
            uint32_t now = SYS_TIME_CounterGet();
            if ((now - pDrvInst->unlockExtTime) >= DELAY_UNLOCK_EXT) {
                pDrvInst->unlockExtTime = 0u;
                TC6_UnlockExtendedStatus(pDrvInst->drvTc6);
            }
        }
        if (!SYS_PORT_PinRead(pDrvInst->drvCfg.interruptPin)) {
            _Lock(&pDrvInst->drvMutex);
            (void)TC6_Service(pDrvInst->drvTc6, false);
            _Unlock(&pDrvInst->drvMutex);
        }
    }
    if (pDrvInst->needService) {
        _Lock(&pDrvInst->drvMutex);
        pDrvInst->needService = false;
        (void)TC6_Service(pDrvInst->drvTc6, true);
        _Unlock(&pDrvInst->drvMutex);
    }
}

// *****************************************************************************
/* LAN865X Set MAC Control Information

    Summary:
      This function sets the MAC control information for the driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function is used to pass in the TCPIP_MAC_CONTROL_INIT information that
      is used for allocation and deallocation of memory, event signaling, etc.
      This function is needed to be called so that the driver can enter
      initialization state when the tasks function is called.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize.

    Parameters:
      - object    - The object valid passed back to DRV_LAN865X_Initialize
      - init    - The structure containing the MAC control information

    Returns:
      None.
*/
void DRV_LAN865X_SetMacCtrlInfo(SYS_MODULE_OBJ object, TCPIP_MAC_MODULE_CTRL * init)
{
    DRV_LAN865X_DriverInfo *pDrvInst = (DRV_LAN865X_DriverInfo*) object;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    (void)memcpy(pDrvInst->stackParameters.ifPhyAddress.v, init->ifPhyAddress.v, sizeof(TCPIP_MAC_ADDR));
    (void)memcpy(&pDrvInst->stackCfg, init, sizeof(TCPIP_MAC_MODULE_CTRL));
    pDrvInst->stackParameters.macType = TCPIP_MAC_TYPE_ETH;
    pDrvInst->stackParameters.processFlags = 0;
    pDrvInst->stackParameters.linkMtu = TCPIP_MAC_LINK_MTU_ETH;
    pDrvInst->stackCfgReady = true;

    uint8_t count;
    for (count = 0; count < pDrvInst->drvCfg.rxDescriptors; count++) {
        TCPIP_MAC_PACKET* pkt = (*pDrvInst->stackCfg.pktAllocF)(pDrvInst->drvCfg.rxDescBufferSize, pDrvInst->drvCfg.rxDescBufferSize, TCPIP_MAC_PKT_FLAG_STATIC);
        pkt->ackFunc = _RxPacketAck;
        pkt->ackParam = pDrvInst;
        TCPIP_Helper_ProtectedSingleListTailAdd(&pDrvInst->rxFreePackets, (SGL_LIST_NODE*) pkt);
    }
}

// *****************************************************************************
/* LAN865X Stack Initialization

    Summary:
      This function initializes the driver with a TCPIP_MAC_INIT object.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function is used by the TCP/IP stack to fully initialize the driver with
      both the LAN865X specific configuration and the MAC control information.
      With this function there is no need to call DRV_LAN865X_SetMacCtrlInfo.

    Preconditions:
      None.

    Parameters:
      index    - This is the index of the driver instance to be initialized. The definition
                 DRV_LAN865X_NUM_DRV_INSTANCES controls how many instances are available.
      init     - This is a pointer to a TCPIP_MAC_INIT structure.

    Returns:
      Returns a valid handle to the driver instance - If successful
      SYS_MODULE_OBJ_INVALID                        - If unsuccessful
*/
SYS_MODULE_OBJ DRV_LAN865X_StackInitialize(SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init)
{
    SYS_MODULE_OBJ pDrvInst = SYS_MODULE_OBJ_INVALID;
    const TCPIP_MAC_INIT * const ptr = (const TCPIP_MAC_INIT * const) init;
    if ((index < TCPIP_MODULE_MAC_LAN865X) || (index >= (TCPIP_MODULE_MAC_LAN865X + 16))) {
        SYS_ASSERT(false, "Index is not in range of LAN865X devices");
    } else {
        pDrvInst = DRV_LAN865X_Initialize(index - TCPIP_MODULE_MAC_LAN865X, (SYS_MODULE_INIT*) ptr->moduleData);
        if (SYS_MODULE_OBJ_INVALID != pDrvInst) {
            DRV_LAN865X_SetMacCtrlInfo(pDrvInst, (TCPIP_MAC_MODULE_CTRL*) ptr->macControl);
        }
    }
    return pDrvInst;
}

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Client Level
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* LAN865X Open
    Summary:
      This function is called by the client to open a handle to a driver instance.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      The client will call this function to open a handle to the driver.  When the
      first instance is opened than the driver will send the RX enabled command to
      the ENC hardware.

    Preconditions:
      The driver had to be successfully initialized with DRV_LAN865X_Initialize.

    Parameters:
      index  - This is the index of the driver instance to be initialized.  The
               definition DRV_LAN865X_NUM_DRV_INSTANCES controls how many
               instances are available.
      intent - The intent to use when opening the driver.  Only exclusive is supported

    Returns:
      Returns a valid handle - If successful
      DRV_HANDLE_INVALID     - If unsuccessful
*/
DRV_HANDLE DRV_LAN865X_Open(SYS_MODULE_INDEX index, DRV_IO_INTENT intent)
{
    DRV_HANDLE result = DRV_HANDLE_INVALID;
    DRV_LAN865X_DriverInfo *pDrvInst;
    DRV_LAN865X_ClientInfo* ptr = NULL;
    SYS_ASSERT((index >= TCPIP_MODULE_MAC_LAN865X) && (index < (TCPIP_MODULE_MAC_LAN865X + 16u)), "Index is not in range of LAN865X devices");
    pDrvInst = &(drvLAN865XDrvInst[index - TCPIP_MODULE_MAC_LAN865X]);
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    if (pDrvInst->stackCfgReady) {
        uint8_t x;
        bool success = true;
        _Lock(&pDrvInst->drvMutex);

        if ((intent & DRV_IO_INTENT_EXCLUSIVE) == DRV_IO_INTENT_EXCLUSIVE) {
            if (0u == pDrvInst->numClients) {
                success = false;
            }
        } else {
            if (pDrvInst->exclusiveMode) {
                success = false;
            }
        }

        for (x = 0; success && (DRV_HANDLE_INVALID == result) && (x < DRV_LAN865X_CLIENT_INSTANCES_IDX0); x++) {
            ptr = &drvLAN865XClntInst[x];
            if (LAN865X_CLIENT_MAGIC != ptr->clientMagic) {
                if (-1 == _OpenInterface(pDrvInst)) {
                    SYS_ASSERT(false, "Could not initialize the SPI bus interface.");
                    success = false;
                }
                if (success) {
                    pDrvInst->drvTc6 = TC6_Init(pDrvInst);
                    if (NULL == pDrvInst->drvTc6) {
                        SYS_ASSERT(false, "Could not instance TC6 Lib");
                        success = false;
                    } else {
                        ptr->clientMagic = LAN865X_CLIENT_MAGIC;
                        ptr->pDrvInst = pDrvInst;
                        ptr->pDrvInst->state = SYS_STATUS_UNINITIALIZED;
                        pDrvInst->numClients++;
                        pDrvInst->drvTc6Inst = TC6_GetInstance(pDrvInst->drvTc6);
                        pDrvInst->exclusiveMode = ((intent & DRV_IO_INTENT_EXCLUSIVE) == DRV_IO_INTENT_EXCLUSIVE);
                        result = (DRV_HANDLE)ptr;
                    }
                }
            }
        }

        _Unlock(&pDrvInst->drvMutex);
    }
    return result;
}


// *****************************************************************************
/* LAN865X Close

    Summary:
      Closes a client handle to the driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function closes a handle to the driver.  If it is the last client open,
      the driver will send an RX Disable command to the ENC hardware and move to
      the closed state.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      handle - The successfully opened handle

    Returns:
      None.
*/
void DRV_LAN865X_Close(DRV_HANDLE handle)
{
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) handle;
    DRV_LAN865X_DriverInfo *pDrvInst;

    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    _Lock(&pDrvInst->drvMutex);

    DRV_SPI_Close(pDrvInst->spiClientHandle);
    pDrvInst->spiClientHandle = DRV_HANDLE_INVALID;
    pDrvInst->numClients--;
    pDrvInst->exclusiveMode = false;

    _Unlock(&pDrvInst->drvMutex);
    (void)memset(pClient, 0, sizeof(DRV_LAN865X_ClientInfo));
    pDrvInst->state = SYS_STATUS_UNINITIALIZED;
}

// *****************************************************************************
/* LAN865X Link Check

    Summary:
      This function returns the status of the link.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function checks the status of the link and returns it to the caller.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac: the successfully opened handle

    Returns:
      - true    - if the link is active
      - false   - all other times
*/
bool DRV_LAN865X_LinkCheck(DRV_HANDLE hMac)
{
    /* Link check on LAN865x chip alway returns true, so no need for query this information  */
    (void)hMac;
    return true;
}

// *****************************************************************************
/* LAN865X Receive Filter Hash Table Entry Set

    Summary:
      This function adds an entry to the hash table.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function adds to the MAC's hash table for hash table matching.
      Note: This functionality is not implemented in the first release.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac           - the successfully opened handle
      DestMACAddr    - MAC address to add to the hash table

    Returns:
      - TCPIP_MAC_RES_TYPE_ERR  - if the hMac is invalid
      - TCPIP_MAC_RES_OP_ERR    - if the hMac is valid
 */
TCPIP_MAC_RES DRV_LAN865X_RxFilterHashTableEntrySet(DRV_HANDLE hMac, const TCPIP_MAC_ADDR* DestMACAddr)
{
    (void)hMac;
    (void)DestMACAddr;
    return TCPIP_MAC_RES_OP_ERR;
}

// *****************************************************************************
/* LAN865X Power Mode

    Summary:
      This function sets the power mode of the device.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function sets the power mode of the LAN865X.
      Note: This functionality  is not implemented in the first release.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac        - the successfully opened handle
      pwrMode    - the power mode to set

    Returns:
      - false    - This functionality is not supported in this version of the driver
*/
bool  DRV_LAN865X_PowerMode(DRV_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode)
{
    (void)hMac;
    (void)pwrMode;
    return false;
}

// *****************************************************************************
/* LAN865X Packet Transmit

    Summary:
      This function queues a packet for transmission.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function will take a packet and add it to the queue for transmission.
      When the packet has finished transmitting the driver will call the packets
      acknowledge function.  When that acknowledge function is complete the driver
      will forget about the packet.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters
      hMac        - the successfully opened handle
      ptrPacket    - pointer to the packet

    Returns:
      - TCPIP_MAC_RES_OP_ERR        - if the client handle is invalid
      - TCPIP_MAC_RES_IS_BUSY       - if the driver is not in the run state
      - TCPIP_MAC_RES_QUEUE_TX_FULL - if there are no free descriptors
      - TCPIP_MAC_RES_OK            - on successful queuing of the packet
*/
TCPIP_MAC_RES DRV_LAN865X_PacketTx(DRV_HANDLE hMac, TCPIP_MAC_PACKET * ptrPacket)
{
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    TCPIP_MAC_DATA_SEGMENT* pSeg;
    TCPIP_MAC_RES result = TCPIP_MAC_RES_OP_ERR;
    TC6_RawTxSegment *tc6Seg = NULL;
    uint8_t tc6SegCount;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    SYS_ASSERT(ptrPacket, "Packet pointer invalid");
    _Lock(&pDrvInst->drvMutex);

    tc6SegCount = TC6_GetRawSegments(pDrvInst->drvTc6, &tc6Seg);

    if (0u == tc6SegCount) {
        pDrvInst->txStats.nTxQueueFull++;
        result = TCPIP_MAC_RES_QUEUE_TX_FULL;
    } else {
        uint16_t totalLen = 0;
        uint8_t currSegCount = 0;
        result = TCPIP_MAC_RES_OK;
        pSeg = ptrPacket->pDSeg;
        while((TCPIP_MAC_RES_OK == result) && (NULL != pSeg)) {
            if (currSegCount >= tc6SegCount) {
                pDrvInst->txStats.nTxErrorPackets++;
                SYS_ASSERT(false, "Not enough TC6 Segments, increase TC6_TX_ETH_MAX_SEGMENTS");
                result = TCPIP_MAC_RES_OP_ERR;
            } else {
                tc6Seg[currSegCount].pEth = pSeg->segLoad;
                tc6Seg[currSegCount].segLen = pSeg->segLen;
                totalLen += pSeg->segLen;
                currSegCount++;
                pSeg = pSeg->next;
            }
        }
        if (TCPIP_MAC_RES_OK == result) {
            pDrvInst->txStats.nTxPendBuffers++;
            ptrPacket->pktFlags |= TCPIP_MAC_PKT_FLAG_QUEUED;
            if (!TC6_SendRawEthernetSegments(pDrvInst->drvTc6, tc6Seg, currSegCount, totalLen, 0 /* no timestamp */, _TxDone, ptrPacket)) {
                pDrvInst->txStats.nTxPendBuffers--;
                ptrPacket->pktFlags &= ~TCPIP_MAC_PKT_FLAG_QUEUED;
                pDrvInst->txStats.nTxQueueFull++;
                result = TCPIP_MAC_RES_QUEUE_TX_FULL;
            }
        }
    }
    _Unlock(&pDrvInst->drvMutex);
    return result;
}

// *****************************************************************************
/* LAN865X Receive Packet

    Summary:
      Receive a packet from the driver.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function retrieves a packet from the driver.  The packet needs to be
      acknowledged with the linked acknowledge function so it can be reused.
      Note: pPktStat is ignored in the first release.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac            - the successfully opened handle
      pRes            - the result of the operation
      pPktStat        - address to the receive statistics

    Returns:
      - Pointer to a valid packet   - if successful
      - NULL                        - if unsuccessful
*/
TCPIP_MAC_PACKET* DRV_LAN865X_PacketRx(DRV_HANDLE hMac, TCPIP_MAC_RES* pRes, TCPIP_MAC_PACKET_RX_STAT* pPktStat)
{
    TCPIP_MAC_PACKET *result = NULL;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    if (!TCPIP_Helper_ProtectedSingleListIsEmpty(&pDrvInst->rxWaitingForPickupPackets)) {

        result = (TCPIP_MAC_PACKET*) TCPIP_Helper_ProtectedSingleListHeadRemove(&pDrvInst->rxWaitingForPickupPackets);
    }
    if (NULL != pRes) {
        *pRes = (NULL != result) ? TCPIP_MAC_RES_OK : TCPIP_MAC_RES_NOT_READY_ERR;
    }
    if (NULL != pPktStat) {
        pPktStat->rxStatPIC32INT.rxOk = (NULL != result) ? 1 : 0;
    }
    return result;
}

// *****************************************************************************
/* LAN865X Process

    Summary:
      Additional processing that happens outside the tasks function.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function does additional processing that is not done inside the
      tasks function.
      Note: This function does nothing in the first release.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac    - the successfully opened handle

    Returns:
      - TCPIP_MAC_RES_TYPE_ERR  - if the hMac is invalid
      - TCPIP_MAC_RES_OP_ERR    - if the hMac is valid
*/
TCPIP_MAC_RES DRV_LAN865X_Process(DRV_HANDLE hMac)
{
    (void)hMac;
    return TCPIP_MAC_RES_OP_ERR;
}

// *****************************************************************************
/* LAN865X Get Statistics

    Summary:
    Retrieve the devices statistics.
    <p><b>Implementation:</b> Dynamic</p>

    Description:
    Get the current statistics stored in the driver.
    Note: Statistics are not planned for the first release.

    Preconditions:
    The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac    - the successfully opened handle

    Returns:
      - TCPIP_MAC_RES_TYPE_ERR  - if the hMac is invalid
      - TCPIP_MAC_RES_OP_ERR    - if the hMac is valid
*/
TCPIP_MAC_RES DRV_LAN865X_StatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_RX_STATISTICS* pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics)
{
    TCPIP_MAC_RES result = TCPIP_MAC_RES_OP_ERR;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    if (pRxStatistics && pTxStatistics) {
        SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
        pDrvInst = pClient->pDrvInst;
        SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
        (void)memcpy(pRxStatistics, &pDrvInst->rxStats, sizeof(TCPIP_MAC_RX_STATISTICS));
        (void)memcpy(pTxStatistics, &pDrvInst->txStats, sizeof(TCPIP_MAC_TX_STATISTICS));
        result = TCPIP_MAC_RES_OK;
    }
    return result;
}

// *****************************************************************************
/* LAN865X Get Parameters

    Summary:
      Get the parameters of the device.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      Get the parameters of the device, which includes that it is an Ethernet device
      and what it's MAC address is.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac          - the successfully opened handle
      pMacParams    - pointer to put the parameters

    Returns:
      - TCPIP_MAC_RES_TYPE_ERR  - if the hMac is invalid
      - TCPIP_MAC_RES_OK        - if the hMac is valid
*/
TCPIP_MAC_RES DRV_LAN865X_ParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS* pMacParams)
{
    TCPIP_MAC_RES result = TCPIP_MAC_RES_TYPE_ERR;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    if (NULL != pMacParams) {
        SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
        pDrvInst = pClient->pDrvInst;
        SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
        (void)memcpy(pMacParams, &pDrvInst->stackParameters, sizeof(TCPIP_MAC_PARAMETERS));
        result = TCPIP_MAC_RES_OK;
    }
    return result;
}

// *****************************************************************************
/* LAN865X Get Register Statistics

  Summary:
    Gets the current MAC hardware statistics registers.

  Description:
    This function will get the current value of the statistic registers
    maintained by the MAC hardware.

  Precondition:
   DRV_LAN865X_Initialize should have been called.
   DRV_LAN865X_Open should have been called to obtain a valid handle.

  Parameters:
    hMac           - handle identifying the MAC driver client
    pRegEntries    - pointer to a pRegEntries that will receive the current
                       hardware statistics registers values.
                       Can be 0, if only the number of supported hardware registers is requested
    nEntries       - provides the number of TCPIP_MAC_STATISTICS_REG_ENTRY structures present in pRegEntries
                       Can be 0, if only the number of supported hardware registers is requested
                       The register entries structures will be filled by the driver, up to the
                       supported hardware registers.
    pHwEntries    - pointer to an address to store the number of the statistics registers that the hardware supports
                      It is updated by the driver.
                      Can be 0 if not needed

  Returns:
    - TCPIP_MAC_RES_OK - if all processing went on OK
    - TCPIP_MAC_RES_OP_ERR error code - if function not supported by the driver

  Remarks:
    The reported values are info only and change dynamically.
*/
TCPIP_MAC_RES DRV_LAN865X_RegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries)
{
    int entries = 0;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    if (entries < nEntries) {
        const char regName[] = "PLCA-Enabled";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.plcaEnable;
        entries++;
    }
    if (true == pDrvInst->drvCfg.plcaEnable) {
        if (entries < nEntries) {
            const char regName[] = "PLCA-Status";
            (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
            pRegEntries[entries].registerValue = pDrvInst->plcaStatus;
            entries++;
        }
        if (entries < nEntries) {
            const char regName[] = "PLCA-NodeId";
            (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
            pRegEntries[entries].registerValue = pDrvInst->drvCfg.nodeId;
            entries++;
        }
        if (entries < nEntries) {
            const char regName[] = "PLCA-NodeCount";
            (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
            pRegEntries[entries].registerValue = pDrvInst->drvCfg.nodeCount;
            entries++;
        }
        if (entries < nEntries) {
            const char regName[] = "PLCA-BurstCount";
            (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
            pRegEntries[entries].registerValue = pDrvInst->drvCfg.burstCount;
            entries++;
        }
        if (entries < nEntries) {
            const char regName[] = "PLCA-BurstTimer";
            (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
            pRegEntries[entries].registerValue = pDrvInst->drvCfg.burstTimer;
            entries++;
        }
    }
    if (entries < nEntries) {
        const char regName[] = "TC6-ChunkSize";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.chunkSize;
        entries++;
    }
    if (entries < nEntries) {
        const char regName[] = "TX-CutThrough";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.txCutThrough;
        entries++;
    }
    if (entries < nEntries) {
        const char regName[] = "RX-CutThrough";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.rxCutThrough;
        entries++;
    }
    if (entries < nEntries) {
        const char regName[] = "Promiscuous";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.promiscuous;
        entries++;
    }
    if (entries < nEntries) {
        const char regName[] = "SPI Frequency";
        (void)memcpy(pRegEntries[entries].registerName, regName, sizeof(regName));
        pRegEntries[entries].registerValue = pDrvInst->drvCfg.spiFrequecny;
        entries++;
    }
    *pHwEntries = entries;
    return (0 != entries) ? TCPIP_MAC_RES_OK : TCPIP_MAC_RES_OP_ERR;
}

// *****************************************************************************
/* LAN865X Get Configuration

  Function:
       size_t TCPIP_MAC_ConfigGet(DRV_HANDLE hMac, void* configBuff, size_t buffSize,
       size_t* pConfigSize);

  Summary:
    Gets the current MAC driver configuration.

  Description:
    This function will get the current MAC driver configuration and store it into
    a supplied buffer.

  Precondition:
    DRV_LAN865X_Initialize must have been called to set up the driver.
    DRV_LAN865X_Open should have been called to obtain a valid handle.

  Parameters:
     hMac          - handle identifying the MAC driver client
     configBuff    - pointer to a buffer to store the configuration. Can be NULL if not needed.
     buffSize      - size of the supplied buffer
     pConfigSize   - address to store the number of bytes needed for the storage of the
                     MAC configuration. Can be NULL if not needed.

  Returns:
    The number of bytes copied into the supplied storage buffer

  Remarks:
    None.
*/
size_t DRV_LAN865X_ConfigGet(DRV_HANDLE hMac, void* configBuff, size_t buffSize, size_t* pConfigSize)
{
    size_t result = 0u;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    if (buffSize >= sizeof(DRV_LAN865X_Configuration)) {
        (void)memcpy(configBuff, &pDrvInst->drvCfg, sizeof(DRV_LAN865X_Configuration));
        if (NULL != pConfigSize) {
            *pConfigSize = sizeof(DRV_LAN865X_Configuration);
        }
        result = sizeof(DRV_LAN865X_Configuration);
    }
    return result;
}

// *****************************************************************************
/* LAN865X Set Event Mask

    Summary:
      Sets the event mask.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      Sets the event mask to what is passed in.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac      - the successfully opened handle
      macEvents - the mask to enable or disable
      enable    - to enable or disable events

    Returns
      - true     - if the mask could be set
      - false    - if the mast could not be set
*/
bool DRV_LAN865X_EventMaskSet(DRV_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable)
{
    (void)enable;
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    pDrvInst->eventMask = macEvents;
    return true;
}

// *****************************************************************************
/* LAN865X Acknowledge Event

    Summary:
      Acknowledges an event.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function acknowledges an event.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac      - the successfully opened handle
      macEvents - the events to acknowledge

    Returns:
      - true    - if successful
      - false   - if not successful
*/
bool DRV_LAN865X_EventAcknowledge(DRV_HANDLE hMac, TCPIP_MAC_EVENT macEvents)
{
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    _Lock(&pDrvInst->drvMutex);

    pDrvInst->currentEvents &= ~macEvents;

    if (!TCPIP_Helper_ProtectedSingleListIsEmpty(&pDrvInst->rxWaitingForPickupPackets)) {
        pDrvInst->currentEvents |= pDrvInst->eventMask & TCPIP_MAC_EV_RX_DONE;
    }

    _Unlock(&pDrvInst->drvMutex);
    return true;
}

// *****************************************************************************
/* LAN865X Get Events

    Summary:
      Gets the current events.
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      This function gets the current events.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      hMac - the successfully opened handle

    Returns:
      - TCPIP_MAC_EV_NONE    - Returned on an error
      - List of events       - Returned on event other than an error

*/
TCPIP_MAC_EVENT DRV_LAN865X_EventPendingGet(DRV_HANDLE hMac)
{
    DRV_LAN865X_ClientInfo *pClient = (DRV_LAN865X_ClientInfo*) hMac;
    DRV_LAN865X_DriverInfo *pDrvInst;
    SYS_ASSERT(pClient && (LAN865X_CLIENT_MAGIC == pClient->clientMagic), "Client pointer is invalid");
    pDrvInst = pClient->pDrvInst;
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    return pDrvInst->currentEvents;
}

// *****************************************************************************
/* Reads from the given MAC / Phy registers address

    Summary:
      Performs a read procedure to the MAC / Phy registers
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      Reads from the given register address.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      idx           - the MAC-PHY instance number, starting with 0 for the first instance
      addr          - The 32 Bit register offset
      protected     - true, enables protected control data transmission (normal + inverted data). false, no protection feature is used
      rxCallback    - Pointer to a callback handler. May left NULL, but then the result of the read procedure will be lost.
      pTag          - Any pointer. Will be given back in given modifyCallback. May left NULL

    Returns:
      - TCPIP_MAC_RES_OK                - Returned on success
      - TCPIP_MAC_RES_NO_DRIVER         - Given idx parameter is out of range
      - TCPIP_MAC_RES_PENDING           - MAC-PHY Driver is currently busy, try again later
      - TCPIP_MAC_RES_EVENT_INIT_FAIL   - MAC-PHY initialization is still in progress or failed

*/
TCPIP_MAC_RES DRV_LAN865X_ReadRegister(uint8_t idx, uint32_t addr, bool protected, DRV_LAN865X_RegCallback_t rxCallback, void *pTag)
{
    TCPIP_MAC_RES result = TCPIP_MAC_RES_NO_DRIVER;
    if (idx < DRV_LAN865X_INSTANCES_NUMBER) {
        DRV_LAN865X_DriverInfo *pDrv = &drvLAN865XDrvInst[idx];
        if (SYS_STATUS_READY == pDrv->state) {
            bool success = TC6_ReadRegister(pDrv->drvTc6, addr, protected, (TC6_RegCallback_t)rxCallback, pTag);
            result = (success ? TCPIP_MAC_RES_OK : TCPIP_MAC_RES_PENDING);
        } else {
            result = TCPIP_MAC_RES_EVENT_INIT_FAIL;
        }
    }
    return result;
}

// *****************************************************************************
/* Writes the given value into the MAC / Phy registers

    Summary:
      Performs a write procedure to the MAC / Phy registers
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      Adjusts the given register by writing the given values.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      idx           - the MAC-PHY instance number, starting with 0 for the first instance
      addr          - The 32 Bit register offset
      value         - The 32 Bit register bit value to be written
      protected     - true, enables protected control data transmission (normal + inverted data). false, no protection feature is used
      txCallback    - Pointer to a callback handler. May left NULL
      pTag          - Any pointer. Will be given back in given modifyCallback. May left NULL

    Returns:
      - TCPIP_MAC_RES_OK                - Returned on success
      - TCPIP_MAC_RES_NO_DRIVER         - Given idx parameter is out of range
      - TCPIP_MAC_RES_PENDING           - MAC-PHY Driver is currently busy, try again later
      - TCPIP_MAC_RES_EVENT_INIT_FAIL   - MAC-PHY initialization is still in progress or failed

*/
TCPIP_MAC_RES DRV_LAN865X_WriteRegister(uint8_t idx, uint32_t addr, uint32_t value, bool protected, DRV_LAN865X_RegCallback_t txCallback, void *pTag)
{
    TCPIP_MAC_RES result = TCPIP_MAC_RES_NO_DRIVER;
    if (idx < DRV_LAN865X_INSTANCES_NUMBER) {
        DRV_LAN865X_DriverInfo *pDrv = &drvLAN865XDrvInst[idx];
        if (SYS_STATUS_READY == pDrv->state) {
            bool success = TC6_WriteRegister(pDrv->drvTc6, addr, value, protected, (TC6_RegCallback_t)txCallback, pTag);
            result = (success ? TCPIP_MAC_RES_OK : TCPIP_MAC_RES_PENDING);
        } else {
            result = TCPIP_MAC_RES_EVENT_INIT_FAIL;
        }
    }
    return result;
}

// *****************************************************************************
/* Reads, modifies and writes back the changed value to MAC / Phy registers

    Summary:
      Performs a read, modify, write procedure to the MAC / Phy registers
      <p><b>Implementation:</b> Dynamic</p>

    Description:
      Adjusts the given register by applying values according to the given mask.

    Preconditions:
      The client had to be successfully opened with DRV_LAN865X_Open.

    Parameters:
      idx               - the MAC-PHY instance number, starting with 0 for the first instance
      addr              - The 32 Bit register offset
      value             - The 32 Bit register bit value to be changed. This value will be set to register only if mask on the corresponding position is set to 1
      mask              - The 32 Bit register bit mask. Only Bits set to 1 will be changed accordingly to value
      protected         - true, enables protected control data transmission (normal + inverted data). false, no protection feature is used
      modifyCallback    - Pointer to a callback handler. May left NULL
      pTag              - Any pointer. Will be given back in given modifyCallback. May left NULL

    Returns:
      - TCPIP_MAC_RES_OK                - Returned on success
      - TCPIP_MAC_RES_NO_DRIVER         - Given idx parameter is out of range
      - TCPIP_MAC_RES_PENDING           - MAC-PHY Driver is currently busy, try again later
      - TCPIP_MAC_RES_EVENT_INIT_FAIL   - MAC-PHY initialization is still in progress or failed

*/
TCPIP_MAC_RES DRV_LAN865X_ReadModifyWriteRegister(uint8_t idx, uint32_t addr, uint32_t value, uint32_t mask, bool protected, DRV_LAN865X_RegCallback_t modifyCallback, void *pTag)
{
    TCPIP_MAC_RES result = TCPIP_MAC_RES_NO_DRIVER;
    if (idx < DRV_LAN865X_INSTANCES_NUMBER) {
        DRV_LAN865X_DriverInfo *pDrv = &drvLAN865XDrvInst[idx];
        if (SYS_STATUS_READY == pDrv->state) {
            bool success = TC6_ReadModifyWriteRegister(pDrv->drvTc6, addr, value, mask, protected, (TC6_RegCallback_t)modifyCallback, pTag);
            result = (success ? TCPIP_MAC_RES_OK : TCPIP_MAC_RES_PENDING);
        } else {
            result = TCPIP_MAC_RES_EVENT_INIT_FAIL;
        }
    }
    return result;
}


/******************************************************************************
*  Callbacks from TC6 Software Stack
******************************************************************************/

/**
 * \brief Callback when ever this component needs to be serviced by calling TC6_Service()
 * \note This function must be implemented by the integrator.
 * \note Do not call TC6_Service() within this callback! Set a flag or raise an event to do it in the cyclic task context.
 * \warning !! THIS FUNCTION MAY GET CALLED FROM TASK AND INTERRUPT CONTEXT !!
 * \param pInst - The pointer returned by TC6_Init.
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 */
void TC6_CB_OnNeedService(TC6_t *pInst, void *pGlobalTag)
{
    (void)pInst;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        pDrvInst->needService = true;
    }
}

/**
 * \brief Callback when ever a slice of an Ethernet packet was received.
 * \note This function must be implemented by the integrator.
 * \note In most cases this function will not give an entire Ethernet frame. It will be only a piece of it. The integrator needs to combine it.
 * \param pInst - The pointer returned by TC6_Init.
 * \param pRx - Filled byte array holding the received Ethernet packet
 * \param offset - 0, if this is the start of a new Ethernet frame. Otherwise the offset to which this slide of payload belongs to.
 * \param len - Length of the byte array.
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 */
void TC6_CB_OnRxEthernetSlice(TC6_t *pInst, const uint8_t *pRx, uint16_t offset, uint16_t len, void *pGlobalTag)
{
    (void)pInst;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        if (!pDrvInst->rxPacketInvalid) {
            if (NULL == pDrvInst->rxDescriptors.macPkt) {
                if (true == TCPIP_Helper_ProtectedSingleListIsEmpty(&pDrvInst->rxFreePackets)) {
                    pDrvInst->rxPacketInvalid = true;
                    pDrvInst->rxStats.nRxBuffNotAvailable++;
                } else {
                    pDrvInst->rxDescriptors.macPkt = (TCPIP_MAC_PACKET*) TCPIP_Helper_ProtectedSingleListHeadRemove(&pDrvInst->rxFreePackets);
                    pDrvInst->rxDescriptors.macPkt->pDSeg->segLen = 0;
                }
            }
            if (NULL != pDrvInst->rxDescriptors.macPkt) {
                (void)memcpy(pDrvInst->rxDescriptors.macPkt->pDSeg->segLoad + offset, pRx, len);
                if (0u != offset) {
                    pDrvInst->rxDescriptors.macPkt->pDSeg->segLen += len;
                } else {
                    pDrvInst->rxDescriptors.macPkt->pDSeg->segLen = len;
                }
            }
        }
    }
}

/**
 * \brief Callback when ever an Ethernet packet was received. This will notify the integrator, that now all chunks very reported by TC6_CB_OnRxEthernetPacket and the data can be processed.
 * \note This function must be implemented by the integrator.
 * \param pInst - The pointer returned by TC6_Init.
 * \param success - true, if the received Ethernet frame was received without errors. false, if there were errors.
 * \param len - Length of the entire Ethernet frame. This is all length reported TC6_CB_OnRxEthernetPacket combined.
 * \param rxTimestamp - Pointer to the receive timestamp, if there was any. NULL, otherwise. Pointer will be invalid after returning out of the callback!
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 */
void TC6_CB_OnRxEthernetPacket(TC6_t *pInst, bool success, uint16_t len, uint64_t *rxTimestamp, void *pGlobalTag)
{
    (void)pInst;
    (void)rxTimestamp;
    TCPIP_MAC_PACKET *macPkt = NULL;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        if (pDrvInst->rxPacketInvalid || !success) {
            pDrvInst->rxPacketInvalid = false;
            pDrvInst->rxStats.nRxErrorPackets++;
        } else {
            macPkt = pDrvInst->rxDescriptors.macPkt;
            SYS_ASSERT(len == macPkt->pDSeg->segLen, "Invalid RX length");
            pDrvInst->rxDescriptors.macPkt = NULL;
            pDrvInst->rxStats.nRxPendBuffers++;
            macPkt->pMacLayer = macPkt->pDSeg->segLoad;
            macPkt->pNetLayer = macPkt->pMacLayer + sizeof(TCPIP_MAC_ETHERNET_HEADER);
            if (0xFF == macPkt->pDSeg->segLoad[0]) {
                macPkt->pktFlags = TCPIP_MAC_PKT_FLAG_BCAST;
            } else {
                macPkt->pktFlags|= TCPIP_MAC_PKT_FLAG_UNICAST;
            }
            TCPIP_Helper_ProtectedSingleListTailAdd(&pDrvInst->rxWaitingForPickupPackets, (SGL_LIST_NODE*) macPkt);

            pDrvInst->currentEvents |= TCPIP_MAC_EV_RX_DONE;
            (*pDrvInst->stackCfg.eventF)(pDrvInst->currentEvents, pDrvInst->stackCfg.eventParam);
        }
    }
}

/**
 * \brief Callback when ever an error occurred.
 * \note This function must be implemented by the integrator.
 * \param pInst - The pointer returned by TC6_Init.
 * \param err - Enumeration value holding the actual error condition.
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 */
void TC6_CB_OnError(TC6_t *pInst, TC6_Error_t err, void *pGlobalTag)
{
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    bool reinit = false;
    (void)pInst;
    if (NULL != pDrvInst) {
        uint32_t val = err;
        switch(val) {
            case TC6Error_Succeeded:
                PRINT("TC6Error_Succeeded\r\n");
                break;
            case TC6Error_NoHardware:
                PRINT("TC6Error_NoHardware\r\n");
                reinit = true;
            break;
            case TC6Error_UnexpectedSv:
                PRINT("TC6Error_UnexpectedSv\r\n");
                break;
            case TC6Error_UnexpectedDvEv:
                PRINT("TC6Error_UnexpectedDvEv\r\n");
                break;
            case TC6Error_BadChecksum:
                PRINT("TC6Error_BadChecksum\r\n");
                reinit = true;
                break;
            case TC6Error_UnexpectedCtrl:
                PRINT("TC6Error_UnexpectedCtrl\r\n");
                reinit = true;
                break;
            case TC6Error_BadTxData:
                PRINT("TC6Error_BadTxData\r\n");
                reinit = true;
                break;
            case TC6Error_SyncLost:
                PRINT("TC6Error_SyncLost\r\n");
                reinit = true;
                break;
            case TC6Error_SpiError:
                PRINT("TC6Error_SpiError\r\n");
                reinit = true;
                break;
            default:
                PRINT("TC6_Error unknown_flag=%d\r\n", val);
                break;
        }
        if (reinit) {
            pDrvInst->initState = DRV_LAN865X_INITSTATE_RESET;
            pDrvInst->state = SYS_STATUS_UNINITIALIZED;
        }
    }
}

/**
 * \brief Callback when ever TC6 packet was received, which had the extended flag set. This means, that the user should read (and clear) at least Status0 and Status1 registers.
 * \note The integrator MUST call TC6_UnlockExtendedStatus() whenever he is ready to process the next extended status flag. Reenabling it to early may trigger thousands of events per second.
 * \note This function must be implemented by the integrator.
 * \param pInst - The pointer returned by TC6_Init.
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 */
void TC6_CB_OnExtendedStatus(TC6_t *pInst, void *pGlobalTag)
{
    (void)pGlobalTag;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    pDrvInst->unlockExtTime = SYS_TIME_CounterGet();
    if(!TC6_ReadRegister(pInst, 0x00000008u /* STATUS0 */, CONTROL_PROTECTION, _OnStatus0, NULL)) {
        (void)TC6_Service(pInst, true);
    }
}

/**
 * \brief Users implementation of SPI transfer function.
 * \note The implementation may be synchronous or asynchronous. But in any case the TC6Stub_CB_OnSpiBufferDone() must be called, when the SPI transaction is over!
 * \param tc6instance - The instance number of the hardware. Starting with 0 for the first.
 * \param pTx - Pointer to the MOSI data. The pointer stays valid until user calls TC6Stub_CB_OnSpiBufferDone()
 * \param pRx - Pointer to the MISO Buffer. The pointer stays valid until user calls TC6Stub_CB_OnSpiBufferDone()
 * \param len - The length of both buffers (pTx and pRx). The entire length must be transfered via SPI.
 * \param pGlobalTag - The exact same pointer, which was given along with the TC6_Init() function.
 * \return true, if the SPI data was enqueued/transfered. false, there was an error.
 */
bool TC6_CB_OnSpiTransaction(uint8_t tc6instance, uint8_t *pTx, uint8_t *pRx, uint16_t len, void *pGlobalTag)
{
    (void)tc6instance;
    bool spiStarted = false;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        if (!pDrvInst->spiBusy) {
            pDrvInst->spiBusy = true;
            DRV_SPI_WriteReadTransferAdd(pDrvInst->spiClientHandle,(void*)pTx, len, (void*)pRx, len, &pDrvInst->spiTransferHandle);
            if (DRV_SPI_TRANSFER_HANDLE_INVALID == pDrvInst->spiTransferHandle) {
                pDrvInst->spiBusy = false;
            } else {
                spiStarted = true;
            }
        }
    }
    return spiStarted;
}

/******************************************************************************
*  Local Function Implementations
******************************************************************************/

static inline void _Lock(OSAL_MUTEX_HANDLE_TYPE *drvMutex)
{
    OSAL_RESULT res = OSAL_MUTEX_Lock(drvMutex, OSAL_WAIT_FOREVER);
    (void)res;
    SYS_ASSERT(res == OSAL_RESULT_TRUE, "Could not lock the driver mutex");
}

static inline void _Unlock(OSAL_MUTEX_HANDLE_TYPE *drvMutex)
{
    OSAL_RESULT res = OSAL_MUTEX_Unlock(drvMutex);
    (void)res;
    SYS_ASSERT(res == OSAL_RESULT_TRUE, "Could not unlock the driver mutex");

}

/* This function was invented to overcome MISRA rule violation 11.4 and 11.5, where it is not allowed to cast back from void pointer */
static DRV_LAN865X_DriverInfo *_Dereference(const void *tag)
{
    uint8_t i;
    DRV_LAN865X_DriverInfo *pDrvInst = NULL;
    for (i = 0; (NULL == pDrvInst) && (i < DRV_LAN865X_INSTANCES_NUMBER); i++) {
        if (tag == &drvLAN865XDrvInst[i]) {
            pDrvInst = &drvLAN865XDrvInst[i];
        }
    }
    SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
    return pDrvInst;
}

static void _Initialize(DRV_LAN865X_DriverInfo *pDrvInst)
{
    bool done = false;
    switch(pDrvInst->initState) {
        case DRV_LAN865X_INITSTATE_RESET:
            done = _InitReset(pDrvInst);
            break;
        case DRV_LAN865X_INITSTATE_MEMMAP:
            done = _InitMemMap(pDrvInst);
            break;
        case DRV_LAN865X_INITSTATE_TRIM:
            done = _InitTrim(pDrvInst);
            break;
        case DRV_LAN865X_INITSTATE_SETREGS:
            done = _InitUserSettings(pDrvInst);
            break;
        case DRV_LAN865X_INITSTATE_FINISHED:
        default:
            SYS_ASSERT(false, "Wrong Initialize state");
            break;
    }
    if (done) {
        pDrvInst->initSubState = 0;
        pDrvInst->initState++;
        if (DRV_LAN865X_INITSTATE_FINISHED == pDrvInst->initState) {
            pDrvInst->state = SYS_STATUS_READY;
        }
    }
}

static bool _InitReset(DRV_LAN865X_DriverInfo * pDrvInst)
{
    bool done = false;
    if (SYS_PORT_PIN_NONE != pDrvInst->drvCfg.resetPin) {
        /* Hardware Reset */
        switch(pDrvInst->initSubState) {
            case 0:
                SYS_PORT_PinWrite(pDrvInst->drvCfg.resetPin, false);
                pDrvInst->initTimer = SYS_TIME_CounterGet();
                pDrvInst->initSubState++;
                break;
            case 1:
                if (SYS_TIME_CountToMS(SYS_TIME_CounterGet() - pDrvInst->initTimer) >= RESET_LOW_TIME_MS) {
                    SYS_PORT_PinWrite(pDrvInst->drvCfg.resetPin, true);
                    pDrvInst->initTimer = SYS_TIME_CounterGet();
                    pDrvInst->initSubState++;
                }
                break;
            case 2:
                if (SYS_TIME_CountToMS(SYS_TIME_CounterGet() - pDrvInst->initTimer) >= RESET_HIGH_TIME_MS) {
                    done = true;
                }
                break;
            default:
                SYS_ASSERT(false, "Wrong Hard Reset sub state");
                done = true;
                break;
        }
    } else {
        /* Software Reset */
        switch(pDrvInst->initSubState) {
            case 0:
                pDrvInst->initTimer = SYS_TIME_CounterGet();
                pDrvInst->initSubState++;
                break;
            case 1:
                if (SYS_TIME_CountToMS(SYS_TIME_CounterGet() - pDrvInst->initTimer) >= RESET_LOW_TIME_MS) {
                    pDrvInst->initSubState++;
                }
                break;
            case 2:
                if (TC6_WriteRegister(pDrvInst->drvTc6, 0x00000003u /* RESET */, 0x1u, false, _OnSoftResetCB, NULL)) {
                    pDrvInst->initSubState++;
                }
                break;
            case 3:
                if (TC6_WriteRegister(pDrvInst->drvTc6, 0x00000003u /* RESET */, 0x1u, true, _OnSoftResetCB, NULL)) {
                    pDrvInst->initTimer = SYS_TIME_CounterGet();
                    pDrvInst->initSubState++;
                }
                break;
            case 4:
                if (SYS_TIME_CountToMS(SYS_TIME_CounterGet() - pDrvInst->initTimer) >= RESET_HIGH_TIME_MS) {
                    done = true;
                }
                break;
            default:
                SYS_ASSERT(false, "Wrong Soft Reset sub state");
                done = true;
                break;
        }
    }
    return done;
}

static bool _InitMemMap(DRV_LAN865X_DriverInfo * pDrvInst)
{
    SYS_ASSERT(TC6_MEMMAP_LENGTH == MEMMAP_LEN, "MEMMAP size mismatch");
    pDrvInst->initSubState += TC6_MultipleRegisterAccess(pDrvInst->drvTc6, &TC6_MEMMAP[pDrvInst->initSubState], (TC6_MEMMAP_LENGTH - pDrvInst->initSubState), _OnInitialRegisterCB, NULL);
    return (TC6_MEMMAP_LENGTH == pDrvInst->initSubState);
}

static void _OnEFuseOp(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag)
{
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    (void)pInst;
    (void)addr;
    (void)pTag;
    if (NULL != pDrvInst) {
        if (success) {
            pDrvInst->initReadVal = value;
            pDrvInst->initSubState++;
        } else {
            pDrvInst->initSubState--;
        }
    }
}

static bool _ReadEFuseReg(DRV_LAN865X_DriverInfo * pDrvInst, uint32_t addr, uint32_t *pVal, uint8_t subState)
{
    uint32_t regVal;
    TC6_t *tc = pDrvInst->drvTc6;
    bool finished = false;
    switch (subState) {
        case 0:
            regVal = (addr & 0x000Fu);
            if (TC6_WriteRegister(tc, 0x000400D8, regVal, CONTROL_PROTECTION, _OnEFuseOp, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 1:
            /* Wait for _OnEFuseOp callback */
            break;
        case 2:
            if (TC6_WriteRegister(tc, 0x000400DA, 0x0002, CONTROL_PROTECTION, _OnEFuseOp, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 3:
            /* Wait for _OnEFuseOp callback */
            break;
        case 4:
            pDrvInst->initTimer = SYS_TIME_CounterGet();
            pDrvInst->initSubState++;
            break;
        case 5:
            if (SYS_TIME_CountToMS(SYS_TIME_CounterGet() - pDrvInst->plcaTimer) >= EFUSE_DELAY) {
                pDrvInst->initSubState++;
            }
            break;
        case 6:
            if (TC6_ReadRegister(tc, 0x000400D9, CONTROL_PROTECTION, _OnEFuseOp, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 7:
            /* Wait for _OnEFuseOp callback */
            break;
        case 8:
            if (NULL != pVal) {
                *pVal = pDrvInst->initReadVal;
                finished = true;
            }
            break;
        default:
            SYS_ASSERT(false, "Wrong EFuse sub state");
            break;
    }
    return finished;
}

static int8_t _GetSignedVal(uint32_t val)
{
    int8_t result;
    if ((val & (1u << 4u)) ==  (1u << 4u)) {
        /* negative value */
        result = val | 0xE0u;
        if(result < -5) {
            result = -5;
        }
    } else {
        /* positive value */
        result = val;
    }
    return result;
}

static uint32_t _CalculateValueAndMask(uint8_t start, uint8_t end, uint32_t newValue, uint32_t *mask)
{
    if (NULL != mask) {
        uint8_t i;
        for (i = start; i <= end; i++) {
            *mask |= (1u << i);
        }
    }
    return (newValue << start);
}

static bool _InitTrim(DRV_LAN865X_DriverInfo * pDrvInst)
{
    TC6_t *tc = pDrvInst->drvTc6;
    uint32_t val;
    uint32_t mask;
    bool done = false;
    bool success;
    switch(pDrvInst->initSubState) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (_ReadEFuseReg(pDrvInst, 0x5, &val, pDrvInst->initSubState)) {
                bool isTrimmed = (0u != (val & 0x40u));
                if (isTrimmed) {
                    pDrvInst->initSubState = 10;
                } else {
                    /* Abort trimming */
                    PRINT("Warning:PHY is not trimmed!\r\n");
                    done = true;
                }
            }
            break;

        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
            if (_ReadEFuseReg(pDrvInst, 0x4, &val, pDrvInst->initSubState - 10u)) {
                pDrvInst->initEfuseA4 = _GetSignedVal(val);
                pDrvInst->initSubState = 20;
            }
            break;

        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
            if (_ReadEFuseReg(pDrvInst, 0x8, &val, pDrvInst->initSubState - 20u)) {
                pDrvInst->initEfuseA8 = _GetSignedVal(val);
                pDrvInst->initSubState = 30;
            }
            break;

        case 30:
            mask = 0u;
            val = _CalculateValueAndMask(10, 15, (0x9 + pDrvInst->initEfuseA4), &mask);
            val |= _CalculateValueAndMask(4, 9, (0xE + pDrvInst->initEfuseA4), &mask);
            pDrvInst->initSubState++; /* Avoid race condition error */
            success = TC6_ReadModifyWriteRegister(tc, 0x00040084, val, mask, CONTROL_PROTECTION, _OnEFuseOp, NULL);
            if (!success) {
                pDrvInst->initSubState--;
            }
            break;
        case 31:
            /* Wait for _OnEFuseOp callback */
            break;

        case 32:
            mask = 0u;
            val = _CalculateValueAndMask(10, 15, (0x28 + pDrvInst->initEfuseA8), &mask);
            pDrvInst->initSubState++; /* Avoid race condition error */
            success = TC6_ReadModifyWriteRegister(tc, 0x0004008A, val, mask, CONTROL_PROTECTION, _OnEFuseOp, NULL);
            if (!success) {
                pDrvInst->initSubState--;
            }
            break;
        case 33:
            /* Wait for _OnEFuseOp callback */
            break;

        case 34:
            mask = 0u;
            val = _CalculateValueAndMask(8, 13, (0x5 + pDrvInst->initEfuseA4), &mask);
            val |= _CalculateValueAndMask(0, 5, (0x9 + pDrvInst->initEfuseA4), &mask);
            pDrvInst->initSubState++; /* Avoid race condition error */
            success = TC6_ReadModifyWriteRegister(tc, 0x000400AD, val, mask, CONTROL_PROTECTION, _OnEFuseOp, NULL);
            if (!success) {
                pDrvInst->initSubState--;
            }
            break;
        case 35:
            /* Wait for _OnEFuseOp callback */
            break;

        case 36:
            mask = 0u;
            val = _CalculateValueAndMask(8, 13, (0x9 + pDrvInst->initEfuseA4), &mask);
            val |= _CalculateValueAndMask(0, 5, (0xE + pDrvInst->initEfuseA4), &mask);
            pDrvInst->initSubState++; /* Avoid race condition error */
            success = TC6_ReadModifyWriteRegister(tc, 0x000400AE, val, mask, CONTROL_PROTECTION, _OnEFuseOp, NULL);
            if (!success) {
                pDrvInst->initSubState--;
            }
            break;
        case 37:
            /* Wait for _OnEFuseOp callback */
            break;

        case 38:
            mask = 0u;
            val = _CalculateValueAndMask(8, 13, (0x11 + pDrvInst->initEfuseA4), &mask);
            val |= _CalculateValueAndMask(0, 5, (0x16 + pDrvInst->initEfuseA4), &mask);
            pDrvInst->initSubState++; /* Avoid race condition error */
            success = TC6_ReadModifyWriteRegister(tc, 0x000400AF, val, mask, CONTROL_PROTECTION, _OnEFuseOp, NULL);
            if (!success) {
                pDrvInst->initSubState--;
            }
            break;
        case 39:
            /* Wait for _OnEFuseOp callback */
            break;

        case 40:
            done = true;
            break;
        default:
            SYS_ASSERT(false, "Wrong trim sub state");
            break;
    }
    return done;
}

static bool _InitUserSettings(DRV_LAN865X_DriverInfo * pDrvInst)
{
    bool done = false;
    uint8_t *mac = pDrvInst->stackParameters.ifPhyAddress.v;
    TC6_t *tc = pDrvInst->drvTc6;
    uint32_t regVal;
    switch(pDrvInst->initSubState) {
        case 0:
            if (true == pDrvInst->drvCfg.plcaEnable) {
                /* PLCA Phy Node Id and Max Node Count */
                regVal = (pDrvInst->drvCfg.nodeCount << 8) | pDrvInst->drvCfg.nodeId;
                if (TC6_WriteRegister(tc, 0x0004CA02u /* PLCA_CONTROL_1_REGISTER */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                    pDrvInst->initSubState++;
                }
            } else {
                /* PLCA disabled */
                pDrvInst->initSubState = 3u;
            }
            break;
        case 1:
            /* PLCA Burst Count and Burst Timer */
            regVal = (pDrvInst->drvCfg.burstCount << 8) | pDrvInst->drvCfg.burstTimer;
            if (TC6_WriteRegister(tc, 0x0004CA05u /* PLCA_BURST_MODE_REGISTER */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 2:
            /* Enable PLCA */
            regVal = (1u << 15);
            if (TC6_WriteRegister(tc, 0x0004CA01u /* PLCA_CONTROL_0_REGISTER */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 3:
            /* MAC address setting (LOW) */
            regVal = (mac[3] << 24) | (mac[2] << 16) | (mac[1] << 8) | mac[0];
            if (TC6_WriteRegister(tc, 0x00010024u /* SPEC_ADD2_BOTTOM */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 4:
            /* MAC address setting (HIGH) */
            regVal = (mac[5] << 8) | mac[4];
            if (TC6_WriteRegister(tc, 0x00010025u /* SPEC_ADD2_TOP */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 5:
            /* MAC address setting, setting unique lower MAC address, back off time is generated out of that */
            regVal = (mac[5] << 24) | (mac[4] << 16) | (mac[3] << 8) | mac[2];
            if (TC6_WriteRegister(tc, 0x00010022u /* SPEC_ADD1_BOTTOM */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 6:
            /* Promiscuous mode */
            regVal = (pDrvInst->drvCfg.promiscuous ? 0x10u : 0x0u);
            if (TC6_WriteRegister(tc, 0x00010001u /* NETWORK_CONFIG */, regVal, CONTROL_PROTECTION, _OnInitialRegisterCB, NULL)) {
                pDrvInst->initSubState++;
            }
            break;
        case 7:
            /* Cut Through / Store and Forward mode */
            regVal = 0x9026u;
            if (true == pDrvInst->drvCfg.txCutThrough) {
                regVal |= 0x200u;
            }
            if (true == pDrvInst->drvCfg.rxCutThrough) {
                regVal |= 0x100u;
            }
            if (TC6_WriteRegister(tc, 0x00000004u /* CONFIG0 */, regVal, CONTROL_PROTECTION, _OnRegisterDoneCB, NULL)) {
                done = true;
            }
            break;
        default:
            SYS_ASSERT(false, "Wrong User Settings sub state");
            done = true;
            break;
    }
    return done;
}

static int32_t _OpenInterface(DRV_LAN865X_DriverInfo * pDrvInstance)
{
    bool success = false;
    pDrvInstance->spiClientHandle = DRV_SPI_Open(pDrvInstance->drvCfg.spiDrvIndex, DRV_IO_INTENT_READWRITE);
    if (DRV_HANDLE_INVALID != pDrvInstance->spiClientHandle) {
        DRV_SPI_TransferEventHandlerSet(pDrvInstance->spiClientHandle, _EventHandlerSPI, pDrvInstance->index);

        DRV_SPI_TRANSFER_SETUP spiClientInfo = {
            .baudRateInHz = pDrvInstance->drvCfg.spiFrequecny,
            .clockPhase = DRV_SPI_CLOCK_PHASE_VALID_LEADING_EDGE,
            .clockPolarity = DRV_SPI_CLOCK_POLARITY_IDLE_LOW,
            .dataBits = DRV_SPI_DATA_BITS_8,
            .csPolarity = DRV_SPI_CS_POLARITY_ACTIVE_LOW,
            .chipSelect = pDrvInstance->drvCfg.spiChipSelectPin
        };
        DRV_SPI_TransferSetup(pDrvInstance->spiClientHandle, &spiClientInfo);
        success = true;
    }
    return success ? 0 : -1;
}

static void _EventHandlerSPI(DRV_SPI_TRANSFER_EVENT event, DRV_SPI_TRANSFER_HANDLE transferHandle, uintptr_t context)
{
    (void)transferHandle;
    if (context < DRV_LAN865X_INSTANCES_NUMBER) {
        DRV_LAN865X_DriverInfo *pDrvInst = &drvLAN865XDrvInst[context]; /* Overcome MISRA Issue 11.4 */
        SYS_ASSERT(pDrvInst && (LAN865X_MAGIC == pDrvInst->magic), "Driver info pointer is invalid");
        if (DRV_SPI_TRANSFER_EVENT_ERROR == event) {
            pDrvInst->txStats.nTxErrorPackets++;
            pDrvInst->rxStats.nRxErrorPackets++;
            SYS_ASSERT(false, "SPI transfer failed");
        }
        if (DRV_SPI_TRANSFER_EVENT_PENDING != event) {
            SYS_ASSERT(pDrvInst->spiBusy, "IRQ without request");
            pDrvInst->spiBusy = false;
            TC6_SpiBufferDone(pDrvInst->drvTc6Inst, (DRV_SPI_TRANSFER_EVENT_COMPLETE == event));
        }
    }
}

static void _OnSoftResetCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag)
{
    (void)pInst;
    (void)success;
    (void)addr;
    (void)value;
    (void)pTag;
    (void)pGlobalTag;
    /* Silently ignore anything */
}

static void _OnInitialRegisterCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag)
{
    (void)pInst;
    (void)pTag;
    (void)addr;
    (void)value;
    if (!success) {
        /* TODO: add proper error handling here */
        DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
        if (NULL != pDrvInst) {
            PRINT("Setting failed, addr=0x%X\r\n", addr);
        }
    }
}

static void _OnRegisterDoneCB(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *pTag, void *pGlobalTag)
{
    (void)pTag;
    (void)addr;
    (void)value;
    if (success) {
        TC6_EnableData(pInst, true);
    } else {
        DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
        if (NULL != pDrvInst) {
            PRINT("Final setting failed, addr=0x%X\r\n", addr);
        }
    }
}

static void _OnExtendedBlock(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    (void)pInst;
    (void)addr;
    (void)tag;
    (void)pGlobalTag;
    if (success) {
        uint8_t i;
        for (i = 0u; i < 32u; i++) {
            if (0u != (value & (1u << i))) {
                switch (i) {
                    case 0:
                        PRINT("ExtBlock.SPI_Err_Int\r\n");
                        break;
                    case 1:
                        PRINT("ExtBlock.MAC_BMGR_Int\r\n");
                        break;
                    case 2:
                        PRINT("ExtBlock.MAC_Int\r\n");
                        break;
                    case 3:
                        PRINT("ExtBlock.HMX_Int\r\n");
                        break;
                    case 31:
                        PRINT("ExtBlock.GINT_Mask\r\n");
                        break;
                    default:
                        PRINT("ExtBlock.UnknownError\r\n");
                        break;
                }
            }
        }
    } else {
        PRINT("ExtBlock.UnknownError\r\n");
    }
}

static void _OnClearStatus1(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    (void)success;
    (void)addr;
    (void)value;
    (void)tag;
    (void)pGlobalTag;
    if (pDrvInst->extBlock) {
        pDrvInst->extBlock = false;
        while (!TC6_ReadRegister(pInst, 0x000A0087u /* EXT_BLK_STATUS */, CONTROL_PROTECTION, _OnExtendedBlock, NULL)) {
            (void)TC6_Service(pInst, true);
        }
    }
}

static void _OnStatus1(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    (void)addr;
    (void)tag;
    (void)pGlobalTag;
    pDrvInst->extBlock = false;
    if (success) {
        uint8_t i;
        bool reinit = false;
        for (i = 0u; i < 32u; i++) {
            if (0u != (value & (1u << i))) {
                switch (i) {
                    case 0:
                        PRINT("Status1.RX_Non_Recoverable_Error\r\n");
                        reinit = true;
                        break;
                    case 1:
                        PRINT("Status1.TX_Non_Recoverable_Error\r\n");
                        reinit = true;
                        break;
                    case 17:
                        PRINT("Status1.FSM_State_Error\r\n");
                        break;
                    case 18:
                        PRINT("Status1.SRAM_ECC_Error\r\n");
                        break;
                    case 19:
                        PRINT("Status1.Undervoltage\r\n");
                        break;
                    case 20:
                        PRINT("Status1.Internal_Bus_Error\r\n");
                        break;
                    case 21:
                        PRINT("Status1.TX_Timestamp_Capture_Overflow_A\r\n");
                        break;
                    case 22:
                        PRINT("Status1.TX_Timestamp_Capture_Overflow_B\r\n");
                        break;
                    case 23:
                        PRINT("Status1.TX_Timestamp_Capture_Overflow_C\r\n");
                        break;
                    case 24:
                        PRINT("Status1.TX_Timestamp_Capture_Missed_A\r\n");
                        break;
                    case 25:
                        PRINT("Status1.TX_Timestamp_Capture_Missed_B\r\n");
                        break;
                    case 26:
                        PRINT("Status1.TX_Timestamp_Capture_Missed_C\r\n");
                        break;
                    case 27:
                        PRINT("Status1.MCLK_GEN_Status\r\n");
                        break;
                    case 28:
                        PRINT("Status1.gPTP_PA_TS_EG_Status\r\n");
                        break;
                    case 29:
                        PRINT("Status1.Extended_Block_Status\r\n");
                         pDrvInst->extBlock = true;
                        break;
                    default:
                        PRINT("Status1.UnknownError\r\n");
                        break;
                }
            }
        }
        if (0u != value) {
            /* Write to clear pending flags */
            while (!TC6_WriteRegister(pInst, addr, value, CONTROL_PROTECTION, _OnClearStatus1, NULL)) {
                (void)TC6_Service(pInst, true);
            }
        }
        if (reinit) {
            pDrvInst->initState = DRV_LAN865X_INITSTATE_RESET;
            pDrvInst->state = SYS_STATUS_UNINITIALIZED;
        }
    } else {
        PRINT("Status1.UnknownError\r\n");
    }
}

static void _OnClearStatus0(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    (void)success;
    (void)addr;
    (void)value;
    (void)tag;
    (void)pGlobalTag;
    while (!TC6_ReadRegister(pInst, 0x00000009u /* STATUS1 */, CONTROL_PROTECTION, _OnStatus1, NULL)) {
        (void)TC6_Service(pInst, true);
    }
}

static void _OnStatus0(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    (void)addr;
    (void)tag;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        if (success) {
            bool reinit = false;
            uint8_t i;
            while (!TC6_WriteRegister(pInst, addr, value, CONTROL_PROTECTION, _OnClearStatus0, NULL)) {
                (void)TC6_Service(pInst, true);
            }
            for (i = 0; i < 32u; i++) {
                if (0u != (value & (1u << i))) {
                    switch (i) {
                        case 0:
                            PRINT("Status0.Transmit Protocol Error\r\n");
                            break;
                        case 1:
                            PRINT("Status0.Transmit Buffer Overflow Error\r\n");
                            break;
                        case 2:
                            PRINT("Status0.Transmit Buffer Underflow Error\r\n");
                            break;
                        case 3:
                            PRINT("Status0.Receive Buffer Overflow Error\r\n");
                            break;
                        case 4:
                            PRINT("Status0.Loss of Framing Error\r\n");
                            reinit = true;
                            break;
                        case 5:
                            PRINT("Status0.Header Error\r\n");
                            break;
                        case 6:
                            PRINT("Status0.Reset Complete\r\n");
                            break;
                        case 7:
                            PRINT("Status0.PHY Interrupt\r\n");
                            break;
                        case 8:
                            PRINT("Status0.Transmit Timestamp Capture Available A\r\n");
                            break;
                        case 9:
                            PRINT("Status0.Transmit Timestamp Capture Available B\r\n");
                            break;
                        case 10:
                            PRINT("Status0.Transmit Timestamp Capture Available C\r\n");
                            break;
                        case 11:
                            PRINT("Status0.Transmit Frame Check Sequence Error\r\n");
                            break;
                        case 12:
                            PRINT("Status0.Control Data Protection Error\r\n");
                            break;
                        default:
                            PRINT("Status0.Unknown Bit=%d reg-val=0x%d\r\n", i, value);
                            break;
                    }
                }
            }
            if (reinit) {
                pDrvInst->initState = DRV_LAN865X_INITSTATE_RESET;
                pDrvInst->state = SYS_STATUS_UNINITIALIZED;
            }
        } else {
            PRINT("Config0 register read failed\r\n");
        }
    }
}

static void _OnPlcaStatus(TC6_t *pInst, bool success, uint32_t addr, uint32_t value, void *tag, void *pGlobalTag)
{
    (void)pInst;
    (void)addr;
    (void)tag;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        if (success) {
            pDrvInst->plcaStatus = (0u != ((1u << 15u) & value));
        } else {
            pDrvInst->plcaStatus = false;
        }
    }
}

static void _TxDone(TC6_t *pInst, const uint8_t *pTx, uint16_t len, void *pTag, void *pGlobalTag)
{
    (void)pInst;
    (void)pTx;
    (void)len;
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(pGlobalTag);
    if (NULL != pDrvInst) {
        TCPIP_MAC_PACKET* pkt = (TCPIP_MAC_PACKET*) pTag;
        SYS_ASSERT(0 != (pkt->pktFlags & TCPIP_MAC_PKT_FLAG_QUEUED), "Non enqueued TX packet acknowledged");
        pDrvInst->txStats.nTxPendBuffers--;
        pDrvInst->txStats.nTxOkPackets++;
        pkt->pktFlags &= ~TCPIP_MAC_PKT_FLAG_QUEUED;
        (*pkt->ackFunc)(pkt, pkt->ackParam);
    }
}

static void _RxPacketAck(TCPIP_MAC_PACKET* pkt, const void* param)
{
    DRV_LAN865X_DriverInfo *pDrvInst = _Dereference(param);
    if (NULL != pDrvInst) {
        pDrvInst->rxStats.nRxPendBuffers--;
        pDrvInst->rxStats.nRxOkPackets++;
        TCPIP_Helper_ProtectedSingleListTailAdd(&pDrvInst->rxFreePackets, (SGL_LIST_NODE*) pkt);
    }
}
