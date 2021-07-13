---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## 10BASE-T1S Release v1.0.0
### New Features

- **New device support** - 
  This release introduces support for

    | Device    | Description             |
    | ------    | ------                  |
    | LAN867x   | 10BASE-T1S Ethernet PHY |

- **Demo application support** -
  The following table provides a list of of new applications

    | Application               | Platform  | Description                                               |
    | ------                    | ------    | ------                                                    |
    | tcpip_iperf_10base_t1s    | SAM E54   | TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY   |

- **Development kit support** -
  The following table provides a list of supported development kits

    | Development Kits                                                                                                  |
    | ------                                                                                                            |
    | [SAM E54 Curiosity Ultra Development Board](https://www.microchip.com/Developmenttools/ProductDetails/DM320210)   |

  In addition to the listed development kits above, a 10BASE-T1S PHY RMII Card is required.
  Please contact [10BASE-T1S Support](10BASE-T1S-Info@microchip.com) for more information.

### Known Issues

* No known issues

### Dependencies

* [MPLAB® X IDE v5.45](https://www.microchip.com/mplab/mplab-x-ide) or later
* [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers) or later
* [net](https://github.com/Microchip-MPLAB-Harmony/net/tree/v3.7.2)
