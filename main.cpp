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
