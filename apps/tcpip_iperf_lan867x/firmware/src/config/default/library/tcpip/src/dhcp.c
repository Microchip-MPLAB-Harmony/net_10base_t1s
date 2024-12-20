/*******************************************************************************
  Dynamic Host Configuration Protocol (DHCP) Client

  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    - Provides automatic IP address, subnet mask, gateway address, 
      DNS server address, and other configuration parameters on DHCP 
      enabled networks.
    - Reference: RFC 2131, 2132
*******************************************************************************/

/*
Copyright (C) 2012-2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
*/

#include <ctype.h>
#include "tcpip/src/tcpip_private.h"

// Dummy functions when DHCP Client module is disabled
bool TCPIP_DHCP_IsEnabled(TCPIP_NET_HANDLE hNet)
{
    return false;
}

bool TCPIP_DHCP_IsActive(TCPIP_NET_HANDLE hNet)
{
    return false;
}

bool TCPIP_DHCP_Disable(TCPIP_NET_HANDLE hNet)
{
    return false;
}

bool TCPIP_DHCP_Enable(TCPIP_NET_HANDLE hNet)
{
    return false;
}

bool TCPIP_DHCP_Renew(TCPIP_NET_HANDLE hNet)
{
    return false;
}


bool TCPIP_DHCP_Request(TCPIP_NET_HANDLE hNet, IPV4_ADDR reqAddress)
{
    return false;
}

bool TCPIP_DHCP_InfoGet(TCPIP_NET_HANDLE hNet, TCPIP_DHCP_INFO* pDhcpInfo)
{
    return false;
}




