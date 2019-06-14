/**
 * \file
 * \brief STM32F7xx_HAL_Driver configuration
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef STM32F7XX_HAL_DRIVER_CONFIGURATION_H_
#define STM32F7XX_HAL_DRIVER_CONFIGURATION_H_

/*---------------------------------------------------------------------------------------------------------------------+
| clock-related global defines
+---------------------------------------------------------------------------------------------------------------------*/

/** value of the external clock source for I2S peripheral, Hz */
#define EXTERNAL_CLOCK_VALUE				12288000

/** time out for HSE start up, milliseconds */
#define HSE_STARTUP_TIMEOUT					100

/** value of the internal oscillator, Hz */
#define HSI_VALUE							16000000

/** time out for LSE start up, milliseconds */
#define LSE_STARTUP_TIMEOUT					5000

/*---------------------------------------------------------------------------------------------------------------------+
| PHY-related global defines
+---------------------------------------------------------------------------------------------------------------------*/

/* delays and timeouts */

/** configuration delay of LAN8720A, milliseconds */
#define PHY_CONFIG_DELAY					500

/** read timeout for LAN8720A, milliseconds */
#define PHY_READ_TO							1000

/** reset delay of LAN8720A, milliseconds */
#define PHY_RESET_DELAY						100

/** write timeout for LAN8720A, milliseconds */
#define PHY_WRITE_TO						1000

/* PHY register indexes */

/** LAN8720A Basic Control Register index */
#define PHY_BCR								0

/** LAN8720A Basic Status Register index */
#define PHY_BSR								1

/** LAN8720A PHY Special Control/Status Register index */
#define PHY_SR								31

/* bits of PHY_BCR */

/** Soft Reset bit in LAN8720A Basic Control Register */
#define PHY_RESET							(1 << 15)

/** Auto-Negotiation Enable bit in LAN8720A Basic Control Register */
#define PHY_AUTONEGOTIATION					(1 << 12)

/* bits of PHY_BSR */

/** Auto-Negotiate Complete bit in LAN8720A Basic Status Register */
#define PHY_AUTONEGO_COMPLETE				(1 << 5)

/** Link Status bit in LAN8720A Basic Status Register */
#define PHY_LINKED_STATUS					(1 << 2)

/* bits of PHY_SR */

/** Full-duplex bit in Speed Indication field in LAN8720A PHY Special Control/Status Register */
#define PHY_DUPLEX_STATUS					(1 << 4)

/** 100BASE-TX bit in Speed Indication field in LAN8720A PHY Special Control/Status Register */
#define PHY_SPEED_STATUS					(1 << 2)

#endif	/* STM32F7XX_HAL_DRIVER_CONFIGURATION_H_ */
