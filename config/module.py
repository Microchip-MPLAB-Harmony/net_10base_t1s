######################  Harmony 3 10BASE-T1S PHY  ######################
def loadModule():
    print("Load Module: Harmony 3 10BASE-T1S PHY")

    # Driver for MCHP LAN867x PHY
    drvExtPhyLan867xComponent = Module.CreateComponent("drvExtPhyLan867x", "LAN867x", "/Drivers/PHY Driver", "driver/lan867x/config/drv_extphy_lan867x.py")
    drvExtPhyLan867xComponent.addCapability("libdrvExtPhyLan867x", "PHY", False)
    drvExtPhyLan867xComponent.addDependency("LAN867x_MIIM_Dependency", "MIIM", None, True, True)
    drvExtPhyLan867xComponent.setDisplayType("PHY Layer")

    #Driver for MCHP LAN865x MACPHY
    drvExtMacLan865xComponent = Module.CreateGeneratorComponent("drvExtMacLan865x", "LAN865x", "/Drivers/External Ethernet Controller", "driver/lan865x/config/drv_lan865x_common.py", "driver/lan865x/config/drv_lan865x.py")
    drvExtMacLan865xComponent.addCapability("libdrvExtMacLan865x","MAC",None, False)
    drvExtMacLan865xComponent.addDependency("LAN865x_SPI_Dependency", "DRV_SPI", None, False, True)
    drvExtMacLan865xComponent.setDisplayType("MAC Layer")