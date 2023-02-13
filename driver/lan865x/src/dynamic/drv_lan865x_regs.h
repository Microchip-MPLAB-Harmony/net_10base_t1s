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
  Default register settings for LAN865x

  Company:
    Microchip Technology Inc.
    
  File Name:
    drv_lan865x_regs.h

  Summary:
    Initial default register settings

  Description:
    This file contains a list of register address-value pairs for the initialization of LAN865x
*******************************************************************************/

#ifndef TC6_REGS_H_
#define TC6_REGS_H_

#include <stdint.h>
#include "tc6/tc6.h"

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*                          AUTO GENERATED DEFINES                      */
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#define MEMMAP_LEN  (34u)  /* Overcome MISRA issue 8.11 */

extern const uint32_t TC6_MEMMAP_LENGTH;

extern const MemoryMap_t TC6_MEMMAP[MEMMAP_LEN];

#endif /* TC6_REGS_H_ */
