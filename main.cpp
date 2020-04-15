/**
 * \file
 * \brief Main code block.
 *
 * \author Copyright (C) 2019-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ethernetInterfaceInitialize.hpp"

#include "distortos/board/initializeStreams.hpp"
#include "distortos/board/standardOutputStream.h"

#include "distortos/ThisThread.hpp"

#include "estd/ScopeGuard.hpp"

#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Link callback for network interface
 *
 * \param [in] netif is a pointer to network interface for which the link state changed
 */

void netifLinkCallback(netif* const netif)
{
	fiprintf(standardOutputStream, "netifLinkCallback: netif = %c%c%" PRIu8 ", link = %s\r\n",
			netif->name[0], netif->name[1], netif->num, netif_is_link_up(netif) != 0 ? "up" : "down");
}

/**
 * \brief Status callback for network interface
 *
 * \param [in] netif is a pointer to network interface for which the status changed
 */

void netifStatusCallback(netif* const netif)
{
	const auto linkUp = netif_is_link_up(netif) != 0;
	const auto statusUp = netif_is_up(netif) != 0;
	fiprintf(standardOutputStream, "netifStatusCallback: netif = %c%c%" PRIu8 ", link = %s, status = %s\r\n",
			netif->name[0], netif->name[1], netif->num, linkUp == true ? "up" : "down",
			statusUp == true ? "up" : "down");

	if (linkUp == false || statusUp == false)
		return;

	char buffer[IP4ADDR_STRLEN_MAX];
	fiprintf(standardOutputStream, "  ip4 = %s\r\n",
			ip4addr_ntoa_r(netif_ip4_addr(netif), buffer, sizeof(buffer)));
	fiprintf(standardOutputStream, "  gateway = %s\r\n",
			ip4addr_ntoa_r(netif_ip4_gw(netif), buffer, sizeof(buffer)));
	fiprintf(standardOutputStream, "  netmask = %s\r\n",
			ip4addr_ntoa_r(netif_ip4_netmask(netif), buffer, sizeof(buffer)));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Main code block of application
 *
 * \return doesn't return
 */

int main()
{
	distortos::board::initializeStreams();

	fiprintf(standardOutputStream, "Started %s board\r\n", DISTORTOS_BOARD);

	tcpip_init({}, {});

	netif networkInterface {};

	{
		LOCK_TCPIP_CORE();
		const auto unlockScopeGuard = estd::makeScopeGuard(
				[]()
				{
					UNLOCK_TCPIP_CORE();
				});

		{
			const auto ret = netif_add(&networkInterface, IP_ADDR_ANY, IP_ADDR_ANY, IP_ADDR_ANY, {},
					&ethernetInterfaceInitialize, &tcpip_input);
			assert(ret != nullptr);
		}

		netif_set_link_callback(&networkInterface, netifLinkCallback);
		netif_set_status_callback(&networkInterface, netifStatusCallback);
		netif_set_default(&networkInterface);
		netif_set_up(&networkInterface);

		{
			const auto ret = dhcp_start(&networkInterface);
			assert(ret == ERR_OK);
		}
	}

	while (1)
		distortos::ThisThread::sleepFor(std::chrono::seconds{1});
}
