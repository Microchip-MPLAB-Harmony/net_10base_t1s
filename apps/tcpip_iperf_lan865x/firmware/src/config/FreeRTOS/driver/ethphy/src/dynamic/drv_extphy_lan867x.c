/*******************************************************************************
  MCHP LAN867x PHY API for Microchip TCP/IP Stack
*******************************************************************************/
/*****************************************************************************
 Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.

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

/******************************************************************************
 *  INCLUDES
 ******************************************************************************/

#include "driver/ethphy/src/dynamic/drv_extphy_lan867x.h"
#include "driver/ethphy/src/drv_ethphy_local.h"

/******************************************************************************
 *  PRIVATE FUNCTION DECLARATIONS
 ******************************************************************************/
static void Set_Operation_Flow(const uint32_t regAddr, const DRV_MIIM_OP_TYPE opType,
                               ePHY_REG_ACCESS_PHASE *opState);
static DRV_MIIM_RESULT Lan867x_Miim_Task(LAN867X_REG_OBJ *clientObj, DRV_MIIM_OP_TYPE opType,
                                         uint32_t regAddr, uint16_t *data);

/******************************************************************************
 *  FUNCTION DEFINITIONS
 *****************************************************************************/
/****************************************************************************
 * Function:        DRV_EXTPHY_MIIConfigure
 *
 * PreCondition:    Communication to the PHY should have been established.
 *
 * Input:           handle - A valid open-instance handle, returned from the driver's open
 *routine cFlags - the requested configuration flags:
 *DRV_ETHPHY_CFG_RMII/DRV_ETHPHY_CFG_MII
 *
 * Output:          DRV_ETHPHY_RES_OK      - operation completed successfully
 *                  DRV_ETHPHY_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_ETHPHY_RES_OK or an error reported < 0                    - an error
 *occurred and the operation was aborted
 *
 * Side Effects:    None
 *
 * Overview:        This function configures the PHY in one of MII/RMII operation modes.
 *
 * Note:
 *****************************************************************************/
static DRV_ETHPHY_RESULT DRV_EXTPHY_MIIConfigure(const DRV_ETHPHY_OBJECT_BASE *pBaseObj,
                                                 DRV_HANDLE hClientObj,
                                                 DRV_ETHPHY_CONFIG_FLAGS cFlags)
{
    (void)cFlags;
    uint16_t registerValue = 0;
    uint16_t state = 0;
    LAN867X_REG_OBJ clientObj = {0};
    DRV_MIIM_RESULT miimRes = DRV_MIIM_RES_OK;
    DRV_ETHPHY_RESULT res = DRV_ETHPHY_RES_OK;
    bool inProgress = true;

    /* Get the miim client instance data. */
    clientObj.miimBase =
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->pMiimBase; /* MIIM driver base object to use. */
    clientObj.miimHandle =
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->miimHandle; /* MMIM client handle. */
    clientObj.miimOpHandle = &((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)
                                  ->miimOpHandle; /* current MIIM op in progress. */

    /* Get the phy address. */
    pBaseObj->DRV_ETHPHY_PhyAddressGet(hClientObj, DRV_ETHPHY_INF_IDX_ALL_EXTERNAL,
                                       &clientObj.phyAddress);

    /* Get the vendor data. */
    pBaseObj->DRV_ETHPHY_VendorDataGet(hClientObj, &clientObj.vendorData);

    /* Get the state information from vendor data. */
    state = R2F(clientObj.vendorData, VENDOR_STATE);

    switch (state) {
    case 0:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F00D0, 0x0E03, 0x0002);
        break;

    case 1:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F00D1, 0x0300, 0x0000);
        break;

    case 2:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0084, 0xFFC0, 0x3380);
        break;

    case 3:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0085, 0x000F, 0x0006);
        break;

    case 4:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F008A, 0xF800, 0xC000);
        break;

    case 5:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0087, 0x801C, 0x801C);
        break;

    case 6:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0088, 0x1FFF, 0x033F);
        break;

    case 7:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F008B, 0xFFFF, 0x0404);
        break;

    case 8:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0080, 0x0600, 0x0600);
        break;

    case 9:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F00F1, 0x7F00, 0x2400);
        break;

    case 10:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0096, 0x2000, 0x2000);
        break;

    case 11:
        miimRes = Lan867x_Write_Bit_Register(&clientObj, 0x1F0099, 0xFFFF, 0x7F80);
        break;

    case 12: /* Set the PLCA Burst setting. */
#ifdef DRV_ETHPHY_PLCA_ENABLED
        registerValue = F2R_(DRV_ETHPHY_PLCA_BURST_TIMER, PHY_PLCA_BURST_BTMR) |
                        F2R_(DRV_ETHPHY_PLCA_MAX_BURST_COUNT, PHY_PLCA_BURST_MAXBC);
        miimRes = Lan867x_Write_Register(&clientObj, PHY_PLCA_BURST, registerValue);
#endif
        break;

    case 13: /* Set the PLCA node setting.*/
#ifdef DRV_ETHPHY_PLCA_ENABLED
        registerValue = F2R_(DRV_ETHPHY_PLCA_LOCAL_NODE_ID, PHY_PLCA_CTRL1_ID0) |
                        F2R_(DRV_ETHPHY_PLCA_NODE_COUNT, PHY_PLCA_CTRL1_NCNT);
        miimRes = Lan867x_Write_Register(&clientObj, PHY_PLCA_CONTROL_1, registerValue);
#endif
        break;

    case 14: /* Enable the PLCA. */
#ifdef DRV_ETHPHY_PLCA_ENABLED
        miimRes =
            Lan867x_Write_Register(&clientObj, PHY_PLCA_CONTROL_0, F2R_(1, PHY_PLCA_CTRL0_EN));
#else
        miimRes =
            Lan867x_Write_Register(&clientObj, PHY_PLCA_CONTROL_0, F2R_(0, PHY_PLCA_CTRL0_EN));
#endif
        break;

    default:
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->operRes = DRV_ETHPHY_RES_OK;
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->miimOpHandle = 0;
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->smiTxferStatus = DRV_ETHPHY_SMI_TXFER_OP_NONE;
        res = DRV_ETHPHY_RES_OK;
        inProgress = false;
        break;
    }

    if (inProgress == true) {
        if (miimRes < 0) {
            res = DRV_ETHPHY_RES_MIIM_ERR;
        } else if (miimRes != DRV_MIIM_RES_OK) {
            res = DRV_ETHPHY_RES_PENDING;
        } else {
            /* If operation is completed continue to below code. */
            ++state;
            clientObj.vendorData =
                F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj.vendorData);
            res = DRV_ETHPHY_RES_PENDING;
        }
    }

    /* Write back the state to vendor data. */
    clientObj.vendorData = F2R(state, VENDOR_STATE, clientObj.vendorData);
    pBaseObj->DRV_ETHPHY_VendorDataSet(hClientObj, clientObj.vendorData);
    return res;
}

/****************************************************************************
 * Function:        DRV_EXTPHY_MDIXConfigure
 *
 * PreCondition:    Communication to the PHY should have been established.
 *
 * Input:           handle - A valid open-instance handle, returned from the driver's open
 *routine oFlags - the requested open flags: TCPIP_ETH_OPEN_MDIX_AUTO,
 *TCPIP_ETH_OPEN_MDIX_NORM/TCPIP_ETH_OPEN_MDIX_SWAP
 *
 * Output:          DRV_ETHPHY_RES_OK      - operation completed successfully
 *                  DRV_ETHPHY_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_ETHPHY_RES_OK or an error reported < 0                    - an error
 *occurred and the operation was aborted
 *
 * Side Effects:    None
 *
 * Overview:        This function configures the MDIX mode for the PHY.
 *
 * Note:            None
 *****************************************************************************/
static DRV_ETHPHY_RESULT DRV_EXTPHY_MDIXConfigure(const DRV_ETHPHY_OBJECT_BASE *pBaseObj,
                                                  DRV_HANDLE hClientObj,
                                                  TCPIP_ETH_OPEN_FLAGS oFlags)
{
    (void)pBaseObj;
    (void)hClientObj;
    (void)oFlags;
    return DRV_ETHPHY_RES_OK;
}

/****************************************************************************
 * Function:        DRV_EXTPHY_SMIClockGet
 *
 * PreCondition:    None
 *
 * Input:           handle - A valid open-instance handle, returned from the driver's open
 *routine
 *
 * Output:          PHY MIIM clock, Hz
 *
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the maximum clock frequency that the PHY can use
 *for the MIIM transactions
 *
 * Note:            None
 *****************************************************************************/
static unsigned int DRV_EXTPHY_SMIClockGet(const DRV_ETHPHY_OBJECT_BASE *pBaseObj,
                                           DRV_HANDLE handle)
{
    (void)pBaseObj;
    (void)handle;
    return 2500000; /*  2.5 MHz max clock supported. */
}

/****************************************************************************
 * Function:        DRV_ETHPHY_Detect
 *
 * PreCondition:    Communication to the PHY should have been established.
 *
 * Input:           pBaseObj - A valid open-instance handle, returned from the driver's
 *open routine
 *
 * Output:          DRV_ETHPHY_RES_OK      - operation completed successfully
 *                  DRV_ETHPHY_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_ETHPHY_RES_OK or an error reported < 0                    - an error
 *occurred and the operation was aborted
 *
 * Side Effects:    None
 *
 * Overview:        This function checks if the phy matches to the driver..
 *
 * Note:            None
 *****************************************************************************/
static DRV_ETHPHY_RESULT DRV_ETHPHY_Detect(const struct DRV_ETHPHY_OBJECT_BASE_TYPE *pBaseObj,
                                           DRV_HANDLE hClientObj)
{
    uint16_t registerValue = 0;
    uint16_t state = 0;
    LAN867X_REG_OBJ clientObj = {0};
    DRV_MIIM_RESULT miimRes = DRV_MIIM_RES_OK;
    DRV_ETHPHY_RESULT res = DRV_ETHPHY_RES_OK;

    /* Get the miim client instance data. */
    /* MIIM driver base object to use. */
    clientObj.miimBase = ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->pMiimBase;
    /* MMIM client handle. */
    clientObj.miimHandle = ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->miimHandle;
    /* current MIIM op in progress. */
    clientObj.miimOpHandle = &((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->miimOpHandle;

    /* Get the phy address. */
    pBaseObj->DRV_ETHPHY_PhyAddressGet(hClientObj, DRV_ETHPHY_INF_IDX_ALL_EXTERNAL,
                                       &clientObj.phyAddress);

    /* Get the vendor data. */
    pBaseObj->DRV_ETHPHY_VendorDataGet(hClientObj, &clientObj.vendorData);

    /* Get the state information from vendor data. */
    state = R2F(clientObj.vendorData, VENDOR_STATE);

    switch (state) {
    case 0: /* Check if PHY ID 1 matches. */
        miimRes = Lan867x_Read_Register(&clientObj, PHY_REG_PHYID1, &registerValue);

        if (miimRes < 0) {
            res = DRV_ETHPHY_RES_MIIM_ERR;
        } else if (miimRes != DRV_MIIM_RES_OK) {
            res = DRV_ETHPHY_RES_PENDING;
        } else {
            /* Verify the PHY is LAN867x, else return error.*/
            if (registerValue != (uint16_t)0x07) {
                res = DRV_ETHPHY_RES_CPBL_ERR;
            } else {
                ++state;
                clientObj.vendorData =
                    F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj.vendorData);
                res = DRV_ETHPHY_RES_PENDING;
            }
        }
        break;

    case 1: /* Check if PHY ID 2 matches. */
        miimRes = Lan867x_Read_Register(&clientObj, PHY_REG_PHYID2, &registerValue);

        if (miimRes < 0) {
            res = DRV_ETHPHY_RES_MIIM_ERR;
        } else if (miimRes != DRV_MIIM_RES_OK) {
            res = DRV_ETHPHY_RES_PENDING;
        } else {
            /* Verify the PHY is LAN867x, else return error.*/
            if (registerValue != (uint16_t)0xC162) {
                res = DRV_ETHPHY_RES_CPBL_ERR;
            } else {
                ++state;
                clientObj.vendorData =
                    F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj.vendorData);
                res = DRV_ETHPHY_RES_PENDING;
            }
        }
        break;

    default:
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->operRes = DRV_ETHPHY_RES_OK;
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->miimOpHandle = 0;
        ((DRV_ETHPHY_CLIENT_OBJ *)hClientObj)->smiTxferStatus = DRV_ETHPHY_SMI_TXFER_OP_NONE;
        res = DRV_ETHPHY_RES_OK;
        break;
    }
    /* Write back the state to vendor data. */
    clientObj.vendorData = F2R(state, VENDOR_STATE, clientObj.vendorData);
    pBaseObj->DRV_ETHPHY_VendorDataSet(hClientObj, clientObj.vendorData);
    return res;
}

/* the DRV_ETHPHY_OBJECT */
const DRV_ETHPHY_OBJECT DRV_ETHPHY_OBJECT_LAN867x = {
    .miiConfigure = DRV_EXTPHY_MIIConfigure,
    .mdixConfigure = DRV_EXTPHY_MDIXConfigure,
    .smiClockGet = DRV_EXTPHY_SMIClockGet,
    .wolConfigure = 0, // no WOL functionality yet
    .phyDetect = DRV_ETHPHY_Detect,
};

/******************************************************************************
 *  PUBLIC FUNCTION DEFINITIONS
 ******************************************************************************/
/****************************************************************************
 * Function:        Lan867x_Write_Register
 *
 * PreCondition:    none
 *
 * Input:           clientObj - A valid open-instance handle.
 *                  regAddr   - Valid clause 22 or 45 register address.
 *                  wData     - data which is written to register.
 *
 * Output:          DRV_MIIM_RES_OK      - operation completed successfully.
 *                  DRV_MIIM_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_MIIM_RES_OK or an error reported. < 0                  - an error
 *occurred and the operation was aborted.
 *
 * Side Effects:    None
 *
 * Overview:        This function write value to phy registers and support Clause 22 and
 *45 operation.
 *
 * Note:
 *****************************************************************************/
DRV_MIIM_RESULT Lan867x_Write_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr,
                                       uint16_t wData)
{
    DRV_MIIM_RESULT res;

    /* Check register operation type is Clause 22 or Clause 45 type. */
    if (R2F(clientObj->vendorData, VENDOR_INTERNAL_STATE) == IDLE_PHASE) {
        /* Set the phase for the new operation. */
        ePHY_REG_ACCESS_PHASE initialState = 0;
        Set_Operation_Flow(regAddr, DRV_MIIM_OP_WRITE, &initialState);
        clientObj->vendorData =
            F2R(initialState, VENDOR_INTERNAL_STATE, clientObj->vendorData);
        clientObj->vendorData = F2R(false, VENDOR_IS_BIT_OP, clientObj->vendorData);
    }

    res = Lan867x_Miim_Task(clientObj, DRV_MIIM_OP_WRITE, regAddr, &wData);
    if (res == DRV_MIIM_RES_OK) {
        clientObj->vendorData = F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
    }

    return res;
}

/****************************************************************************
 * Function:        Lan867x_Read_Register
 *
 * PreCondition:    none
 *
 * Input:           clientObj - A valid open-instance handle.
 *                  regAddr   - Valid clause 22 or 45 register address.
 *                  rData     - pointer to data where register value is read to.
 *
 * Output:          DRV_MIIM_RES_OK      - operation completed successfully.
 *                  DRV_MIIM_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_MIIM_RES_OK or an error reported. < 0                  - an error
 *occurred and the operation was aborted.
 *
 * Side Effects:    None
 *
 * Overview:        This function read register value from phy and support Clause 22 and
 *45 operation.
 *
 * Note:
 *****************************************************************************/
DRV_MIIM_RESULT Lan867x_Read_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr,
                                      uint16_t *rData)
{
    DRV_MIIM_RESULT res;

    /* Check register operation type is Clause 22 or Clause 45 type. */
    if (R2F(clientObj->vendorData, VENDOR_INTERNAL_STATE) == IDLE_PHASE) {
        /* Set the phase for the new operation. */
        ePHY_REG_ACCESS_PHASE initialState = 0;
        Set_Operation_Flow(regAddr, DRV_MIIM_OP_READ, &initialState);
        clientObj->vendorData =
            F2R(initialState, VENDOR_INTERNAL_STATE, clientObj->vendorData);
        clientObj->vendorData = F2R(false, VENDOR_IS_BIT_OP, clientObj->vendorData);
    }

    res = Lan867x_Miim_Task(clientObj, DRV_MIIM_OP_READ, regAddr, rData);
    if (res == DRV_MIIM_RES_OK) {
        clientObj->vendorData = F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
    }

    return res;
}

/****************************************************************************
 * Function:        Lan867x_Write_Bit_Register
 *
 * PreCondition:    none
 *
 * Input:           clientObj - A valid open-instance handle.
 *                  regAddr   - Valid clause 22 or 45 register address.
 *                  mask      - mask applied to write the data.
 *                  wData     - data which is written to register.
 *
 * Output:          DRV_MIIM_RES_OK      - operation completed successfully.
 *                  DRV_MIIM_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_MIIM_RES_OK or an error reported. < 0                  - an error
 *occurred and the operation was aborted.
 *
 * Side Effects:    None
 *
 * Overview:        This function read register value and udpate the bits according to
 *mask and data, and write back.
 *
 * Note:
 *****************************************************************************/
DRV_MIIM_RESULT Lan867x_Write_Bit_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr,
                                           uint16_t mask, uint16_t wData)
{
    uint16_t regValue = 0;
    ePHY_REG_ACCESS_PHASE internalState = 0;
    DRV_MIIM_RESULT res = DRV_MIIM_RES_OK;

    /* Check register operation type is Clause 22 or Clause 45 type. */
    if (R2F(clientObj->vendorData, VENDOR_INTERNAL_STATE) == IDLE_PHASE) {
        /* Set the phase for the new operation. */
        Set_Operation_Flow(regAddr, DRV_MIIM_OP_READ, &internalState);
        clientObj->vendorData =
            F2R(internalState, VENDOR_INTERNAL_STATE, clientObj->vendorData);
        /* Set the bit value as true, to understand it as the first stage of Bit
         * operation. */
        clientObj->vendorData = F2R(true, VENDOR_IS_BIT_OP, clientObj->vendorData);
    }

    /* The read part of the bit set operation. */
    if (R2F(clientObj->vendorData, VENDOR_IS_BIT_OP) == true) {
        res = Lan867x_Miim_Task(clientObj, DRV_MIIM_OP_READ, regAddr, &regValue);
        if (res == DRV_MIIM_RES_OK) {
            /* Set the bit false to start the write operation. */
            clientObj->vendorData = F2R(false, VENDOR_IS_BIT_OP, clientObj->vendorData);
            clientObj->vendorData = F2R(((regValue & (uint16_t)~mask) | (wData & mask)),
                                        VENDOR_DATA, clientObj->vendorData);

            /* Set the phase for the new operation. */
            Set_Operation_Flow(regAddr, DRV_MIIM_OP_WRITE, &internalState);
            clientObj->vendorData =
                F2R(internalState, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            res = DRV_MIIM_RES_PENDING;
        }
    }
    /* The write part of the bit set operation. */
    else {
        regValue = R2F(clientObj->vendorData, VENDOR_DATA);
        res = Lan867x_Miim_Task(clientObj, DRV_MIIM_OP_WRITE, regAddr, &regValue);
        if (res == DRV_MIIM_RES_OK) {
            clientObj->vendorData =
                F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            res = DRV_MIIM_RES_OK;
        }
    }

    return res;
}

/******************************************************************************
 *  PRIVATE FUNCTION DEFINITIONS
 ******************************************************************************/

/****************************************************************************
 * Function:        Lan867x_Miim_Task
 *
 * PreCondition:    none
 *
 * Input:           clientObj - A valid open-instance handle.
 *                  opType - pointer to data where register value is read to.
 *                  regAddr - Valid clause 22 or 45 register address.
 *                  data - pointer to data where register value is read to.
 *
 * Output:          DRV_MIIM_RES_OK      - operation completed successfully.
 *                  DRV_MIIM_RES_PENDING - operation pending; the call needs to be
 *re-issued until DRV_MIIM_RES_OK or an error reported. < 0                  - an error
 *occurred and the operation was aborted.
 *
 * Side Effects:    None
 *
 * Overview:        This function does the phy operation task, to access register and
 *support Clause 22 and 45 operation.
 *
 * Note:
 *****************************************************************************/
static DRV_MIIM_RESULT Lan867x_Miim_Task(LAN867X_REG_OBJ *clientObj, DRV_MIIM_OP_TYPE opType,
                                         uint32_t regAddr, uint16_t *data)
{
    uint16_t mmdData = 0;
    DRV_MIIM_RESULT opRes = DRV_MIIM_RES_OK;

    switch (R2F(clientObj->vendorData, VENDOR_INTERNAL_STATE)) {
    case WRITE_22_PHASE: /* Write to clause 22 register. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Write(
            clientObj->miimHandle, regAddr, clientObj->phyAddress, *data,
            DRV_MIIM_OPERATION_FLAG_DISCARD, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* Operation successfully completed.*/
            clientObj->vendorData =
                F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_OK;
        }
        break;

    case READ_22_PHASE: /* Read from clause 22 register. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Read(
            clientObj->miimHandle, regAddr, clientObj->phyAddress,
            DRV_MIIM_OPERATION_FLAG_NONE, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* advance to the next phase. */
            clientObj->vendorData =
                F2R(READ_RESULT_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_PENDING;
        }
        break;

    case READ_RESULT_PHASE: /* Get read result. */
        opRes = clientObj->miimBase->DRV_MIIM_OperationResult(clientObj->miimHandle,
                                                              *clientObj->miimOpHandle, data);
        if (opRes != DRV_MIIM_RES_PENDING) /* Check operation is in progress or not. */
        {
            /* Operation successfully completed.*/
            clientObj->vendorData =
                F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            *clientObj->miimOpHandle = 0;
        }
        break;

    case MMD_ADDR_CONFIG_PHASE: /* Initiate clause 45 operation. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Write(
            clientObj->miimHandle, PHY_MMD_ACCESS_CONTROL, clientObj->phyAddress,
            (regAddr >> 16), DRV_MIIM_OPERATION_FLAG_DISCARD, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* advance to the next phase. */
            clientObj->vendorData =
                F2R(MMD_ADDR_SET_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_PENDING;
        }
        break;

    case MMD_ADDR_SET_PHASE: /* Set clause 45 address phase operation. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Write(
            clientObj->miimHandle, PHY_MMD_ACCESS_DATA_ADDR, clientObj->phyAddress,
            (regAddr & (uint16_t)0xffff), DRV_MIIM_OPERATION_FLAG_DISCARD, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* advance to the next phase. */
            clientObj->vendorData =
                F2R(MMD_DATA_CONFIG_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_PENDING;
        }
        break;

    case MMD_DATA_CONFIG_PHASE: /* Set clause 45 data phase operation. */
        mmdData = (F2R_((regAddr >> 16), PHY_MMDCTRL_DEVAD) | F2R_(1, PHY_MMDCTRL_FNCTN));
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Write(
            clientObj->miimHandle, PHY_MMD_ACCESS_CONTROL, clientObj->phyAddress, mmdData,
            DRV_MIIM_OPERATION_FLAG_DISCARD, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* advance to the next phase. */
            if (opType == DRV_MIIM_OP_READ) {
                clientObj->vendorData =
                    F2R(MMD_DATA_READ_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            } else {
                clientObj->vendorData =
                    F2R(MMD_DATA_WRITE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            }
            opRes = DRV_MIIM_RES_PENDING;
        }
        break;

    case MMD_DATA_READ_PHASE: /* Start clause 45 data read phase. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Read(
            clientObj->miimHandle, PHY_MMD_ACCESS_DATA_ADDR, clientObj->phyAddress,
            DRV_MIIM_OPERATION_FLAG_NONE, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* advance to the next phase. */
            clientObj->vendorData =
                F2R(READ_RESULT_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_PENDING;
        }
        break;

    case MMD_DATA_WRITE_PHASE: /* Start clause 45 data write phase. */
        *clientObj->miimOpHandle = clientObj->miimBase->DRV_MIIM_Write(
            clientObj->miimHandle, PHY_MMD_ACCESS_DATA_ADDR, clientObj->phyAddress, *data,
            DRV_MIIM_OPERATION_FLAG_DISCARD, &opRes);
        /* If success in queuing the request, go to next state, else retry. */
        if (*clientObj->miimOpHandle != 0) {
            /* Operation successfully completed.*/
            clientObj->vendorData =
                F2R(IDLE_PHASE, VENDOR_INTERNAL_STATE, clientObj->vendorData);
            opRes = DRV_MIIM_RES_OK;
        }
        break;

    case IDLE_PHASE:
        opRes = DRV_MIIM_RES_OK;
        break;

    default:
        /* shouldn't happen */
        opRes = DRV_MIIM_RES_OP_INTERNAL_ERR;
        break;
    }
    return opRes;
}

/****************************************************************************
 * Function:        Set_Operation_Flow
 *
 * PreCondition:    none
 *
 * Input:           regAddr - Valid clause 22 or 45 register address.
 *                  opType  - Read or Write Operation type.
 *                  opState - initial state for the task to access register data.
 *
 * Side Effects:    None
 *
 * Overview:        This function set operation register access mode. Whether Clause 22 or
 *clause 45, and read or write.
 *
 * Note:
 *****************************************************************************/
static void Set_Operation_Flow(const uint32_t regAddr, const DRV_MIIM_OP_TYPE opType,
                               ePHY_REG_ACCESS_PHASE *opState)
{
    if (regAddr < (uint32_t)0x20) {
        if (opType == DRV_MIIM_OP_READ) {
            *opState = READ_22_PHASE;
        } else {
            *opState = WRITE_22_PHASE;
        }
    } else {
        *opState = MMD_ADDR_CONFIG_PHASE;
    }
}
