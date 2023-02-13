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
    drv_lan865x_regs.c

  Summary:
    Initial default register settings

  Description:
    This file contains a list of register address-value pairs for the initialization of LAN865x
*******************************************************************************/

#include "drv_lan865x_regs.h"

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*                          AUTO GENERATED DEFINES                      */
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

const MemoryMap_t TC6_MEMMAP[] = {
    {  .address=0x00000004,  .value=0x00000026,  .mask=0x00000000,  .op=MemOp_Write,            .secure=false }, /* CONFIG0 */
    {  .address=0x00040091,  .value=0x00009660,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040081,  .value=0x000000C0,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00010077,  .value=0x00000028,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040043,  .value=0x000000FF,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040044,  .value=0x0000FFFF,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040045,  .value=0x00000000,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040053,  .value=0x000000FF,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040054,  .value=0x0000FFFF,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040055,  .value=0x00000000,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040040,  .value=0x00000002,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x00040050,  .value=0x00000002,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400D0,  .value=0x00005F21,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400E9,  .value=0x00009E50,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400F5,  .value=0x00001CF8,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400F4,  .value=0x0000C020,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400F8,  .value=0x00009B00,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400F9,  .value=0x00004E53,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B0,  .value=0x00000103,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B1,  .value=0x00000910,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B2,  .value=0x00001D26,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B3,  .value=0x0000002A,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B4,  .value=0x00000103,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B5,  .value=0x0000070D,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B6,  .value=0x00001720,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B7,  .value=0x00000027,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B8,  .value=0x00000509,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400B9,  .value=0x00000E13,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400BA,  .value=0x00001C25,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },
    {  .address=0x000400BB,  .value=0x0000002B,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  },

    {  .address=0x00040087,  .value=0x00000083,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  }, /* COL_DET_CTRL0 */
    {  .address=0x0000000C,  .value=0x00000100,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  }, /* IMASK0 */
    {  .address=0x00040081,  .value=0x000000E0,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  }, /* DEEP_SLEEP_CTRL_1 */
    {  .address=0x00010000,  .value=0x0000000C,  .mask=0x00000000,  .op=MemOp_Write,            .secure=true  }, /* NETWORK_CONTROL */
};

const uint32_t TC6_MEMMAP_LENGTH = (sizeof(TC6_MEMMAP) / sizeof(MemoryMap_t));
