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

def instantiateComponent(drvExtMacLan865xCommonComponent):
    print("External Ethernet Controller LAN865X Driver Component")
    configName = Variables.get("__CONFIGURATION_NAME")

    # Enable LAN865X
    drvLan865xModule = drvExtMacLan865xCommonComponent.createBooleanSymbol("DRV_LAN865X_USE_DRIVER", None)
    drvLan865xModule.setLabel("Enable LAN865X")
    drvLan865xModule.setVisible(False)
    drvLan865xModule.setDescription("Enable LAN865X")
    drvLan865xModule.setDefaultValue(True)

    # add "<#include \"/framework/driver/lan865x/config/drv_lan865x.c.ftl\">"  to list SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA
    drvLan865xInitDataSourceFtl = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xInitDataSourceFtl.setType("STRING")
    drvLan865xInitDataSourceFtl.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    drvLan865xInitDataSourceFtl.setSourcePath("driver/lan865x/config/drv_lan865x.c.ftl")
    drvLan865xInitDataSourceFtl.setMarkup(True)

    drvLan865xSystemDefFile = drvExtMacLan865xCommonComponent.createFileSymbol("DRV_LAN865X_H_FILE", None)
    drvLan865xSystemDefFile.setType("STRING")
    drvLan865xSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    drvLan865xSystemDefFile.setSourcePath("driver/lan865x/templates/system/system_definitions.h.ftl")
    drvLan865xSystemDefFile.setMarkup(True)

    # add "<#include \"/framework/driver/lan865x/config/drv_lan865x.h.ftl\">"  to list SYSTEM_CONFIG_H_DRIVER_CONFIGURATION
    drvLan865xSysConfigSourceFtl = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xSysConfigSourceFtl.setType("STRING")
    drvLan865xSysConfigSourceFtl.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    drvLan865xSysConfigSourceFtl.setSourcePath("driver/lan865x/config/drv_lan865x.h.ftl")
    drvLan865xSysConfigSourceFtl.setMarkup(True)

    drvLan865xSystemDefObjFile = drvExtMacLan865xCommonComponent.createFileSymbol("DRV_LAN865X_DEF_OBJ", None)
    drvLan865xSystemDefObjFile.setType("STRING")
    drvLan865xSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    drvLan865xSystemDefObjFile.setSourcePath("driver/lan865x/templates/system/system_definitions_objects.h.ftl")
    drvLan865xSystemDefObjFile.setMarkup(True)

    # file DRV_LAN865X_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/drv_lan865x.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/drv_lan865x.h"
    drvLan865xHeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xHeaderFile.setSourcePath("driver/lan865x/drv_lan865x.h")
    drvLan865xHeaderFile.setOutputName("drv_lan865x.h")
    drvLan865xHeaderFile.setDestPath("driver/lan865x/")
    drvLan865xHeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/")
    drvLan865xHeaderFile.setType("HEADER")
    drvLan865xHeaderFile.setOverwrite(True)

    # file DRV_LAN865X_API_C "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/drv_lan865x_api.c" to "$PROJECT_SOURCE_FILES/framework/driver/lan865x/src/dynamic/drv_lan865x_api.c"
    drvLan865xApiSourceFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xApiSourceFile.setSourcePath("driver/lan865x/src/dynamic/drv_lan865x_api.c")
    drvLan865xApiSourceFile.setOutputName("drv_lan865x_api.c")
    drvLan865xApiSourceFile.setOverwrite(True)
    drvLan865xApiSourceFile.setDestPath("driver/lan865x/src/dynamic/")
    drvLan865xApiSourceFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/")
    drvLan865xApiSourceFile.setType("SOURCE")
    drvLan865xApiSourceFile.setEnabled(True)

    # file DRV_LAN865X_LOCAL_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/drv_lan865x_local.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/impl/drv_lan865x_local.h"
    drvLan865xLocalHeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xLocalHeaderFile.setSourcePath("driver/lan865x/src/dynamic/drv_lan865x_local.h")
    drvLan865xLocalHeaderFile.setOutputName("drv_lan865x_local.h")
    drvLan865xLocalHeaderFile.setDestPath("driver/lan865x/src/dynamic/")
    drvLan865xLocalHeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/")
    drvLan865xLocalHeaderFile.setType("HEADER")
    drvLan865xLocalHeaderFile.setOverwrite(True)

    # file DRV_LAN865X_REGS_C "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/drv_lan865x_regs.c" to "$PROJECT_SOURCE_FILES/framework/driver/lan865x/src/dynamic/drv_lan865x_regs.c"
    drvLan865xRegsSourceFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xRegsSourceFile.setSourcePath("driver/lan865x/src/dynamic/drv_lan865x_regs.c")
    drvLan865xRegsSourceFile.setOutputName("drv_lan865x_regs.c")
    drvLan865xRegsSourceFile.setOverwrite(True)
    drvLan865xRegsSourceFile.setDestPath("driver/lan865x/src/dynamic/")
    drvLan865xRegsSourceFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/")
    drvLan865xRegsSourceFile.setType("SOURCE")
    drvLan865xRegsSourceFile.setEnabled(True)

    # file DRV_LAN865X_REGS_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/drv_lan865x_regs.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/src/dynamic/drv_lan865x_regs.h"
    drvLan865xRegsHeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xRegsHeaderFile.setSourcePath("driver/lan865x/src/dynamic/drv_lan865x_regs.h")
    drvLan865xRegsHeaderFile.setOutputName("drv_lan865x_regs.h")
    drvLan865xRegsHeaderFile.setDestPath("driver/lan865x/src/dynamic/")
    drvLan865xRegsHeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/")
    drvLan865xRegsHeaderFile.setType("HEADER")
    drvLan865xRegsHeaderFile.setOverwrite(True)

    #############################
    #### TC6 Protocol Driver ####
    #############################

    # file DRV_LAN865X_TC6_C "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/tc6/tc6.c" to "$PROJECT_SOURCE_FILES/framework/driver/lan865x/src/dynamic/tc6/tc6.c"
    drvLan865xTC6SourceFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xTC6SourceFile.setSourcePath("driver/lan865x/src/dynamic/tc6/tc6.c")
    drvLan865xTC6SourceFile.setOutputName("tc6.c")
    drvLan865xTC6SourceFile.setOverwrite(True)
    drvLan865xTC6SourceFile.setDestPath("driver/lan865x/src/dynamic/tc6")
    drvLan865xTC6SourceFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/tc6/")
    drvLan865xTC6SourceFile.setType("SOURCE")
    drvLan865xTC6SourceFile.setEnabled(True)

    # file DRV_LAN865X_TC6_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/tc6/tc6.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/src/dynamic/tc6/tc6.h"
    drvLan865xTC6HeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xTC6HeaderFile.setSourcePath("driver/lan865x/src/dynamic/tc6/tc6.h")
    drvLan865xTC6HeaderFile.setOutputName("tc6.h")
    drvLan865xTC6HeaderFile.setDestPath("driver/lan865x/src/dynamic/tc6")
    drvLan865xTC6HeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/tc6/")
    drvLan865xTC6HeaderFile.setType("HEADER")
    drvLan865xTC6HeaderFile.setOverwrite(True)

    # file DRV_LAN865X_TC6CONF_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/tc6/tc6-conf.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/src/dynamic/tc6/tc6-conf.h"
    drvLan865xTC6ConfHeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xTC6ConfHeaderFile.setSourcePath("driver/lan865x/src/dynamic/tc6/tc6-conf.h")
    drvLan865xTC6ConfHeaderFile.setOutputName("tc6-conf.h")
    drvLan865xTC6ConfHeaderFile.setDestPath("driver/lan865x/src/dynamic/tc6")
    drvLan865xTC6ConfHeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/tc6/")
    drvLan865xTC6ConfHeaderFile.setType("HEADER")
    drvLan865xTC6ConfHeaderFile.setOverwrite(True)

    # file DRV_LAN865X_TC6QUEUE_H "$HARMONY_VERSION_PATH/framework/driver/lan865x/src/dynamic/tc6/tc6-queue.h" to "$PROJECT_HEADER_FILES/framework/driver/lan865x/src/dynamic/tc6/tc6-queue.h"
    drvLan865xTC6QueueHeaderFile = drvExtMacLan865xCommonComponent.createFileSymbol(None, None)
    drvLan865xTC6QueueHeaderFile.setSourcePath("driver/lan865x/src/dynamic/tc6/tc6-queue.h")
    drvLan865xTC6QueueHeaderFile.setOutputName("tc6-queue.h")
    drvLan865xTC6QueueHeaderFile.setDestPath("driver/lan865x/src/dynamic/tc6")
    drvLan865xTC6QueueHeaderFile.setProjectPath("config/" + configName + "/driver/lan865x/src/dynamic/tc6/")
    drvLan865xTC6QueueHeaderFile.setType("HEADER")
    drvLan865xTC6QueueHeaderFile.setOverwrite(True)

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

