<#--
/*******************************************************************************
  LAN865X Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan865x.c.ftl

  Summary:
    LAN865X Driver Freemarker Template File

  Description:

*******************************************************************************/
-->

<#----------------------------------------------------------------------------
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
----------------------------------------------------------------------------->

// <editor-fold defaultstate="collapsed" desc="LAN865X Driver Initialization Data">

<#if (DRV_LAN865X_USE_DRIVER)?has_content && (DRV_LAN865X_USE_DRIVER == true)>
/* LAN865X Driver Configuration */
const DRV_LAN865X_Configuration drvLan865xInitData[] = {
<#list 0..(__INSTANCE_COUNT?number-1) as idx>
{
    .spiDrvIndex =          DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX${idx},
    .spiChipSelectPin =     DRV_LAN865X_SPI_CS_IDX${idx},
    .interruptPin =         DRV_LAN865X_INTERRUPT_PIN_IDX${idx},
    .resetPin =             DRV_LAN865X_RESET_PIN_IDX${idx},
    .spiFrequecny =         DRV_LAN865X_SPI_FREQ_IDX${idx},
    .rxDescriptors =        DRV_LAN865X_MAC_RX_DESCRIPTORS_IDX${idx},
    .rxDescBufferSize =     DRV_LAN865X_MAX_RX_BUFFER_IDX${idx},
    .chunkSize =            DRV_LAN865X_CHUNK_SIZE_IDX${idx},
    .nodeId =               DRV_LAN865X_PLCA_NODE_ID_IDX${idx},
    .nodeCount =            DRV_LAN865X_PLCA_NODE_COUNT_IDX${idx},
    .burstCount =           DRV_LAN865X_PLCA_BURST_COUNT_IDX${idx},
    .burstTimer =           DRV_LAN865X_PLCA_BURST_TIMER_IDX${idx},
    .plcaEnable =           DRV_LAN865X_PLCA_ENABLE_IDX${idx},
    .promiscuous =          DRV_LAN865X_PROMISCUOUS_IDX${idx},
    .txCutThrough =         DRV_LAN865X_TX_CUT_THROUGH_IDX${idx},
    .rxCutThrough =         DRV_LAN865X_RX_CUT_THROUGH_IDX${idx},
},
</#list>
};

</#if>
// </editor-fold>

