"""
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
"""

def instantiateComponent(drvExtMacLan865xComponent, index):
    configName = Variables.get("__CONFIGURATION_NAME")
    
    drvLan865xInstnIndex = drvExtMacLan865xComponent.createIntegerSymbol("INDEX", None)
    drvLan865xInstnIndex.setLabel("LAN865X Instance Number")
    drvLan865xInstnIndex.setVisible(True)
    drvLan865xInstnIndex.setDefaultValue(index)
    drvLan865xInstnIndex.setReadOnly(True)
    
    drvLan865xInstSpiIdx = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX" + str(index), None)
    drvLan865xInstSpiIdx.setLabel("SPI Driver Instance Used")
    drvLan865xInstSpiIdx.setReadOnly(True)
    drvLan865xInstSpiIdx.setDefaultValue(0)
    drvLan865xInstSpiIdx.setVisible(True)
    
    drvLan865xInstnClientNum = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_CLIENT_INSTANCES_IDX" + str(index),None)
    drvLan865xInstnClientNum.setLabel("Number of Client Instances")
    drvLan865xInstnClientNum.setVisible(True)
    drvLan865xInstnClientNum.setDefaultValue(1)
    drvLan865xInstnClientNum.setDescription("Number of Client Instances")
    
    drvLan865xSpiFreq = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_SPI_FREQ_IDX" + str(index),None)
    drvLan865xSpiFreq.setLabel("SPI frequency in Hz")
    drvLan865xSpiFreq.setVisible(True)
    drvLan865xSpiFreq.setDefaultValue(15000000)
    drvLan865xSpiFreq.setDescription("SPI Frequency in Hz")
    drvLan865xSpiFreq.setMin(100000)
    drvLan865xSpiFreq.setMax(25000000)
    
    drvLan865xInstnRxDescNum = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_MAC_RX_DESCRIPTORS_IDX" + str(index),None)
    drvLan865xInstnRxDescNum.setVisible(True)
    drvLan865xInstnRxDescNum.setLabel("Number of the RX Descriptors")
    drvLan865xInstnRxDescNum.setDefaultValue(2)
    drvLan865xInstnRxDescNum.setDescription("Number of the RX Descriptors")
    
    drvLan865xInstnRxDescBuffSize = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_MAX_RX_BUFFER_IDX" + str(index),None)
    drvLan865xInstnRxDescBuffSize.setVisible(True)
    drvLan865xInstnRxDescBuffSize.setLabel("RX Descriptor Buffer Size")
    drvLan865xInstnRxDescBuffSize.setDefaultValue(1536)
    drvLan865xInstnRxDescBuffSize.setDescription("RX Descriptor Buffer Size")
    
    drvLan865xSpiChipSelect = drvExtMacLan865xComponent.createKeyValueSetSymbol("DRV_LAN865X_SPI_CS_IDX" + str(index),None)
    drvLan865xSpiChipSelect.setLabel("SPI Chip Select Pin")
    drvLan865xSpiChipSelect.setOutputMode("Key")
    drvLan865xSpiChipSelect.setDisplayMode("Key")
    drvLan865xSpiChipSelect.setVisible( True )
    
    # Send message to core to get available pins
    chipSelectavailablePinDictionary = {}
    chipSelectavailablePinDictionary = Database.sendMessage("core", "PIN_LIST", chipSelectavailablePinDictionary)
    
    drvLan865xSpiChipSelect.addKey("SYS_PORT_PIN_NONE", "0", "No Pin Selected")
    for pad in sort_alphanumeric(chipSelectavailablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(chipSelectavailablePinDictionary.keys())[list(chipSelectavailablePinDictionary.values()).index(pad)]
        description = pad
        drvLan865xSpiChipSelect.addKey(key, value, description)
    
    drvLan865xSpiChipSelect.setSelectedKey("SYS_PORT_PIN_NONE")
    
    drvLan865xSpiComment = drvExtMacLan865xComponent.createCommentSymbol("DRV_LAN865X_SPI_COMMENT_IDX" + str(index), drvLan865xSpiChipSelect)
    drvLan865xSpiComment.setLabel("***Configure the Chip-Select pin as GPIO Output in Pin Manager***")
    drvLan865xSpiComment.setVisible( True )
    

    drvLan865xInterruptPin = drvExtMacLan865xComponent.createKeyValueSetSymbol("DRV_LAN865X_INTERRUPT_PIN_IDX" + str(index),None)
    drvLan865xInterruptPin.setLabel("Interrupt Pin")
    drvLan865xInterruptPin.setOutputMode("Key")
    drvLan865xInterruptPin.setDisplayMode("Key")
    drvLan865xInterruptPin.setVisible( True )
    
    drvLan865xInterruptPin.addKey("SYS_PORT_PIN_NONE", "0", "No Pin Selected")
    
    for pad in sort_alphanumeric(chipSelectavailablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(chipSelectavailablePinDictionary.keys())[list(chipSelectavailablePinDictionary.values()).index(pad)]
        description = pad
        drvLan865xInterruptPin.addKey(key, value, description)
    
    drvLan865xInterruptPin.setSelectedKey("SYS_PORT_PIN_NONE")

    drvLan865xInterruptComment = drvExtMacLan865xComponent.createCommentSymbol("DRV_LAN865X_INTERRUPT_COMMENT_IDX" + str(index), drvLan865xInterruptPin)
    drvLan865xInterruptComment.setLabel("***Configure the Interrupt pin as GPIO Input in Pin Manager***")
    drvLan865xInterruptComment.setVisible( True )
    
    drvLan865xResetPin = drvExtMacLan865xComponent.createKeyValueSetSymbol("DRV_LAN865X_RESET_PIN_IDX" + str(index),None)
    drvLan865xResetPin.setLabel("Reset Pin")
    drvLan865xResetPin.setOutputMode("Key")
    drvLan865xResetPin.setDisplayMode("Key")
    drvLan865xResetPin.setVisible( True )
    
    drvLan865xResetPin.addKey("SYS_PORT_PIN_NONE", "0", "No Pin Selected")
    
    for pad in sort_alphanumeric(chipSelectavailablePinDictionary.values()):
        key = "SYS_PORT_PIN_" + pad
        value = list(chipSelectavailablePinDictionary.keys())[list(chipSelectavailablePinDictionary.values()).index(pad)]
        description = pad
        drvLan865xResetPin.addKey(key, value, description)
    
    drvLan865xResetPin.setSelectedKey("SYS_PORT_PIN_NONE")

    drvLan865xResetComment = drvExtMacLan865xComponent.createCommentSymbol("DRV_LAN865X_RESET_COMMENT_IDX" + str(index), drvLan865xResetPin)
    drvLan865xResetComment.setLabel("***Optional*** ***Configure the RESET pin as GPIO Output in Pin Manager***")
    drvLan865xResetComment.setVisible( True )
    
    # Operation mode
    drvLan865xInstnMode = drvExtMacLan865xComponent.createComboSymbol("DRV_LAN865X_10BASE_T1S_MODE_IDX" + str(index), None, ["CSMA/CD", "PLCA"])
    drvLan865xInstnMode.setLabel("10BASE-T1S Operation Mode")
    drvLan865xInstnMode.setVisible(True)
    drvLan865xInstnMode.setDefaultValue("PLCA")
    drvLan865xInstnMode.setDescription("10BASE-T1S Operation Mode: CSMA/CD and PLCA")

    # PLCA Settings
    drvLan865xInstnPLCA = drvExtMacLan865xComponent.createMenuSymbol(None, drvLan865xInstnMode)
    drvLan865xInstnPLCA.setLabel("PLCA Settings")
    drvLan865xInstnPLCA.setVisible(True)
    drvLan865xInstnPLCA.setDescription("Set the PLCA settings for the PHY.")
    drvLan865xInstnPLCA.setDependencies(drvExtPhyLan865xSetOperationMode, ["DRV_LAN865X_10BASE_T1S_MODE_IDX" + str(index)])
    
    # PLCA Node Id
    drvLan865xInstnNodeId = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_PLCA_NODE_ID_IDX" + str(index), drvLan865xInstnPLCA)
    drvLan865xInstnNodeId.setLabel("Local Node Id")
    drvLan865xInstnNodeId.setVisible(True)
    drvLan865xInstnNodeId.setDescription("Set Local PLCA Node Id.")
    drvLan865xInstnNodeId.setDefaultValue(0)
    drvLan865xInstnNodeId.setMin(0)
    drvLan865xInstnNodeId.setMax(255)
    
    # PLCA Node Count
    drvLan865xInstnNodeCount = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_PLCA_NODE_COUNT_IDX"+ str(index), drvLan865xInstnPLCA)
    drvLan865xInstnNodeCount.setLabel("Node Count")
    drvLan865xInstnNodeCount.setVisible(True)
    drvLan865xInstnNodeCount.setDescription("Set PLCA Node Count.")
    drvLan865xInstnNodeCount.setDefaultValue(8)
    drvLan865xInstnNodeCount.setMin(0)
    drvLan865xInstnNodeCount.setMax(255)

    # PLCA Max Burst Count
    drvLan865xInstnBurstCount = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_PLCA_BURST_COUNT_IDX"+ str(index), drvLan865xInstnPLCA)
    drvLan865xInstnBurstCount.setLabel("Max Burst Count")
    drvLan865xInstnBurstCount.setVisible(True)
    drvLan865xInstnBurstCount.setDescription("Set maximum Burst Count.")
    drvLan865xInstnBurstCount.setDefaultValue(0)
    drvLan865xInstnBurstCount.setMin(0)
    drvLan865xInstnBurstCount.setMax(255)
    
    # PLCA Burst Timer
    drvLan865xInstnBurstTimer = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_PLCA_BURST_TIMER_IDX"+ str(index), drvLan865xInstnPLCA)
    drvLan865xInstnBurstTimer.setLabel("Burst Timer")
    drvLan865xInstnBurstTimer.setVisible(True)
    drvLan865xInstnBurstTimer.setDescription("Set Burst Timer.")
    drvLan865xInstnBurstTimer.setDefaultValue(128)
    drvLan865xInstnBurstTimer.setMin(0)
    drvLan865xInstnBurstTimer.setMax(255)

    # PLCA Burst Timer comment
    drvLan865xInstnBurstTimer_comment = drvExtMacLan865xComponent.createCommentSymbol("BURST_TIMER_COMMENT_IDX"+ str(index), drvLan865xInstnPLCA)
    drvLan865xInstnBurstTimer_comment.setLabel("**** Burst Timer value is "  + str(0.1 * int (drvLan865xInstnBurstTimer.getValue())) +" us ****")
    drvLan865xInstnBurstTimer_comment.setDependencies(drvExtPhyLan865xSetBurstComment, ["DRV_LAN865X_PLCA_BURST_TIMER_IDX"+ str(index)])
    
    ## Advanced Settings
    drvLan865xAdvSettings = drvExtMacLan865xComponent.createMenuSymbol("TCPIP_ETHMAC_ADV_SETTING", None)
    drvLan865xAdvSettings.setLabel("Advanced Settings")
    drvLan865xAdvSettings.setDescription("Advanced Settings")
    drvLan865xAdvSettings.setVisible(True)
    
    # Chunk size
    drvLan865xInstnChunkSize = drvExtMacLan865xComponent.createComboSymbol("DRV_LAN865X_CHUNK_SIZE_IDX" + str(index), drvLan865xAdvSettings, ["32", "64"])
    drvLan865xInstnChunkSize.setLabel("TC6 chunk size")
    drvLan865xInstnChunkSize.setVisible(True)
    drvLan865xInstnChunkSize.setDefaultValue("64")
    drvLan865xInstnChunkSize.setDescription("Set TC6 payload chunk size")
    
    # Chunks per Transaction
    drvLan865xInstnChunkXact = drvExtMacLan865xComponent.createIntegerSymbol("DRV_LAN865X_CHUNK_XACT_IDX"+ str(index), drvLan865xAdvSettings)
    drvLan865xInstnChunkXact.setLabel("TC6 chunks per SPI Transaction (XACT)")
    drvLan865xInstnChunkXact.setVisible(True)
    drvLan865xInstnChunkXact.setDescription("Sets the amount of TC6 chunks within in a single SPI transaction.")
    drvLan865xInstnChunkXact.setDefaultValue(31)
    drvLan865xInstnChunkXact.setMin(1)
    drvLan865xInstnChunkXact.setMax(31)
    
    # Promiscuous
    drvLan865xInstnPromiscuous = drvExtMacLan865xComponent.createBooleanSymbol("DRV_LAN865X_PROMISCUOUS_IDX" + str(index), drvLan865xAdvSettings)
    drvLan865xInstnPromiscuous.setVisible(True)
    drvLan865xInstnPromiscuous.setLabel("Promiscuous")
    drvLan865xInstnPromiscuous.setDescription("Set Promiscuous mode")
    
    # TX Cut Through
    drvLan865xInstnTxCutThrough = drvExtMacLan865xComponent.createBooleanSymbol("DRV_LAN865X_TX_CUT_THROUGH_IDX" + str(index), drvLan865xAdvSettings)
    drvLan865xInstnTxCutThrough.setVisible(True)
    drvLan865xInstnTxCutThrough.setLabel("TX cut through")
    drvLan865xInstnTxCutThrough.setDescription("Checked, set TX cut through mode, otherwise TX store and forward mode");
    
    # RX Cut Through
    drvLan865xInstnTxCutThrough = drvExtMacLan865xComponent.createBooleanSymbol("DRV_LAN865X_RX_CUT_THROUGH_IDX" + str(index), drvLan865xAdvSettings)
    drvLan865xInstnTxCutThrough.setVisible(True)
    drvLan865xInstnTxCutThrough.setLabel("RX cut through")
    drvLan865xInstnTxCutThrough.setDescription("Checked, set R cut through mode, otherwise RX store and forward mode");


def sort_alphanumeric(l):
    import re
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def drvExtPhyLan865xSetOperationMode(symbol, event):
    if (event["value"] == "PLCA"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

#Set the PLCA burst timer comment
def drvExtPhyLan865xSetBurstComment(symbol, event):
    symbol.setLabel("**** Burst Timer value is "  + str(0.1 * int (event["value"])) +" us ****")

#Set symbols of other components
def setVal(component, symbol, value):
    triggerDict = {"Component":component,"Id":symbol, "Value":value}
    if(Database.sendMessage(component, "SET_SYMBOL", triggerDict) == None):
        print "Set Symbol Failure" + component + ":" + symbol + ":" + str(value)
        return False
    else:
        return True

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

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    localID = localComponent.getID()
    remoteID = remoteComponent.getID()
    indexLocal = Database.getSymbolValue(localID, "INDEX")
    indexRemote = Database.getSymbolValue(remoteID, "INDEX")
    drvInstance = localComponent.getSymbolByID("DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX" + str(indexLocal))
    drvInstance.clearValue()
    drvInstance.setValue(indexRemote)

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    localID = localComponent.getID()
    remoteID = remoteComponent.getID()
    indexLocal = Database.getSymbolValue(localID, "INDEX")
    indexRemote = Database.getSymbolValue(remoteID, "INDEX")
    drvInstance = localComponent.getSymbolByID("DRV_LAN865X_SPI_DRIVER_INSTANCE_IDX" + str(indexLocal))
    drvInstance.clearValue()
