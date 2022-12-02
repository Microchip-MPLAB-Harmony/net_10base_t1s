<#--
/*******************************************************************************
  LAN865X Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan865x.h.ftl

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

<#if (DRV_LAN865X_USE_DRIVER)?has_content && (DRV_LAN865X_USE_DRIVER == true)>
/*** LAN865X Driver Configuration ***/
/*** Driver Compilation and static configuration options. ***/
#define TCPIP_IF_LAN865X

#define DRV_LAN865X_INSTANCES_NUMBER         ${__INSTANCE_COUNT}

<#list 0..(__INSTANCE_COUNT?number-1) as idx>
	<#assign spiDrvInst = "drvExtMacLan865x_"+idx+".DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX" + idx>
	<#lt>#define DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX${idx} ${spiDrvInst?eval}
	<#assign clientInstnNum = "drvExtMacLan865x_"+idx+".DRV_LAN865X_CLIENT_INSTANCES_IDX" + idx>
	<#lt>#define DRV_LAN865X_CLIENT_INSTANCES_IDX${idx}    ${clientInstnNum?eval}
	<#assign spiFreq = "drvExtMacLan865x_"+idx+".DRV_LAN865X_SPI_FREQ_IDX" + idx>
	<#lt>#define DRV_LAN865X_SPI_FREQ_IDX${idx}            ${spiFreq?eval}
	<#assign rxDescNum = "drvExtMacLan865x_"+idx+".DRV_LAN865X_MAC_RX_DESCRIPTORS_IDX" + idx>
	<#lt>#define DRV_LAN865X_MAC_RX_DESCRIPTORS_IDX${idx}  ${rxDescNum?eval}
	<#assign rxDescBuffSize= "drvExtMacLan865x_"+idx+".DRV_LAN865X_MAX_RX_BUFFER_IDX" + idx>
	<#lt>#define DRV_LAN865X_MAX_RX_BUFFER_IDX${idx}       ${rxDescBuffSize?eval}
	<#assign chipSelectPin= "drvExtMacLan865x_"+idx+".DRV_LAN865X_SPI_CS_IDX" + idx>
	<#if (chipSelectPin?eval == "SYS_PORT_PIN_NONE")>
	    <#lt>#error Configure SPI Chip-Select pin for interfacing LAN865X (in LAN865X Driver)
	<#else>
		<#lt>#define DRV_LAN865X_SPI_CS_IDX${idx}              ${chipSelectPin?eval}
	</#if>
	<#assign interruptPin= "drvExtMacLan865x_"+idx+".DRV_LAN865X_INTERRUPT_PIN_IDX" + idx>
	<#if (interruptPin?eval == "SYS_PORT_PIN_NONE")>
	    <#lt>#error Configure Interrupt pin for interfacing LAN865X (in LAN865X Driver)
	<#else>
		<#lt>#define DRV_LAN865X_INTERRUPT_PIN_IDX${idx}       ${interruptPin?eval}
	</#if>
	<#assign resetPin= "drvExtMacLan865x_"+idx+".DRV_LAN865X_RESET_PIN_IDX" + idx>
	<#lt>#define DRV_LAN865X_RESET_PIN_IDX${idx}           ${resetPin?eval}
	<#assign promiscuous= "drvExtMacLan865x_"+idx+".DRV_LAN865X_PROMISCUOUS_IDX" + idx>
	<#if (promiscuous?eval)>
		<#lt>#define DRV_LAN865X_PROMISCUOUS_IDX${idx}         true
	<#else>
		<#lt>#define DRV_LAN865X_PROMISCUOUS_IDX${idx}         false
	</#if>
	<#assign txCutThrough= "drvExtMacLan865x_"+idx+".DRV_LAN865X_TX_CUT_THROUGH_IDX" + idx>
	<#if (txCutThrough?eval)>
		<#lt>#define DRV_LAN865X_TX_CUT_THROUGH_IDX${idx}      true
	<#else>
		<#lt>#define DRV_LAN865X_TX_CUT_THROUGH_IDX${idx}      false
	</#if>
	<#assign rxCutThrough= "drvExtMacLan865x_"+idx+".DRV_LAN865X_RX_CUT_THROUGH_IDX" + idx>
	<#if (rxCutThrough?eval)>
		<#lt>#define DRV_LAN865X_RX_CUT_THROUGH_IDX${idx}      true
	<#else>
		<#lt>#define DRV_LAN865X_RX_CUT_THROUGH_IDX${idx}      false
	</#if>	
	<#assign chunkSize= "drvExtMacLan865x_"+idx+".DRV_LAN865X_CHUNK_SIZE_IDX" + idx>
	<#if (chunkSize?eval == "64")>
		<#lt>#define DRV_LAN865X_CHUNK_SIZE_IDX${idx}          64
	<#elseif (chunkSize?eval == "32") >
		<#lt>#define DRV_LAN865X_CHUNK_SIZE_IDX${idx}          32
	</#if>
	<#assign chunkXact = "drvExtMacLan865x_"+idx+".DRV_LAN865X_CHUNK_XACT_IDX" + idx>
	<#lt>#define DRV_LAN865X_CHUNK_XACT_IDX${idx}          ${chunkXact?eval}
	<#assign plcaEnabled= "drvExtMacLan865x_"+idx+".DRV_LAN865X_10BASE_T1S_MODE_IDX" + idx>
	<#if (plcaEnabled?eval == "PLCA")>
		<#lt>#define DRV_LAN865X_PLCA_ENABLE_IDX${idx}         true
		<#assign plcaNodeId= "drvExtMacLan865x_"+idx+".DRV_LAN865X_PLCA_NODE_ID_IDX" + idx>
		<#lt>#define DRV_LAN865X_PLCA_NODE_ID_IDX${idx}        ${plcaNodeId?eval}
		<#assign plcaNodeCount= "drvExtMacLan865x_"+idx+".DRV_LAN865X_PLCA_NODE_COUNT_IDX" + idx>
		<#lt>#define DRV_LAN865X_PLCA_NODE_COUNT_IDX${idx}     ${plcaNodeCount?eval}
		<#assign plcaBurstCount= "drvExtMacLan865x_"+idx+".DRV_LAN865X_PLCA_BURST_COUNT_IDX" + idx>
		<#lt>#define DRV_LAN865X_PLCA_BURST_COUNT_IDX${idx}    ${plcaBurstCount?eval}
		<#assign plcaBurstTimer= "drvExtMacLan865x_"+idx+".DRV_LAN865X_PLCA_BURST_TIMER_IDX" + idx>
		<#lt>#define DRV_LAN865X_PLCA_BURST_TIMER_IDX${idx}    ${plcaBurstTimer?eval}
	<#else>
		<#lt>#define DRV_LAN865X_PLCA_ENABLE_IDX${idx}         false
		<#lt>#define DRV_LAN865X_PLCA_NODE_ID_IDX${idx}        0
		<#lt>#define DRV_LAN865X_PLCA_NODE_COUNT_IDX${idx}     0
		<#lt>#define DRV_LAN865X_PLCA_BURST_COUNT_IDX${idx}    0
		<#lt>#define DRV_LAN865X_PLCA_BURST_TIMER_IDX${idx}    0
	</#if>

</#list>
</#if>

