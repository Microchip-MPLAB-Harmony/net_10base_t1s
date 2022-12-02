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

#include "drv_lan865x_regs.h"

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*                          AUTO GENERATED DEFINES                      */
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

const MemoryMap_t TC6_MEMMAP[] = {
    {  .address=0x00000004, .value=0x00000026,  .mask=0x00000000,  .op=MemOp_Write,            .secure=false },
    {  .address=0x00010000, .value=0x0000000C,  .mask=0x0000000C,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x0000000C, .value=0x00000000,  .mask=0x00001FFF,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x0000000D, .value=0x00000000,  .mask=0x3FFE0003,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x00040087, .value=0x00000000,  .mask=0x00008000,  .op=MemOp_ReadModifyWrite,  .secure=true  },  /* Disable Collision Detection */
    {  .address=0x00040091, .value=0x00000060,  .mask=0x00000060,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x00040084, .value=0x000024E0,  .mask=0x0000FFF0,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400D0, .value=0x00004000,  .mask=0x0000E00C,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x0004008A, .value=0x0000A000,  .mask=0x0000FC00,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400E9, .value=0x00001E00,  .mask=0x00001E00,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400F5, .value=0x00001000,  .mask=0x00003000,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x00040080, .value=0x00000600,  .mask=0x00000600,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x00040099, .value=0x00000015,  .mask=0x0000003F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x00040098, .value=0x000000C3,  .mask=0x00000FFF,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400F4, .value=0x00000020,  .mask=0x00000020,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400F8, .value=0x00000000,  .mask=0x00002000,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400F9, .value=0x00004C13,  .mask=0x00007C1F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400AD, .value=0x00000509,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400AE, .value=0x0000090E,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400AF, .value=0x00001116,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B0, .value=0x00000103,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B1, .value=0x00000910,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B2, .value=0x00001D26,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B3, .value=0x0000002A,  .mask=0x0000003F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B4, .value=0x00000103,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B5, .value=0x0000070D,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B6, .value=0x00001720,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B7, .value=0x00000027,  .mask=0x0000003F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B8, .value=0x00000509,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400B9, .value=0x00000E13,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400BA, .value=0x00001C25,  .mask=0x00003F3F,  .op=MemOp_ReadModifyWrite,  .secure=true  },
    {  .address=0x000400BB, .value=0x0000002B,  .mask=0x0000003F,  .op=MemOp_ReadModifyWrite,  .secure=true  }
};

const uint32_t TC6_MEMMAP_LENGTH = (sizeof(TC6_MEMMAP) / sizeof(MemoryMap_t));
