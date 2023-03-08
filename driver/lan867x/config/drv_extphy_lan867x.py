"""*****************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

phyName = "LAN867x"
def instantiateComponent(drvExtPhyLan867xComponent):
    print("LAN867x PHY Driver Component")
    configName = Variables.get("__CONFIGURATION_NAME")
    
    # PHY Address
    drvExtPhyLan867xAddr = drvExtPhyLan867xComponent.createIntegerSymbol("TCPIP_INTMAC_PHY_ADDRESS", None)
    drvExtPhyLan867xAddr.setLabel("PHY Address")
    drvExtPhyLan867xAddr.setVisible(True)
    drvExtPhyLan867xAddr.setDescription("PHY Address")
    drvExtPhyLan867xAddr.setDefaultValue(0)
    
    # Use a Function to be called at PHY Reset
    drvExtPhyLan867xResetCallbackEnable = drvExtPhyLan867xComponent.createBooleanSymbol("DRV_ETHPHY_USE_RESET_CALLBACK", None)
    drvExtPhyLan867xResetCallbackEnable.setLabel("Use a Function to be called at PHY Reset")
    drvExtPhyLan867xResetCallbackEnable.setVisible(True)
    drvExtPhyLan867xResetCallbackEnable.setDescription("Use a Function to be called at PHY Reset")
    drvExtPhyLan867xResetCallbackEnable.setDefaultValue(False)
    
    # App Function
    drvExtPhyLan867xResetCallback = drvExtPhyLan867xComponent.createStringSymbol("DRV_ETHPHY_RESET_CALLBACK", drvExtPhyLan867xResetCallbackEnable)
    drvExtPhyLan867xResetCallback.setLabel("App Function")
    drvExtPhyLan867xResetCallback.setVisible(False)
    drvExtPhyLan867xResetCallback.setDescription("App Function")
    drvExtPhyLan867xResetCallback.setDefaultValue("App" + phyName + "ResetFunction")
    drvExtPhyLan867xResetCallback.setDependencies(drvExtPhyLan867xMenuVisibleSingle, ["DRV_ETHPHY_USE_RESET_CALLBACK"])
    
    # External PHY Connection Flags
    drvExtPhyLan867xConnFlag = drvExtPhyLan867xComponent.createMenuSymbol(None, None)
    drvExtPhyLan867xConnFlag.setLabel("External PHY Connection Flags")
    drvExtPhyLan867xConnFlag.setVisible(True)
    drvExtPhyLan867xConnFlag.setDescription("External PHY Connection Flags")
    
    # RMII Data Interface
    drvExtPhyLan867xConfigRmii = drvExtPhyLan867xComponent.createBooleanSymbol("TCPIP_INTMAC_PHY_CONFIG_RMII", drvExtPhyLan867xConnFlag)
    drvExtPhyLan867xConfigRmii.setLabel("RMII Data Interface")
    drvExtPhyLan867xConfigRmii.setVisible(True)
    drvExtPhyLan867xConfigRmii.setDescription("RMII Data Interface")

    if Peripheral.moduleExists("GMAC"):
        drvExtPhyLan867xConfigRmii.setDefaultValue(True)
    elif "PIC32M" in Variables.get("__PROCESSOR"):
        drvExtPhyLan867xConfigRmii.setDefaultValue(False)
    
        # Configuration Fuses Is ALT
        drvExtPhyLan867xConfigAlt = drvExtPhyLan867xComponent.createBooleanSymbol("TCPIP_INTMAC_PHY_CONFIG_ALTERNATE", drvExtPhyLan867xConnFlag)
        drvExtPhyLan867xConfigAlt.setLabel("Configuration Fuses Is ALT")
        drvExtPhyLan867xConfigAlt.setVisible(True)
        drvExtPhyLan867xConfigAlt.setDescription("Configuration Fuses Is ALT")
        drvExtPhyLan867xConfigAlt.setDefaultValue(False)
        
        # Use The Fuses Configuration
        drvExtPhyLan867xConfigAuto = drvExtPhyLan867xComponent.createBooleanSymbol("TCPIP_INTMAC_PHY_CONFIG_AUTO", drvExtPhyLan867xConnFlag)
        drvExtPhyLan867xConfigAuto.setLabel("Use The Fuses Configuration")
        drvExtPhyLan867xConfigAuto.setVisible(True)
        drvExtPhyLan867xConfigAuto.setDescription("Use The Fuses Configuration")
        drvExtPhyLan867xConfigAuto.setDefaultValue(True)
    
    # Advanced Settings
    drvExtPhyLan867xAdvSettings = drvExtPhyLan867xComponent.createMenuSymbol("TCPIP_INTMAC_PHY_ADV_SETTING", None)
    drvExtPhyLan867xAdvSettings.setLabel("Advanced Settings")
    drvExtPhyLan867xAdvSettings.setDescription("Advanced Settings")
    drvExtPhyLan867xAdvSettings.setVisible(True)

    # Delay for the Link Initialization in ms
    drvExtPhyLan867xLinkInitDelay = drvExtPhyLan867xComponent.createIntegerSymbol("TCPIP_INTMAC_PHY_LINK_INIT_DELAY", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xLinkInitDelay.setLabel("Delay for the Link Initialization - ms")
    drvExtPhyLan867xLinkInitDelay.setVisible(True)
    drvExtPhyLan867xLinkInitDelay.setDescription("Delay for the Link Initialization in ms")
    drvExtPhyLan867xLinkInitDelay.setDefaultValue(500)
    
    # External PHY Type
    drvExtPhyLan867xPhyType = drvExtPhyLan867xComponent.createStringSymbol("TCPIP_EMAC_PHY_TYPE", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xPhyType.setVisible(False)
    drvExtPhyLan867xPhyType.setDefaultValue(phyName)
    
    # Driver PHY Reset Clear Time-out (mSec)
    drvExtPhyLan867xResetClearTimeout = drvExtPhyLan867xComponent.createIntegerSymbol("DRV_ETHPHY_RESET_CLR_TMO", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xResetClearTimeout.setLabel("PHY Reset Clear Time-out - ms")
    drvExtPhyLan867xResetClearTimeout.setVisible(True)
    drvExtPhyLan867xResetClearTimeout.setDescription("Driver PHY Reset Clear Time-out - ms")
    drvExtPhyLan867xResetClearTimeout.setDefaultValue(500)
    
    # Driver PHY Instances Number
    drvExtPhyLan867xInstanceNum = drvExtPhyLan867xComponent.createIntegerSymbol("DRV_ETHPHY_INSTANCES_NUMBER", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xInstanceNum.setLabel("PHY Instances Number")
    drvExtPhyLan867xInstanceNum.setVisible(True)
    drvExtPhyLan867xInstanceNum.setDescription("Driver PHY Instances Number")
    drvExtPhyLan867xInstanceNum.setDefaultValue(1)
    drvExtPhyLan867xInstanceNum.setReadOnly(True)
    
    # Driver PHY Clients Number
    drvExtPhyLan867xClientNum = drvExtPhyLan867xComponent.createIntegerSymbol("DRV_ETHPHY_CLIENTS_NUMBER", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xClientNum.setLabel("PHY Clients Number")
    drvExtPhyLan867xClientNum.setVisible(True)
    drvExtPhyLan867xClientNum.setDescription("Driver PHY Clients Number")
    drvExtPhyLan867xClientNum.setDefaultValue(1)
    drvExtPhyLan867xClientNum.setReadOnly(True)
    
    # Driver PHY Peripheral Index Number
    drvExtPhyLan867xIndexNum = drvExtPhyLan867xComponent.createIntegerSymbol("DRV_ETHPHY_INDEX", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xIndexNum.setLabel("PHY Peripheral Index Number")
    drvExtPhyLan867xIndexNum.setVisible(True)
    drvExtPhyLan867xIndexNum.setDescription("Driver PHY Peripheral Index Number")
    drvExtPhyLan867xIndexNum.setDefaultValue(1)
    drvExtPhyLan867xIndexNum.setReadOnly(True)
    
    # Driver PHY Peripheral ID
    drvExtPhyLan867xPeripheralId = drvExtPhyLan867xComponent.createStringSymbol("DRV_ETHPHY_PERIPHERAL_ID", drvExtPhyLan867xAdvSettings)
    drvExtPhyLan867xPeripheralId.setLabel("PHY Peripheral ID")
    drvExtPhyLan867xPeripheralId.setVisible(True)
    drvExtPhyLan867xPeripheralId.setDescription("Driver PHY Peripheral ID")
    drvExtPhyLan867xPeripheralId.setDefaultValue("")
    drvExtPhyLan867xPeripheralId.setReadOnly(True)
    
    # Operation mode
    drvExtPhyLan867xMode = drvExtPhyLan867xComponent.createComboSymbol("DRV_ETHPHY_10BASE_T1S_MODE",None,["CSMA/CD", "PLCA"])
    drvExtPhyLan867xMode.setLabel("10BASE-T1S Operation Mode")
    drvExtPhyLan867xMode.setVisible(True)
    drvExtPhyLan867xMode.setDefaultValue("CSMA/CD")
    drvExtPhyLan867xMode.setDescription("10BASE-T1S Operation Mode: CSMA/CD and PLCA")

    # PLCA Settings
    drvExtPhyLan867xPLCA = drvExtPhyLan867xComponent.createMenuSymbol(None, None)
    drvExtPhyLan867xPLCA.setLabel("PLCA Settings")
    drvExtPhyLan867xPLCA.setVisible(False)
    drvExtPhyLan867xPLCA.setDescription("Set the PLCA settings for the PHY.")
    drvExtPhyLan867xPLCA.setDependencies(drvExtPhyLan867xSetOperationMode, ["DRV_ETHPHY_10BASE_T1S_MODE"])
    
    # PLCA Node Id
    drvExtPhyLan867xNodeId = drvExtPhyLan867xComponent.createIntegerSymbol("PLCA_NODE_ID", drvExtPhyLan867xPLCA)
    drvExtPhyLan867xNodeId.setLabel("Local Node Id")
    drvExtPhyLan867xNodeId.setVisible(True)
    drvExtPhyLan867xNodeId.setDescription("Set Local PLCA Node Id.")
    drvExtPhyLan867xNodeId.setDefaultValue(0)
    drvExtPhyLan867xNodeId.setMin(0)
    drvExtPhyLan867xNodeId.setMax(255)
    
    # PLCA Node Count
    drvExtPhyLan867xMaxId = drvExtPhyLan867xComponent.createIntegerSymbol("PLCA_NODE_COUNT", drvExtPhyLan867xPLCA)
    drvExtPhyLan867xMaxId.setLabel("Node Count")
    drvExtPhyLan867xMaxId.setVisible(True)
    drvExtPhyLan867xMaxId.setDescription("Set PLCA Node Count.")
    drvExtPhyLan867xMaxId.setDefaultValue(8)
    drvExtPhyLan867xMaxId.setMin(0)
    drvExtPhyLan867xMaxId.setMax(255)

    # PLCA Max Burst Count
    drvExtPhyLan867xBurstCount = drvExtPhyLan867xComponent.createIntegerSymbol("PLCA_BURST_COUNT", drvExtPhyLan867xPLCA)
    drvExtPhyLan867xBurstCount.setLabel("Max Burst Count")
    drvExtPhyLan867xBurstCount.setVisible(True)
    drvExtPhyLan867xBurstCount.setDescription("Set maximum Burst Count.")
    drvExtPhyLan867xBurstCount.setDefaultValue(0)
    drvExtPhyLan867xBurstCount.setMin(0)
    drvExtPhyLan867xBurstCount.setMax(255)
    
    # PLCA Burst Timer
    drvExtPhyLan867xBurstTimer = drvExtPhyLan867xComponent.createIntegerSymbol("PLCA_BURST_TIMER", drvExtPhyLan867xPLCA)
    drvExtPhyLan867xBurstTimer.setLabel("Burst Timer")
    drvExtPhyLan867xBurstTimer.setVisible(True)
    drvExtPhyLan867xBurstTimer.setDescription("Set Burst Timer.")
    drvExtPhyLan867xBurstTimer.setDefaultValue(128)
    drvExtPhyLan867xBurstTimer.setMin(0)
    drvExtPhyLan867xBurstTimer.setMax(255)

    # PLCA Burst Timer comment
    drvExtPhyLan867xBurstTimer_comment = drvExtPhyLan867xComponent.createCommentSymbol("BURST_TIMER_COMMENT", drvExtPhyLan867xPLCA)
    drvExtPhyLan867xBurstTimer_comment.setLabel("**** Burst Timer value is "  + str(0.1 * int (drvExtPhyLan867xBurstTimer.getValue())) +" us ****")
    drvExtPhyLan867xBurstTimer_comment.setDependencies(drvExtPhyLan867xSetBurstComment, ["PLCA_BURST_TIMER"])
    
    # Add forward declaration to initialization.c
    drvExtPhyLan867xInitDataSourceFtl = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xInitDataSourceFtl.setType("STRING")
    drvExtPhyLan867xInitDataSourceFtl.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    drvExtPhyLan867xInitDataSourceFtl.setSourcePath("../net/driver/ethphy/templates/system/system_driver_initialize.c.ftl")
    drvExtPhyLan867xInitDataSourceFtl.setMarkup(True)    
    
    # Add to initialization.c
    drvExtPhyLan867xSysInitDataSourceFtl = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xSysInitDataSourceFtl.setType("STRING")
    drvExtPhyLan867xSysInitDataSourceFtl.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
    drvExtPhyLan867xSysInitDataSourceFtl.setSourcePath("../net/driver/ethphy/templates/system/system_data_initialize.c.ftl")
    drvExtPhyLan867xSysInitDataSourceFtl.setMarkup(True)

    #Add to system_config.h
    drvExtPhyLan867xHeaderFtl = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xHeaderFtl.setSourcePath("driver/lan867x/config/drv_extphy_lan867x.h.ftl")
    drvExtPhyLan867xHeaderFtl.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
    drvExtPhyLan867xHeaderFtl.setMarkup(True)
    drvExtPhyLan867xHeaderFtl.setType("STRING")
    
    # file TCPIP_ETH_PHY_H "$HARMONY_VERSION_PATH/framework/driver/ethphy/drv_ethphy.h" to "$PROJECT_HEADER_FILES/framework/driver/ethphy/drv_ethphy.h"
    # Add drv_ethphy.h file to project
    drvExtPhyHeaderFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyHeaderFile.setSourcePath("../net/driver/ethphy/drv_ethphy.h")
    drvExtPhyHeaderFile.setOutputName("drv_ethphy.h")
    drvExtPhyHeaderFile.setDestPath("driver/ethphy/")
    drvExtPhyHeaderFile.setProjectPath("config/" + configName + "/driver/ethphy/")
    drvExtPhyHeaderFile.setType("HEADER")
    drvExtPhyHeaderFile.setOverwrite(True)
    drvExtPhyHeaderFile.setEnabled(True)
    #drvEthPhyHeaderFile.setDependencies(drvGmacGenHeaderFile, ["TCPIP_USE_ETH_MAC"])

    # file TCPIP_ETH_PHY_LOCAL_H "$HARMONY_VERSION_PATH/framework/driver/ethphy/src/drv_ethphy_local.h" to "$PROJECT_HEADER_FILES/framework/driver/ethphy/src/drv_ethphy_local.h"
    # Add drv_ethphy_local.h file to project
    drvExtPhyLocalHeaderFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLocalHeaderFile.setSourcePath("../net/driver/ethphy/src/drv_ethphy_local.h")
    drvExtPhyLocalHeaderFile.setOutputName("drv_ethphy_local.h")
    drvExtPhyLocalHeaderFile.setDestPath("driver/ethphy/src/")
    drvExtPhyLocalHeaderFile.setProjectPath("config/" + configName + "/driver/ethphy/src/")
    drvExtPhyLocalHeaderFile.setType("HEADER")
    drvExtPhyLocalHeaderFile.setOverwrite(True)
    drvExtPhyLocalHeaderFile.setEnabled(True)
    #drvEthPhyLocalHeaderFile.setDependencies(drvGmacGenHeaderFile, ["TCPIP_USE_ETH_MAC"])

    # file TCPIP_ETH_EXT_PHY_REGS_H "$HARMONY_VERSION_PATH/framework/driver/ethphy/src/dynamic/drv_extphy_regs.h" to "$PROJECT_HEADER_FILES/framework/driver/ethphy/src/dynamic/drv_extphy_regs.h"
    # Add drv_extphy_regs.h file to project
    drvExtPhyLan867xRegHeaderFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xRegHeaderFile.setSourcePath("../net/driver/ethphy/src/dynamic/drv_extphy_regs.h")
    drvExtPhyLan867xRegHeaderFile.setOutputName("drv_extphy_regs.h")
    drvExtPhyLan867xRegHeaderFile.setDestPath("driver/ethphy/src/dynamic/")
    drvExtPhyLan867xRegHeaderFile.setProjectPath("config/" + configName + "/driver/ethphy/src/dynamic/")
    drvExtPhyLan867xRegHeaderFile.setType("HEADER")
    drvExtPhyLan867xRegHeaderFile.setOverwrite(True)
    drvExtPhyLan867xRegHeaderFile.setEnabled(True)
    #drvExtPhyLan867xRegHeaderFile.setDependencies(drvGmacGenHeaderFile, ["TCPIP_USE_ETH_MAC"])


    # file TCPIP_ETH_PHY_C "$HARMONY_VERSION_PATH/framework/driver/ethphy/src/dynamic/drv_ethphy.c" to         "$PROJECT_SOURCE_FILES/framework/driver/ethphy/drv_ethphy.c"
    # Add drv_ethphy.c file
    drvExtPhySourceFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhySourceFile.setSourcePath("../net/driver/ethphy/src/dynamic/drv_ethphy.c")
    drvExtPhySourceFile.setOutputName("drv_ethphy.c")
    drvExtPhySourceFile.setOverwrite(True)
    drvExtPhySourceFile.setDestPath("driver/ethphy/src/dynamic/")
    drvExtPhySourceFile.setProjectPath("config/" + configName + "/driver/ethphy/src/dynamic/")
    drvExtPhySourceFile.setType("SOURCE")
    drvExtPhySourceFile.setEnabled(True)
    #drvExtPhySourceFile.setDependencies(tcpipGmacGenSourceFile, ["TCPIP_USE_ETH_MAC"])

    # file TCPIP_MAC_PHY_Lan867x_H "$HARMONY_VERSION_PATH/framework/driver/ethphy/src/dynamic/drv_extphy_Lan867x.h" to "$PROJECT_HEADER_FILES/framework/driver/ethphy/src/dynamic/drv_extphy_Lan867x.h"
    # Add drv_extphy_lan867x.h file to project
    drvExtPhyLan867xHeaderFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xHeaderFile.setSourcePath("driver/lan867x/src/drv_extphy_lan867x.h")
    drvExtPhyLan867xHeaderFile.setOutputName("drv_extphy_lan867x.h")
    drvExtPhyLan867xHeaderFile.setDestPath("driver/ethphy/src/dynamic/")
    drvExtPhyLan867xHeaderFile.setProjectPath("config/" + configName + "/driver/ethphy/src/dynamic/")
    drvExtPhyLan867xHeaderFile.setType("HEADER")
    drvExtPhyLan867xHeaderFile.setOverwrite(True)
    #drvExtPhyLan867xHeaderFile.setDependencies(drvGmacGenHeaderFile, ["TCPIP_USE_ETH_MAC"])  
    
    # ifblock TCPIP_EMAC_PHY_TYPE = "Lan867x"
    # file TCPIP_MAC_PHY_Lan867x_C "$HARMONY_VERSION_PATH/framework/driver/ethphy/src/dynamic/drv_extphy_Lan867x.c" to "$PROJECT_SOURCE_FILES/framework/driver/ethphy/drv_extphy_Lan867x.c"
    # endif
    # Add drv_extphy_Lan867x.c file
    drvExtPhyLan867xSourceFile = drvExtPhyLan867xComponent.createFileSymbol(None, None)
    drvExtPhyLan867xSourceFile.setSourcePath("driver/lan867x/src/drv_extphy_lan867x.c")
    drvExtPhyLan867xSourceFile.setOutputName("drv_extphy_lan867x.c")
    drvExtPhyLan867xSourceFile.setOverwrite(True)
    drvExtPhyLan867xSourceFile.setDestPath("driver/ethphy/src/dynamic/")
    drvExtPhyLan867xSourceFile.setProjectPath("config/" + configName + "/driver/ethphy/src/dynamic/")
    drvExtPhyLan867xSourceFile.setType("SOURCE")
    drvExtPhyLan867xSourceFile.setEnabled(True)
    # drvExtPhyLan867xSourceFile.setDependencies(drvExtPhyLan867xGenSourceFile, ["TCPIP_USE_ETH_MAC","TCPIP_EMAC_PHY_TYPE"])
    
    
def drvExtPhyLan867xMenuVisibleSingle(symbol, event):
    if (event["value"] == True):
        print("EthMac Menu Visible.")
        symbol.setVisible(True)
    else:
        print("EthMac Menu Invisible.")
        symbol.setVisible(False)


def drvExtPhyLan867xSetOperationMode(symbol, event):
    if (event["value"] == "PLCA"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)


#Set the PLCA burst timer comment
def drvExtPhyLan867xSetBurstComment(symbol, event):
    symbol.setLabel("**** Burst Timer value is "  + str(0.1 * int (event["value"])) +" us ****")


#Set symbols of other components
def setVal(component, symbol, value):
    triggerDict = {"Component":component,"Id":symbol, "Value":value}
    if(Database.sendMessage(component, "SET_SYMBOL", triggerDict) == None):
        print "Set Symbol Failure" + component + ":" + symbol + ":" + str(value)
        return False
    else:
        return True
'''
def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]


def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]
'''

#Handle messages from other components
def handleMessage(messageID, args):
    retDict= {}
    if (messageID == "SET_SYMBOL"):
        print "handleMessage: Set Symbol"
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict