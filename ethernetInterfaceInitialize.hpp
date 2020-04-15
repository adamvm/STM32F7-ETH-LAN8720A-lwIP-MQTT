/**
 * \file
 * \brief ethernetInterfaceInitialize() declaration
 *
 * \author Copyright (C) 2019-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ETHERNETINTERFACEINITIALIZE_HPP_
#define ETHERNETINTERFACEINITIALIZE_HPP_

#include "lwip/err.h"

struct netif;

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Sets up Ethernet interface.
 *
 * Should be called at the beginning of the program to set up the network interface. This function should be passed as a
 * parameter to netif_add().
 *
 * \param [in] netif is the lwIP network interface structure for this Ethernet interface
 *
 * \return ERR_OK on success, any other err_t otherwise
 */

err_t ethernetInterfaceInitialize(netif* netif);

#endif	// ETHERNETINTERFACEINITIALIZE_HPP_
