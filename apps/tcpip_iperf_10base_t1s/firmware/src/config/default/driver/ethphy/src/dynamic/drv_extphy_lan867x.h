/*******************************************************************************
  MCHP LAN867x definitions

  Company:
    Microchip Technology Inc.
    
  File Name:
    drv_extphy_lan867x.h

  Summary:
    MCHP LAN867x definitions

  Description:
    This file provides the MCHP LAN867x definitions.
*******************************************************************************/
// DOM-IGNORE-BEGIN
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

// DOM-IGNORE-END

#ifndef LAN867x_H_
#define LAN867x_H_

/******************************************************************************
*  INCLUDES
******************************************************************************/

#include "driver/miim/drv_miim.h"
#include "driver/miim/src/drv_miim_local.h"

/******************************************************************************
*  DEFINES & MACROS
******************************************************************************/

/* converts field descriptor to a field width */
#define FIELD_WIDTH(fd) ((fd)&255)

/* converts field descriptor to a field offset */
#define FIELD_OFFSET(fd) ((fd) >> 8 & 255)

/* converts field descriptor to bit mask of a field, like 0b0110 */
#define FIELD_MASK(fd) (((1u << FIELD_WIDTH(fd)) - 1) << FIELD_OFFSET(fd))

/* converts field offset and field width to a field descriptor */
#define FIELD_DESCRIPTOR(offset, width) ((offset) << 8 | (width))

/* converts register mask to a field value */
#define R2F(reg, fd) (((unsigned)(reg)&FIELD_MASK(fd)) >> FIELD_OFFSET(fd))

/* converts field value to a register mask */
/* may only be used for register initialization */
#define F2R_(val, fd) (((unsigned)(val) << FIELD_OFFSET(fd)) & FIELD_MASK(fd))

/* returns modified value reg for a field descriptor fd */
#define F2R(val, fd, reg) (((reg) & ~FIELD_MASK(fd)) | F2R_(val, fd))

/******************************************************************************
*  ENUM
******************************************************************************/

typedef enum {
    IDLE_PHASE = 0,
    READ_22_PHASE,
    WRITE_22_PHASE,
    READ_RESULT_PHASE,
    /* Expand if you need. */

    MMD_ADDR_CONFIG_PHASE,
    MMD_ADDR_SET_PHASE,
    MMD_DATA_CONFIG_PHASE,
    MMD_DATA_READ_PHASE,
    MMD_DATA_WRITE_PHASE,
} ePHY_REG_ACCESS_PHASE;

typedef enum {
    /* Specific vendor registers: 16-31 */
    PHY_MMD_ACCESS_CONTROL = 13,
    PHY_MMD_ACCESS_DATA_ADDR = 14,
    PHY_STRAP_CONTROL0 = 18,

    /* Clause 45 registers. */
    /* PMA/PMD registers. */
    PHY_PMA_EXT_ABILITY = 0x010012,
    PHY_T1S_PMA_PMD_CTRL = 0x010834,
    PHY_T1S_PMA_CTRL = 0x0108F9,
    PHY_T1S_PMA_STATUS = 0x0108FA,
    PHY_T1S_TEST_CTRL = 0x0108FB,

    /* PCS registers. */
    PHY_T1S_PCS_CTRL = 0x0308F3,
    PHY_T1S_PCS_STATUS = 0x0308F4,
    PHY_T1S_PCS_DIAG_1 = 0x0308F5,
    PHY_T1S_PCS_DIAG_2 = 0x0308F6,

    /* Miscellaneous registers. */
    PHY_CONFIGURATION_PROTECTION = 0x1F000F,
    PHY_CONTROL_1 = 0x1F0010,
    PHY_STATUS_1 = 0x1F0018,
    PHY_STATUS_2 = 0x1F0019,
    PHY_STATUS_3 = 0x1F001A,
    PHY_INTERRUPT_MASK_1 = 0x1F001C,
    PHY_INTERRUPT_MASK_2 = 0x1F001D,
    PHY_COUNTER_CONTROL = 0x1F0020,
    PHY_RECEIVE_BYTE_COUNT_HIGH = 0x1F0022,
    PHY_RECEIVE_BYTE_COUNT_LOW = 0x1F0023,
    PHY_TRANSMIT_OPPORTUNITY_COUNT_HIGH = 0x1F0024,
    PHY_TRANSMIT_OPPORTUNITY_COUNT_LOW = 0x1F0025,
    PHY_BEACON_COUNT_HIGH = 0x1F0026,
    PHY_BEACON_COUNT_LOW = 0x1F0027,
    PHY_TSTPATGENCTL = 0x1F0078,
    PHY_TSTPATFRM = 0x1F0079,
    PHY_TSTPCKCTL = 0x1F007A,
    PHY_TSTERRCNT = 0x1F007B,
    PHY_TSTPATCKSTS = 0x1F007C,
    PHY_PAD_CONTROL_3 = 0x1F00CB,
    PHY_PLCA_IDVER = 0x1FCA00,
    PHY_PLCA_CONTROL_0 = 0x1FCA01,
    PHY_PLCA_CONTROL_1 = 0x1FCA02,
    PHY_PLCA_STATUS = 0x1FCA03,
    PHY_PLCA_TO_TIMER = 0x1FCA04,
    PHY_PLCA_BURST = 0x1FCA05,
} ePHY_VENDOR_REG;

/******************************************************************************
*  STRUCTURE
******************************************************************************/

typedef struct {
    int phyAddress;                          /* Address of the phy. */
    uint32_t vendorData;                     /* Used to maintain the state machine of operation. */
    DRV_HANDLE miimHandle;                   /* Instance of MIIM object. */
    const DRV_MIIM_OBJECT_BASE *miimBase;    /* Instance to MIIM functional pointer. */
    DRV_MIIM_OPERATION_HANDLE *miimOpHandle; /* Handle to a MIIM operation. */
} LAN867X_REG_OBJ;

/******************************************************************************
*  VENDOR DATA MACRO
******************************************************************************/

#define VENDOR_DATA           FIELD_DESCRIPTOR(0, 16)
#define VENDOR_STATE          FIELD_DESCRIPTOR(16, 8)
#define VENDOR_IS_BIT_OP      FIELD_DESCRIPTOR(24, 1)
#define VENDOR_INTERNAL_STATE FIELD_DESCRIPTOR(25, 7)

/******************************************************************************
*  VENDOR REGISTER MACRO
******************************************************************************/

/* PHY_REG_BMCON                        : 0 */
#define PHY_BMCON_UNIDIR_EN   FIELD_DESCRIPTOR(5, 1)
#define PHY_BMCON_SPD_SEL_1   FIELD_DESCRIPTOR(6, 1)
#define PHY_BMCON_COL_TEST    FIELD_DESCRIPTOR(7, 1)
#define PHY_BMCON_DUPLEX_MODE FIELD_DESCRIPTOR(8, 1)
#define PHY_BMCON_RE_AUTO_NEG FIELD_DESCRIPTOR(9, 1)
#define PHY_BMCON_ISOLATE     FIELD_DESCRIPTOR(10, 1)
#define PHY_BMCON_PD          FIELD_DESCRIPTOR(11, 1)
#define PHY_BMCON_AUTO_NEG_EN FIELD_DESCRIPTOR(12, 1)
#define PHY_BMCON_SPD_SEL_0   FIELD_DESCRIPTOR(13, 1)
#define PHY_BMCON_LOOPBACK    FIELD_DESCRIPTOR(14, 1)
#define PHY_BMCON_SW_RESET    FIELD_DESCRIPTOR(15, 1)

/* PHY_REG_BMSTAT                       : 1 */
#define PHY_BMSTAT_EXT_CAP           FIELD_DESCRIPTOR(0, 1)
#define PHY_BMSTAT_JAB_DET           FIELD_DESCRIPTOR(1, 1)
#define PHY_BMSTAT_LINK_STAT         FIELD_DESCRIPTOR(2, 1)
#define PHY_BMSTAT_AUTO_NEG          FIELD_DESCRIPTOR(3, 1)
#define PHY_BMSTAT_RMT_FAULT         FIELD_DESCRIPTOR(4, 1)
#define PHY_BMSTAT_AUTO_NEG_COMP     FIELD_DESCRIPTOR(5, 1)
#define PHY_BMSTAT_MF_PRE_SUP        FIELD_DESCRIPTOR(6, 1)
#define PHY_BMSTAT_UNIDIR_ABL        FIELD_DESCRIPTOR(7, 1)
#define PHY_BMSTAT_EXT_STATUS        FIELD_DESCRIPTOR(8, 1)
#define PHY_BMSTAT_100T2_HALF_DUPLEX FIELD_DESCRIPTOR(9, 1)
#define PHY_BMSTAT_100T2_FULL_DUPLEX FIELD_DESCRIPTOR(10, 1)
#define PHY_BMSTAT_10T_HALF_DUPLEX   FIELD_DESCRIPTOR(11, 1)
#define PHY_BMSTAT_10T_FULL_DUPLEX   FIELD_DESCRIPTOR(12, 1)
#define PHY_BMSTAT_100TX_HALF_DUPLEX FIELD_DESCRIPTOR(13, 1)
#define PHY_BMSTAT_100TX_FULL_DUPLEX FIELD_DESCRIPTOR(14, 1)
#define PHY_BMSTAT_100T4             FIELD_DESCRIPTOR(15, 1)

/* PHY_REG_PHYID0                       : 2 */
#define PHY_PHY_ID0_OUI FIELD_DESCRIPTOR(0, 16)

/* PHY_REG_PHYID1                       : 3 */
#define PHY_PHY_ID1_REV   FIELD_DESCRIPTOR(0, 4)
#define PHY_PHY_ID1_MODEL FIELD_DESCRIPTOR(4, 6)
#define PHY_PHY_ID1_OUI   FIELD_DESCRIPTOR(10, 6)

/* PHY_MMD_ACCESS_CONTROL               : 13 */
#define PHY_MMDCTRL_DEVAD FIELD_DESCRIPTOR(0, 5)
#define PHY_MMDCTRL_FNCTN FIELD_DESCRIPTOR(14, 2)

/* PHY_MMD_ACCESS_DATA_ADDR             : 14 */
#define PHY_MMDAD_ADR_DATA FIELD_DESCRIPTOR(0, 16)

/* PHY_STRAP_CONTROL0                   : 18 */
#define PHY_STRAP_CTRL0_SMIADR FIELD_DESCRIPTOR(0, 5)
#define PHY_STRAP_CTRL0_PKGTYP FIELD_DESCRIPTOR(5, 2)
#define PHY_STRAP_CTRL0_MITYP  FIELD_DESCRIPTOR(7, 2)

/******    PMA/PMD registers. ******/
/* PHY_PMA_EXT_ABILITY                  : 0x010012 */
#define PHY_PMA_PMD_EXT_ABILITY_T1LABL FIELD_DESCRIPTOR(2, 1)
#define PHY_PMA_PMD_EXT_ABILITY_T1SABL FIELD_DESCRIPTOR(3, 1)

/* PHY_T1S_PMA_PMD_CTRL                 : 0x010834 */
#define PHY_T1PMAPMDCTL_TYPSEL FIELD_DESCRIPTOR(0, 4)

/* PHY_T1S_PMA_CTRL                     : 0x0108F9 */
#define PHY_T1SPMACTL_LBE FIELD_DESCRIPTOR(0, 1)
#define PHY_T1SPMACTL_MDE FIELD_DESCRIPTOR(10, 1)
#define PHY_T1SPMACTL_LPE FIELD_DESCRIPTOR(11, 1)
#define PHY_T1SPMACTL_TXD FIELD_DESCRIPTOR(14, 1)
#define PHY_T1SPMACTL_RST FIELD_DESCRIPTOR(15, 1)

/* PHY_T1S_PMA_STATUS                   : 0x0108FA */
#define PHY_T1SPMASTS_RXFD FIELD_DESCRIPTOR(1, 1)
#define PHY_T1SPMASTS_RXFA FIELD_DESCRIPTOR(9, 1)
#define PHY_T1SPMASTS_MDA  FIELD_DESCRIPTOR(10, 1)
#define PHY_T1SPMASTS_LPA  FIELD_DESCRIPTOR(11, 1)
#define PHY_T1SPMASTS_LBA  FIELD_DESCRIPTOR(13, 1)

/* PHY_T1S_TEST_CTRL                    : 0x0108FB */
#define PHY_T1STSTCTL_TSTCTL FIELD_DESCRIPTOR(13, 3)

/******   PCS registers.      ******/
/* PHY_T1S_PCS_CTRL                     : 0x0308F3 */
#define PHY_T1SPCSCTL_DUPLEX FIELD_DESCRIPTOR(8, 1)
#define PHY_T1SPCSCTL_LBE    FIELD_DESCRIPTOR(14, 1)
#define PHY_T1SPCSCTL_RST    FIELD_DESCRIPTOR(15, 1)

/* PHY_T1S_PCS_STATUS                   : 0x0308F4 */
#define PHY_T1SPCSSTS_FAULT FIELD_DESCRIPTOR(7, 1)

/* PHY_T1S_PCS_DIAG_1                   : 0x0308F5 */
#define PHY_T1SPCSDIAG1_RMTJABCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_T1S_PCS_DIAG_2                   : 0x0308F6 */
#define PHY_T1SPCSDIAG2_CORTXCNT FIELD_DESCRIPTOR(0, 16)

/****** Miscellaneous registers. ******/
/* PHY_CONFIGURATION_PROTECTION         : 0x1F000F */
#define PHY_CFGPRTCTL_WREN FIELD_DESCRIPTOR(0, 1)
#define PHY_CFGPRTCTL_KEY1 FIELD_DESCRIPTOR(14, 1)
#define PHY_CFGPRTCTL_KEY2 FIELD_DESCRIPTOR(15, 1)

/* PHY_CONTROL_1                        : 0x1F0010 */
#define PHY_CTRL1_DIGLBE FIELD_DESCRIPTOR(1, 1)
#define PHY_CTRL1_MIWDE  FIELD_DESCRIPTOR(3, 1)

/* PHY_STATUS_1                         : 0x1F0018 */
#define PHY_STS1_DEC5B   FIELD_DESCRIPTOR(0, 1)
#define PHY_STS1_PLCASYM FIELD_DESCRIPTOR(2, 1)
#define PHY_STS1_BCNBFTO FIELD_DESCRIPTOR(4, 1)
#define PHY_STS1_UNEXPB  FIELD_DESCRIPTOR(5, 1)
#define PHY_STS1_RXINTO  FIELD_DESCRIPTOR(6, 1)
#define PHY_STS1_EMPCYC  FIELD_DESCRIPTOR(7, 1)
#define PHY_STS1_TXBABL  FIELD_DESCRIPTOR(9, 1)
#define PHY_STS1_TXCOL   FIELD_DESCRIPTOR(10, 1)

/* PHY_STATUS_2                         : 0x1F0019 */
#define PHY_STS2_RESETC FIELD_DESCRIPTOR(11, 1)

/* PHY_STATUS_3                         : 0x1F001A */
#define PHY_STS3_CURID FIELD_DESCRIPTOR(0, 8)

/* PHY_INTERRUPT_MASK_1                 : 0x1F001C */
#define PHY_IMSK1_DEC5B    FIELD_DESCRIPTOR(0, 1)
#define PHY_IMSK1_PLCASYMM FIELD_DESCRIPTOR(2, 1)
#define PHY_IMSK1_BCNBFTOM FIELD_DESCRIPTOR(4, 1)
#define PHY_IMSK1_UNEXPBM  FIELD_DESCRIPTOR(5, 1)
#define PHY_IMSK1_RXINTOM  FIELD_DESCRIPTOR(6, 1)
#define PHY_IMSK1_EMPCYCM  FIELD_DESCRIPTOR(7, 1)
#define PHY_IMSK1_TXBABLM  FIELD_DESCRIPTOR(9, 1)
#define PHY_IMSK1_TXCOLM   FIELD_DESCRIPTOR(10, 1)

/* PHY_INTERRUPT_MASK_2                 : 0x1F001D */
#define PHY_IMSK2_RESETCM FIELD_DESCRIPTOR(11, 1)

/* PHY_COUNTER_CONTROL                  : 0x1F0020 */
#define PHY_CTRCTRL_BCNCNTE FIELD_DESCRIPTOR(0, 1)
#define PHY_CTRCTRL_TOCNTE  FIELD_DESCRIPTOR(1, 1)
#define PHY_CTRCTRL_RXBCNTE FIELD_DESCRIPTOR(2, 1)

/* PHY_RECEIVE_BYTE_COUNT_HIGH          : 0x1F0022 */
#define PHY_RXBCNTH_RXBCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_RECEIVE_BYTE_COUNT_LOW           : 0x1F0023 */
#define PHY_RXBCNTL_RXBCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_TRANSMIT_OPPORTUNITY_COUNT_HIGH  : 0x1F0024 */
#define PHY_TOCNTH_TOCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_TRANSMIT_OPPORTUNITY_COUNT_LOW   : 0x1F0025 */
#define PHY_TOCNTL_TOCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_BEACON_COUNT_HIGH                : 0x1F0026 */
#define PHY_BCNCNTH_BCNCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_BEACON_COUNT_LOW                 : 0x1F0027 */
#define PHY_BCNCNTL_BCNCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_TSTPATGENCTL                     : 0x1F0078 */
#define PHY_TSTPATGENCTL_PATGENSEL FIELD_DESCRIPTOR(0, 2)
#define PHY_TSTPATGENCTL_GENSEL    FIELD_DESCRIPTOR(2, 1)
#define PHY_TSTPATGENCTL_PPREINS   FIELD_DESCRIPTOR(3, 1)
#define PHY_TSTPATGENCTL_PATGENEN  FIELD_DESCRIPTOR(4, 1)

/* PHY_TSTPATFRM                        : 0x1F0079 */
#define PHY_TSTPATFRM_FRMSZ FIELD_DESCRIPTOR(0, 8)

/* PHY_TSTPCKCTL                        : 0x1F007A */
#define PHY_TSTPCKCTL_PATCHKSEL FIELD_DESCRIPTOR(0, 2)
#define PHY_TSTPCKCTL_DCODE5B   FIELD_DESCRIPTOR(2, 1)
#define PHY_TSTPCKCTL_CLRERRCNT FIELD_DESCRIPTOR(3, 1)
#define PHY_TSTPCKCTL_PATCHKEN  FIELD_DESCRIPTOR(4, 1)
#define PHY_TSTPCKCTL_SYNCN     FIELD_DESCRIPTOR(5, 6)

/* PHY_TSTERRCNT                        : 0x1F007B */
#define PHY_TSTERRCNT_CHKERRCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_TSTPATCKSTS                      : 0x1F007C */
#define PHY_TSTPATCKSTS_PATSYNC FIELD_DESCRIPTOR(0, 1)
#define PHY_TSTPATCKSTS_PFRERR  FIELD_DESCRIPTOR(1, 3)

/* PHY_PAD_CONTROL_3                    : 0x1F00CB */
#define PHY_PADCTRL3_PDRV1 FIELD_DESCRIPTOR(8, 2)
#define PHY_PADCTRL3_PDRV2 FIELD_DESCRIPTOR(10, 2)
#define PHY_PADCTRL3_PDRV3 FIELD_DESCRIPTOR(12, 2)
#define PHY_PADCTRL3_PDRV4 FIELD_DESCRIPTOR(14, 2)

/* PHY_PLCA_IDVER                       : 0x1FCA00 */
#define PHY_PLCA_IDVER_VER FIELD_DESCRIPTOR(0, 8)
#define PHY_PLCA_IDVER_IDM FIELD_DESCRIPTOR(8, 8)

/* PHY_PLCA_CONTROL_0                   : 0x1FCA01 */
#define PHY_PLCA_CTRL0_RST FIELD_DESCRIPTOR(14, 1)
#define PHY_PLCA_CTRL0_EN  FIELD_DESCRIPTOR(15, 1)

/* PHY_PLCA_CONTROL_1                   : 0x1FCA02 */
#define PHY_PLCA_CTRL1_ID0  FIELD_DESCRIPTOR(0, 8)
#define PHY_PLCA_CTRL1_NCNT FIELD_DESCRIPTOR(8, 8)

/* PHY_PLCA_STATUS                      : 0x1FCA03 */
#define PHY_PLCA_STS_PST FIELD_DESCRIPTOR(15, 1)

/* PHY_PLCA_TO_TIMER                    : 0x1FCA04 */
#define PHY_PLCA_TOTMR_TOTMR FIELD_DESCRIPTOR(0, 8)

/* PHY_PLCA_BURST                       : 0x1FCA05 */
#define PHY_PLCA_BURST_BTMR  FIELD_DESCRIPTOR(0, 7)
#define PHY_PLCA_BURST_MAXBC FIELD_DESCRIPTOR(8, 8)

/******************************************************************************
*  FUNCTION DECLARATIONS
******************************************************************************/

DRV_MIIM_RESULT Lan867x_Write_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t wData);
DRV_MIIM_RESULT Lan867x_Read_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t *rData);
DRV_MIIM_RESULT Lan867x_Write_Bit_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t mask, uint16_t wData);

#endif // LAN867x_H_