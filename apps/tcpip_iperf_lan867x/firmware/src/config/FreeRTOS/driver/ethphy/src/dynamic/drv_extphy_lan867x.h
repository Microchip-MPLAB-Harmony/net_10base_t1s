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
    PHY_MMDCTRL = 13,
    PHY_MMDAD = 14,
    PHY_STRAP_CONTROL0 = 18,

    /* Clause 45 registers. */
    /* PMA/PMD registers. */
    PHY_PMAPMDEXTA = 0x010012,
    PHY_T1PMAPMDCTL = 0x010834,
    PHY_T1SPMACTL = 0x0108F9,
    PHY_T1SPMASTS = 0x0108FA,
    PHY_T1STSTCTL = 0x0108FB,

    /* PCS registers. */
    PHY_T1SPCSCTL = 0x0308F3,
    PHY_T1SPCSSTS = 0x0308F4,
    PHY_T1SPCSDIAG1 = 0x0308F5,
    PHY_T1SPCSDIAG2 = 0x0308F6,

    /* Miscellaneous registers.*/
    PHY_CFGPRTCTL = 0x1F000F,
    PHY_CTRL1 = 0x1F0010,
    PHY_PINCTRL = 0x1F0011,
    PHY_STS1 = 0x1F0018,
    PHY_STS2 = 0x1F0019,
    PHY_STS3 = 0x1F001A,
    PHY_IMSK1 = 0x1F001C,
    PHY_IMSK2 = 0x1F001D,
    PHY_CTRCTRL = 0x1F0020,
    PHY_TOCNTH = 0x1F0024,
    PHY_TOCNTL = 0x1F0025,
    PHY_BCNCNTH = 0x1F0026,
    PHY_BCNCNTL = 0x1F0027,
    PHY_MULTID0 = 0x1F0030,
    PHY_MULTID1 = 0x1F0031,
    PHY_MULTID2 = 0x1F0032,
    PHY_MULTID3 = 0x1F0033,
    PHY_PRSSTS = 0x1F0036,
    PHY_PRTMGMT2 = 0x1F003D,
    PHY_IWDTOH = 0x1F003E,
    PHY_IWDTOL = 0x1F003F,
    PHY_TXMCTL = 0x1F0040,
    PHY_TXMPATH = 0x1F0041,
    PHY_TXMPATL = 0x1F0042,
    PHY_TXMDLY = 0x1F0049,
    PHY_RXMCTL = 0x1F0050,
    PHY_RXMPATH = 0x1F0051,
    PHY_RXMPATL = 0x1F0052,
    PHY_RXMDLY = 0x1F0059,
    PHY_CBSSPTHH = 0x1F0060,
    PHY_CBSSPTHL = 0x1F0061,
    PHY_CBSSTTHH = 0x1F0062,
    PHY_CBSSTTHL = 0x1F0063,
    PHY_CBSSLPCTL = 0x1F0064,
    PHY_CBSTPLMTH = 0x1F0065,
    PHY_CBSTPLMTL = 0x1F0066,
    PHY_CBSBTLMTH = 0x1F0067,
    PHY_CBSBTLMTL = 0x1F0068,
    PHY_CBSCRCTRH = 0x1F0069,
    PHY_CBSCRCTRL = 0x1F006A,
    PHY_CBSCTRL = 0x1F006B,
    PHY_PLCASKPCTL = 0x1F0070,
    PHY_PLCATOSKP = 0x01F0071,
    PHY_PLCACYCSKP = 0x01F0072,
    PHY_ACMACTL = 0x01F0074,
    PHY_SLPCTL0 = 0x1F0080,
    PHY_SLPCTL1 = 0x1F0081,
    PHY_CDCTL0 = 0x01F0087,
    PHY_SQICTL = 0x01F00A0,
    PHY_SQISTS0 = 0x01F00A1,
    PHY_SQICFG0 = 0x01F00AA,
    PHY_SQICFG2 = 0x01F00AC,
    PHY_PADCTRL3 = 0x01F00CB,
    PHY_ANALOG5 = 0x1F00D5,
    PHY_MIDVER = 0x1FCA00,
    PHY_PLCA_CTRL0 = 0x1FCA01,
    PHY_PLCA_CTRL1 = 0x1FCA02,
    PHY_PLCA_STS = 0x1FCA03,
    PHY_PLCA_TOTMR = 0x1FCA04,
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

/* PHY_REG_PHYID1                       : 2 */
#define PHY_PHY_ID1_OUI FIELD_DESCRIPTOR(0, 16)

/* PHY_REG_PHYID2                       : 3 */
#define PHY_PHY_ID2_REV   FIELD_DESCRIPTOR(0, 4)
#define PHY_PHY_ID2_MODEL FIELD_DESCRIPTOR(4, 6)
#define PHY_PHY_ID2_OUI   FIELD_DESCRIPTOR(10, 6)

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
#define PHY_CTRL1_BCAEN  FIELD_DESCRIPTOR(2, 1)
#define PHY_CTRL1_IWDE   FIELD_DESCRIPTOR(3, 1)

/* PHY_PINCTRL                         : 0x1F00011 */
#define PHY_PINCTRL_ACMAPOL FIELD_DESCRIPTOR(0, 2)
#define PHY_PINCTRL_RXPIPOL FIELD_DESCRIPTOR(4, 2)
#define PHY_PINCTRL_TXPIPOL FIELD_DESCRIPTOR(6, 2)
#define PHY_PINCTRL_GPIO0SS FIELD_DESCRIPTOR(14,2)

/* PHY_STATUS_1                         : 0x1F0018 */
#define PHY_STS1_DEC5B   FIELD_DESCRIPTOR(0, 1)
#define PHY_STS1_ESDERR  FIELD_DESCRIPTOR(1, 1)
#define PHY_STS1_PLCASYM FIELD_DESCRIPTOR(2, 1)
#define PHY_STS1_UNCRS   FIELD_DESCRIPTOR(3, 1)
#define PHY_STS1_BCNBFTO FIELD_DESCRIPTOR(4, 1)
#define PHY_STS1_UNEXPB  FIELD_DESCRIPTOR(5, 1)
#define PHY_STS1_RXINTO  FIELD_DESCRIPTOR(6, 1)
#define PHY_STS1_EMPCYC  FIELD_DESCRIPTOR(7, 1)
#define PHY_STS1_TSSI    FIELD_DESCRIPTOR(8, 1)
#define PHY_STS1_TXJAB   FIELD_DESCRIPTOR(9, 1)
#define PHY_STS1_TXCOL   FIELD_DESCRIPTOR(10, 1)
#define PHY_STS1_PSTC    FIELD_DESCRIPTOR(11, 1)
#define PHY_STS1_SQI     FIELD_DESCRIPTOR(12, 1)

/* PHY_STATUS_2                         : 0x1F0019 */
#define PHY_STS2_IWDTO   FIELD_DESCRIPTOR(5, 1)
#define PHY_STS2_OT      FIELD_DESCRIPTOR(6, 1)
#define PHY_STS2_UV33    FIELD_DESCRIPTOR(8, 1)
#define PHY_STS2_WKEWI   FIELD_DESCRIPTOR(9, 1)
#define PHY_STS2_WKEMDI  FIELD_DESCRIPTOR(10, 1)
#define PHY_STS2_RESETC  FIELD_DESCRIPTOR(11, 1)

/* PHY_STATUS_3                         : 0x1F001A */
#define PHY_STS3_ERRTOID FIELD_DESCRIPTOR(0, 8)

/* PHY_INTERRUPT_MASK_1                 : 0x1F001C */
#define PHY_IMSK1_DEC5BM   FIELD_DESCRIPTOR(0, 1)
#define PHY_IMSK1_ESDERRM  FIELD_DESCRIPTOR(1, 1)
#define PHY_IMSK1_PLCASYMM FIELD_DESCRIPTOR(2, 1)
#define PHY_IMSK1_UNCRSM   FIELD_DESCRIPTOR(3, 1)
#define PHY_IMSK1_BCNBFTOM FIELD_DESCRIPTOR(4, 1)
#define PHY_IMSK1_UNEXPBM  FIELD_DESCRIPTOR(5, 1)
#define PHY_IMSK1_RXINTOM  FIELD_DESCRIPTOR(6, 1)
#define PHY_IMSK1_EMPCYCM  FIELD_DESCRIPTOR(7, 1)
#define PHY_IMSK1_TSSIM    FIELD_DESCRIPTOR(8, 1)
#define PHY_IMSK1_TXBABLM  FIELD_DESCRIPTOR(9, 1)
#define PHY_IMSK1_TXCOLM   FIELD_DESCRIPTOR(10, 1)
#define PHY_IMSK1_PSTCM    FIELD_DESCRIPTOR(11, 1)
#define PHY_IMSK1_SQIM     FIELD_DESCRIPTOR(12, 1)

/* PHY_INTERRUPT_MASK_2                 : 0x1F001D */
#define PHY_IMSK2_IWDTOM   FIELD_DESCRIPTOR(5, 1)
#define PHY_IMSK2_OTM      FIELD_DESCRIPTOR(6, 1)
#define PHY_IMSK2_UV33M    FIELD_DESCRIPTOR(8, 1)
#define PHY_IMSK2_WKEWIM   FIELD_DESCRIPTOR(9, 1)
#define PHY_IMSK2_WKEMDIM  FIELD_DESCRIPTOR(10, 1)
#define PHY_IMSK2_RESETCM  FIELD_DESCRIPTOR(11, 1)

/* PHY_CTRCTRL                          : 0x1F0020 */
#define PHY_CTRCTRL_BCNCTRE FIELD_DESCRIPTOR(0, 1)
#define PHY_CTRCTRL_TOCTRE  FIELD_DESCRIPTOR(1, 1)

/* PHY_TRANSMIT_OPPORTUNITY_COUNT_HIGH  : 0x1F0024 */
#define PHY_TOCNTH_TOCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_TRANSMIT_OPPORTUNITY_COUNT_LOW   : 0x1F0025 */
#define PHY_TOCNTL_TOCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_BEACON_COUNT_HIGH                : 0x1F0026 */
#define PHY_BCNCNTH_BCNCNT FIELD_DESCRIPTOR(0, 16)

/* PHY_BEACON_COUNT_LOW                 : 0x1F0027 */
#define PHY_BCNCNTL_BCNCNT FIELD_DESCRIPTOR(0, 16)

/*PHY_MULTID0                           : 0x1F0030 */
#define PHY_MULTID0_ID2 FIELD_DESCRIPTOR(0, 8)
#define PHY_MULTID0_ID1 FIELD_DESCRIPTOR(8, 8)

/*PHY_MULTID1                           : 0x1F0031 */
#define PHY_MULTID1_ID4 FIELD_DESCRIPTOR(0, 8)
#define PHY_MULTID1_ID3 FIELD_DESCRIPTOR(8, 8)

/*PHY_MULTID2                           : 0x1F0032 */
#define PHY_MULTID2_ID6 FIELD_DESCRIPTOR(0, 8)
#define PHY_MULTID2_ID5 FIELD_DESCRIPTOR(8, 8)

/*PHY_MULTID3                           : 0x1F0033 */
#define PHY_MULTID3_ID8 FIELD_DESCRIPTOR(0, 8)
#define PHY_MULTID3_ID7 FIELD_DESCRIPTOR(8, 8)

/*PHY_PRSSTS                            : 0x1F0036 */
#define PHY_PRSSTS_MAXID FIELD_DESCRIPTOR(8, 8)

/*PHY_PRTMGMT2                          : 0x1F003D */
#define PHY_PRTMGMT2_MITXWDEN FIELD_DESCRIPTOR(11, 1)
#define PHY_PRTMGMT2_PRIWDEN  FIELD_DESCRIPTOR(12, 1)
#define PHY_PRTMGMT2_MIRXWDEN FIELD_DESCRIPTOR(13, 1)

/*PHY_IWDTOH                            : 0x1F003E */
#define PHY_IWDTOH_TIMEOUT FIELD_DESCRIPTOR(0, 16)

/*PHY_IWDTOL                            : 0x1F003F */
#define PHY_IWDTOL_TIMEOUT FIELD_DESCRIPTOR(0, 16)

/*PHY_TXMCTL                            : 0x1F0040 */
#define PHY_TXMCTL_TXME     FIELD_DESCRIPTOR(1, 1)
#define PHY_TXMCTL_MACTXTSE FIELD_DESCRIPTOR(2, 1)
#define PHY_TXMCTL_TXPMDET  FIELD_DESCRIPTOR(7, 1)

/*PHY_TXMPATH                           : 0x1F0041 */
#define PHY_TXMPATH_TXMPAT  FIELD_DESCRIPTOR(0, 8)

/*PHY_TXMPATL                           : 0x1F0042 */
#define PHY_TXMPATL_TXMPAT  FIELD_DESCRIPTOR(0, 16)

/*PHY_TXMDLY                            : 0x1F0049 */
#define PHY_TXMDLY_TXMPKTDLY FIELD_DESCRIPTOR(0, 10)
#define PHY_TXMDLY_TXMDLYEN  FIELD_DESCRIPTOR(15, 1)

/*PHY_RXMCTL                            : 0x1F0050 */
#define PHY_RXMCTL_RXME     FIELD_DESCRIPTOR(1, 1)
#define PHY_RXMCTL_RXPMDET  FIELD_DESCRIPTOR(6, 1)

/*PHY_RXMPATH                           : 0x1F0051 */
#define PHY_RXMPATH_RXMPAT  FIELD_DESCRIPTOR(0, 8)

/*PHY_RXMPATL                           : 0x1F0052 */
#define PHY_RXMPATL_RXMPAT  FIELD_DESCRIPTOR(0, 16)

/*PHY_RXMDLY                            : 0x1F0059 */
#define PHY_RXMDLY_RXMPKTDLY FIELD_DESCRIPTOR(0, 10)
#define PHY_RXMDLY_RXMDLYEN  FIELD_DESCRIPTOR(15, 1)

/*PHY_CBSSPTHH                          : 0x1F0060 */
#define PHY_CBSSPTHH_STOPTHR FIELD_DESCRIPTOR(0, 4)

/*PHY_CBSSPTHL                          : 0x1F0061 */
#define PHY_CBSSPTHL_STOPTHR FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSSTTHH                          : 0x1F0062 */
#define PHY_CBSSTTHH_STARTTHR FIELD_DESCRIPTOR(0, 4)

/*PHY_CBSSTTHL                          : 0x1F0063 */
#define PHY_CBSSTTHL_STARTTHR FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSSLPCTL                          : 0x1F0064 */
#define PHY_CBSSLPCTL_RISESLP FIELD_DESCRIPTOR(0, 8)
#define PHY_CBSSLPCTL_FALLSLP FIELD_DESCRIPTOR(8, 8)

/*PHY_CBSTPLMTH                          : 0x1F0065 */
#define PHY_CBSTPLMTH_TOPLIMIT FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSTPLMTL                         : 0x1F0066 */
#define PHY_CBSSPTHH_TOPLIMIT  FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSBTLMTH                         : 0x1F0067 */
#define PHY_CBSBTLMTH_BOTLIMIT FIELD_DESCRIPTOR(0, 4)

/*PHY_CBSBTLMTL                         : 0x1F0068 */
#define PHY_CBSBTLMTL_BOTLIMIT FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSCRCTRH                         : 0x1F0069 */
#define PHY_CBSCRCTRH_CREDITCTR FIELD_DESCRIPTOR(0, 4)

/*PHY_CBSCRCTRL                         : 0x1F006A */
#define PHY_CBSCRCTRL_CREDITCTR FIELD_DESCRIPTOR(0, 16)

/*PHY_CBSCTRL                           : 0x1F006B */
#define PHY_CBSCTRL_CBSEN  FIELD_DESCRIPTOR(0, 1)
#define PHY_CBSCTRL_ECCRDS FIELD_DESCRIPTOR(1, 8)

/*PHY_PLCASKPCTL                        : 0x1F0070 */
#define PHY_PLCASKPCTL_TOSKPEN FIELD_DESCRIPTOR(1, 1)

/*PHY_PLCATOSKP                         : 0x1F0071 */
#define PHY_PLCATOSKP_TOSKPNUM  FIELD_DESCRIPTOR(0, 8)

/*PHY_PLCACYCSKP                        : 0x1F0072 */
#define PHY_PLCACYCSKP_CYCSKPNUM FIELD_DESCRIPTOR(0, 8)

/*PHY_ACMACTL                           : 0x1F0074 */
#define PHY_ACMACTL_ACMAEN    FIELD_DESCRIPTOR(0, 1)

/*PHY_SLPCTL0                           : 0x1F0080 */
#define PHY_SLPCTL0_SLPCAL    FIELD_DESCRIPTOR(3, 4)
#define PHY_SLPCTL0_SLPINHDLY FIELD_DESCRIPTOR(11, 2)
#define PHY_SLPCTL0_MDIWKEN   FIELD_DESCRIPTOR(13, 1)
#define PHY_SLPCTL0_WKINEN    FIELD_DESCRIPTOR(14, 1)
#define PHY_SLPCTL0_SLPEN     FIELD_DESCRIPTOR(15, 1)

/*PHY_SLPCTL1                           : 0x1F0081 */
#define PHY_SLPCTL1_MDIFWDEN FIELD_DESCRIPTOR(0, 1)
#define PHY_SLPCTL1_WKOFWDEN FIELD_DESCRIPTOR(1, 1)
#define PHY_SLPCTL1_MWKFWD   FIELD_DESCRIPTOR(2, 1)
#define PHY_SLPCTL1_CLRWKI   FIELD_DESCRIPTOR(3, 1)
#define PHY_SLPCTL1_WAKEIND  FIELD_DESCRIPTOR(4, 1)
#define PHY_SLPCTL1_WIPOL    FIELD_DESCRIPTOR(5, 1)
#define PHY_SLPCTL1_WOPOL    FIELD_DESCRIPTOR(7, 1)

/*PHY_CDCTL0                            : 0x1F0087 */
#define PHY_CDCTL0_CDEN      FIELD_DESCRIPTOR(15, 1)

/*PHY_SQICTL                            : 0x1F00A0 */
#define PHY_SQICTL_SQIEN      FIELD_DESCRIPTOR(14, 1)
#define PHY_SQICTL_SQIRST     FIELD_DESCRIPTOR(15, 1)

/*PHY_SQISTS0                           : 0x1F00A1 */
#define PHY_SQISTS0_SQIERRC   FIELD_DESCRIPTOR(0, 3)
#define PHY_SQISTS0_SQIVAL    FIELD_DESCRIPTOR(3, 3)
#define PHY_SQISTS0_SQIVLD    FIELD_DESCRIPTOR(6, 1)
#define PHY_SQISTS0_SQIERR    FIELD_DESCRIPTOR(7, 1)

/*PHY_SQICFG0                           : 0x1F00AA */
#define PHY_SQICFG0_TOID      FIELD_DESCRIPTOR(4, 8)

/*PHY_SQICFG2                           : 0x1F00AC */
#define PHY_SQICFG2_SQIINTTHR FIELD_DESCRIPTOR(8, 5)

/*PHY_PADCTRL3                          : 0x1F00CB */
#define PHY_PADCTRL3_PDRV1   FIELD_DESCRIPTOR(8, 2)
#define PHY_PADCTRL3_PDRV2   FIELD_DESCRIPTOR(10, 2)
#define PHY_PADCTRL3_PDRV3   FIELD_DESCRIPTOR(12, 2)
#define PHY_PADCTRL3_PDRV4   FIELD_DESCRIPTOR(14, 2)

/*PHY_ANALOG5                           : 0x1F00D5 */
#define PHY_ANALOG5_UV33FTM  FIELD_DESCRIPTOR(8, 8)

/* PHY_MIDVER                           : 0x1FCA00 */
#define PHY_MIDVER_VER FIELD_DESCRIPTOR(0, 8)
#define PHY_MIDVER_IDM FIELD_DESCRIPTOR(8, 8)

/* PHY_PLCA_CONTROL_0                   : 0x1FCA01 */
#define PHY_PLCA_CTRL0_RST FIELD_DESCRIPTOR(14, 1)
#define PHY_PLCA_CTRL0_EN  FIELD_DESCRIPTOR(15, 1)

/* PHY_PLCA_CONTROL_1                   : 0x1FCA02 */
#define PHY_PLCA_CTRL1_ID   FIELD_DESCRIPTOR(0, 8)
#define PHY_PLCA_CTRL1_NCNT FIELD_DESCRIPTOR(8, 8)

/* PHY_PLCA_STATUS                      : 0x1FCA03 */
#define PHY_PLCA_STS_PST FIELD_DESCRIPTOR(15, 1)

/* PHY_PLCA_TO_TIMER                    : 0x1FCA04 */
#define PHY_PLCA_TOTMR_TOTMR FIELD_DESCRIPTOR(0, 8)

/* PHY_PLCA_BURST                       : 0x1FCA05 */
#define PHY_PLCA_BURST_BTMR  FIELD_DESCRIPTOR(0, 7)
#define PHY_PLCA_BURST_MAXBC FIELD_DESCRIPTOR(8, 8)

#define CHIP_HEALTH   FIELD_DESCRIPTOR(6, 1)
#define OFFSET1       FIELD_DESCRIPTOR(0, 5)
#define OFFSET2       FIELD_DESCRIPTOR(0, 5)
#define PHY_TYPE      FIELD_DESCRIPTOR(7, 2)

#define LAN867x_PHY_ID2_MASK  (0xC16F)
#define LAN867x_PHY_ID_REV_B1 (0x2)
#define LAN867x_PHY_ID_REV_C1 (0x4)
#define LAN867x_PHY_ID_REV_C2 (0x5)
/******************************************************************************
*  FUNCTION DECLARATIONS
******************************************************************************/

DRV_MIIM_RESULT Lan867x_Write_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t wData);
DRV_MIIM_RESULT Lan867x_Read_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t *rData);
DRV_MIIM_RESULT Lan867x_Write_Bit_Register(LAN867X_REG_OBJ *clientObj, const uint32_t regAddr, uint16_t mask, uint16_t wData);

#endif // LAN867x_H_