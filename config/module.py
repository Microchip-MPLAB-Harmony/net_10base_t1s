######################  Harmony 10BASE-T1S PHY  ######################
def loadModule():
    print("Load Module: Harmony 10BASE-T1S PHY")

    #Driver for MCHP LAN867x PHY
    drvExtPhyLan867xComponent = Module.CreateComponent("drvExtPhyLan867x", "LAN867x", "/Harmony/Drivers/PHY Driver", "driver/lan867x/config/drv_extphy_lan867x.py")
    drvExtPhyLan867xComponent.addCapability("libdrvExtPhyLan867x", "PHY", True)   
    drvExtPhyLan867xComponent.addDependency("LAN867x_MIIM_Dependency", "MIIM", None, True, True)
