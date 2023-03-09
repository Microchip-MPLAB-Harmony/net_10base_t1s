![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## 10BASE-T1S Release v1.2.1
### Bug Fixes

The following table provides the list of bug fixes in the v1.2.1 release:

| Module    | Description                               |
| ------    | ------                                    |
| driver/lan867x/config  | Fix code generation fails with net v3.9.0    |

- **Release notes for v1.2.0 apply**

## 10BASE-T1S Release v1.2.0
### New Features

- **New device support** -
  This release introduces support for

    | Device    | Description             |
    | ------    | ------                  |
    | LAN865x   | 10BASE-T1S Ethernet MAC-PHY |

- **Demo application support** -
  The following table provides a list of of new applications

    | Application               | Platform  | Description                                               |
    | ------                    | ------    | ------                                                    |
    | tcpip_iperf_lan865x    | SAM E54   | TCP/IP Client using the LAN865x 10BASE-T1S Ethernet MAC-PHY   |

- **Development kit support** -
  The following table provides a list of supported development kits

    | Development Kits                                                                                                  |
    | ------                                                                                                            |
    | [SAM E54 Curiosity Ultra Development Board](https://www.microchip.com/Developmenttools/ProductDetails/DM320210)   |

### Dependencies

* [MPLAB® X IDE v6.05](https://www.microchip.com/mplab/mplab-x-ide) or later
* [MPLAB® XC32 C/C++ Compiler v4.21](https://www.microchip.com/mplab/compilers) or later
**(OR)** [MPLAB® XC32 Functional Safety Compiler v4.09](https://www.microchip.com/mplab/compilers) or later
* [net v3.9.1](https://github.com/Microchip-MPLAB-Harmony/net/tree/v3.9.1) or later

### Known Issues
* The build of the LAN867x sample applications fail because of 8 MISRA C 2012 Mandatory Violations.
  Additionally, warnings coming from net and wolfssl repo are reported.
  As all of those violations are not related to the net_10base_t1s package, the build can be considered as "PASSED".
* The build of the LAN865x sample applications fail because of 1 MISRA C 2012 Mandatory Violation.
  The violation is not related to the net_10base_t1s package, the build can be considered as "PASSED".

## 10BASE-T1S Release v1.1.1
### Bug Fixes

The following table provides the list of bug fixes in the v1.1.1 release:

| Module    | Description                               |
| ------    | ------                                    |
| module.py   | Update LAN867x driver component path   |
| docs   | Update offline docs using SDL   |
| tcpip_iperf_10base_t1s.X   | Update meta-data JSON file to register Bare-metal content into MCHP Discover portal   |
| tcpip_iperf_10base_t1s_freertos.X  | Update meta-data JSON file to register FreeRTOS content into MCHP Discover portal   |

### Dependencies

* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide) or later
* [MPLAB® XC32 C/C++ Compiler v4.00](https://www.microchip.com/mplab/compilers) or later
* [net v3.7.4](https://github.com/Microchip-MPLAB-Harmony/net/tree/v3.7.4) or later

**Release notes for v1.1.0 apply**

## 10BASE-T1S Release v1.1.0
### New Features

New features added in this release are as follows:

- Added FreeRTOS example project for LAN867x.

**Release notes for v1.0.1 apply**

## 10BASE-T1S Release v1.0.1

### Bug Fixes

The following table provides the list of bug fixes in the v1.0.1 release:

| Module    | Description                               |
| ------    | ------                                    |
| LAN867x   | Use correct data type for initialState    |

- **Release notes for v1.0.0 apply**

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
  Please contact [10BASE-T1S Support](https://www.microchip.com/support) for more information.

### Known Issues

* No known issues

### Dependencies

* [MPLAB® X IDE v5.45](https://www.microchip.com/mplab/mplab-x-ide) or later
* [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers) or later
* [net v3.7.2](https://github.com/Microchip-MPLAB-Harmony/net/tree/v3.7.2) or later
