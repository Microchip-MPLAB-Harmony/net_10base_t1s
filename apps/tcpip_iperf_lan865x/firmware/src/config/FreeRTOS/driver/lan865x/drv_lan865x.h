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
    drv_lan865x.h

  Summary:
    LAN865X Driver interface definition.

  Description:
    This file defines the interface definition for the LAN865X Driver.

*******************************************************************************/

#if !defined(DRV_LAN865X_H_)
#define DRV_LAN865X_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/driver_common.h"
#include "driver/spi/drv_spi_definitions.h"
#include "tcpip/tcpip_mac.h"
#include "tcpip/tcpip_ethernet.h"
#include "tcpip/tcpip_mac_object.h"
#include "system/system_module.h"
#include "system/system_common.h"
#include <stdbool.h>
#include <string.h>
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************

// *****************************************************************************
/* LAN865X Driver Initialization Data

  Summary:
    Defines the data required to initialize or reinitialize the LAN865X Driver.

  Description:
    This data type defines the data required to initialize or reinitialize the
    LAN865X driver. If the driver is built statically, the members of this data
    structure are statically over-ridden by static override definitions in the
    system_config.h file.

  Remarks:
    None.
*/

typedef struct _DRV_LAN865X_Configuration
{
    /* Index of the SPI driver to use */
    SYS_MODULE_INDEX spiDrvIndex;

    SYS_PORT_PIN spiChipSelectPin;

    SYS_PORT_PIN  interruptPin;

    SYS_PORT_PIN  resetPin;

    uint32_t spiFrequecny;

    uint16_t rxDescriptors;

    uint16_t rxDescBufferSize;

    uint8_t chunkSize;

    uint8_t nodeId;

    uint8_t nodeCount;

    uint8_t burstCount;

    uint8_t burstTimer;

    bool plcaEnable;

    bool promiscuous;

    bool txCutThrough;

    bool rxCutThrough;

} DRV_LAN865X_Configuration;

// *****************************************************************************
// *****************************************************************************
// Section: Global Data - System Level
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* LAN865X External MAC Virtualization Table
extern const TCPIP_MAC_OBJECT DRV_LAN865X_MACObject;
*/

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - System Level
// *****************************************************************************
// *****************************************************************************

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
      - Valid handle to the driver instance     - If successful
      - SYS_MODULE_OBJ_INVALID                  - If unsuccessful 
*/
SYS_MODULE_OBJ DRV_LAN865X_Initialize(SYS_MODULE_INDEX index, SYS_MODULE_INIT * init);

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
void DRV_LAN865X_Deinitialize(SYS_MODULE_OBJ object);

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
      - object  - The object valid passed back to DRV_LAN865X_Initialize
      - init    - The new initialization structure.

    Returns:
      None

    */
void DRV_LAN865X_Reinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT * const init);

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
SYS_STATUS DRV_LAN865X_Status(SYS_MODULE_OBJ obect);

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
void DRV_LAN865X_Tasks(SYS_MODULE_OBJ object);

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
      index - This is the index of the driver instance to be initialized. The definition 
              DRV_LAN865X_NUM_DRV_INSTANCES controls how many instances are available.
      init  - This is a pointer to a TCPIP_MAC_INIT structure.

    Returns:
      Returns a valid handle to the driver instance - If successful
      SYS_MODULE_OBJ_INVALID                        - If unsuccessful
*/
SYS_MODULE_OBJ DRV_LAN865X_StackInitialize(SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Client Level
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
      INVALID_HANDLE         - If unsuccessful
*/
DRV_HANDLE DRV_LAN865X_Open(SYS_MODULE_INDEX index, DRV_IO_INTENT intent);

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
void DRV_LAN865X_Close(DRV_HANDLE handle);

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
bool DRV_LAN865X_LinkCheck(DRV_HANDLE hMac);

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
      hMac          - the successfully opened handle
      DestMACAddr   - MAC address to add to the hash table

    Returns:
      - TCPIP_MAC_RES_TYPE_ERR  - if the hMac is invalid
      - TCPIP_MAC_RES_OP_ERR    - if the hMac is valid
 */
TCPIP_MAC_RES DRV_LAN865X_RxFilterHashTableEntrySet(DRV_HANDLE hMac, const TCPIP_MAC_ADDR* DestMACAddr);

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
      hMac      - the successfully opened handle
      pwrMode   - the power mode to set

    Returns:
      - false   - This functionality is not supported in this version of the driver
*/
bool DRV_LAN865X_PowerMode(DRV_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode);

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
TCPIP_MAC_RES DRV_LAN865X_PacketTx(DRV_HANDLE hMac, TCPIP_MAC_PACKET * ptrPacket);

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
TCPIP_MAC_PACKET* DRV_LAN865X_PacketRx(DRV_HANDLE hMac, TCPIP_MAC_RES* pRes, TCPIP_MAC_PACKET_RX_STAT* pPktStat);

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
TCPIP_MAC_RES DRV_LAN865X_Process(DRV_HANDLE hMac);

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
TCPIP_MAC_RES DRV_LAN865X_StatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_RX_STATISTICS* pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics);

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
      - TCPIP_MAC_RES_TYPE_ERR    - if the hMac is invalid
      - TCPIP_MAC_RES_OK        - if the hMac is valid
*/
TCPIP_MAC_RES DRV_LAN865X_ParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS* pMacParams);

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
TCPIP_MAC_RES DRV_LAN865X_RegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries);

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
size_t DRV_LAN865X_ConfigGet(DRV_HANDLE hMac, void* configBuff, size_t buffSize, size_t* pConfigSize);

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
      hMac       - the successfully opened handle
      macEvents  - the mask to enable or disable
      enable     - to enable or disable events

    Returns
      - true     - if the mask could be set
      - false    - if the mast could not be set
*/
bool DRV_LAN865X_EventMaskSet(DRV_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable);

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
bool DRV_LAN865X_EventAcknowledge(DRV_HANDLE hMac, TCPIP_MAC_EVENT macEvents);

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
      - TCPIP_MAC_EV_NONE    - Returned on success
      - List of events        - Returned on event other than an error

*/
TCPIP_MAC_EVENT DRV_LAN865X_EventPendingGet(DRV_HANDLE hMac);

// *****************************************************************************
/* LAN865X Driver Callback Function Definition

  Summary:
    Defines a callback function, which is getting executed when ever a register access was finished.

  Description:
    This function may be implemented by the integrator and passed as argument with
    DRV_LAN865X_ReadRegister() or DRV_LAN865X_WriteRegister() or DRV_LAN865X_ReadModifyWriteRegister().

  Parameters:
    reserved1   - For internal usage only. Do not use.
    success     - true, if the register could be accessed without errors. false, there was an error while trying to access the register.
    addr        - The register address, as passed a long with DRV_LAN865X_ReadRegister() or DRV_LAN865X_WriteRegister() or DRV_LAN865X_ReadModifyWriteRegister().
    value       - The register value. If this belongs to a write request, it is the same value as given along with DRV_LAN865X_WriteRegister(). In case of DRV_LAN865X_ReadRegister(), this holds the register read value. In case of DRV_LAN865X_ReadModifyWriteRegister() it holds the final value, which was written back into the MAC/PHY.
    pTag        - Tag pointer which was given along with the register access functions.
    reserved2   - For internal usage only. Do not use.

  Remarks:
    It is safe inside this callback to call again DRV_LAN865X_ReadRegister() or DRV_LAN865X_WriteRegister() or DRV_LAN865X_ReadModifyWriteRegister().
*/
typedef void (*DRV_LAN865X_RegCallback_t)(void *reserved1, bool success, uint32_t addr, uint32_t value, void *pTag, void *reserved2);

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
TCPIP_MAC_RES DRV_LAN865X_ReadRegister(uint8_t idx, uint32_t addr, bool protected, DRV_LAN865X_RegCallback_t rxCallback, void *pTag);

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
TCPIP_MAC_RES DRV_LAN865X_WriteRegister(uint8_t idx, uint32_t addr, uint32_t value, bool protected, DRV_LAN865X_RegCallback_t txCallback, void *pTag);

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
TCPIP_MAC_RES DRV_LAN865X_ReadModifyWriteRegister(uint8_t idx, uint32_t addr, uint32_t value, uint32_t mask, bool protected, DRV_LAN865X_RegCallback_t modifyCallback, void *pTag);

#ifdef __cplusplus
}
#endif

#endif /*!defined(DRV_LAN865X_H_)*/

