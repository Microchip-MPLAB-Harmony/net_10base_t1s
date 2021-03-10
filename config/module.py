######################  Harmony 10BaseT1S driver  ######################
def loadModule():
    print("Load Module: Harmony 10BaseT1S driver")

    #Driver for MCHP LAN867x PHY
    drvExtPhyLan867xComponent = Module.CreateComponent("drvExtPhyLan867x", "LAN867x", "/Harmony/Drivers/PHY Driver", "driver/lan867x/config/drv_extphy_lan867x.py")
    drvExtPhyLan867xComponent.addCapability("libdrvExtPhyLan867x", "PHY", True)   
    drvExtPhyLan867xComponent.addDependency("LAN867x_MIIM_Dependency", "MIIM", None, True, True)
