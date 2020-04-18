/**
 * \file
 * \brief ethernetInterfaceInitialize() definition
 *
 * \author Copyright (C) 2019-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ethernetInterfaceInitialize.hpp"

#include "stm32f7xx_hal.h"

#include "distortos/chip/PinInitializer.hpp"
#include "distortos/chip/uniqueDeviceId.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"
#include "distortos/DynamicThread.hpp"

#include "estd/ScopeGuard.hpp"

#include "netif/etharp.h"

#include <cstring>

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// name of interface
constexpr char interfaceName[] {"st"};

/// Ethernet Rx DMA descriptors
ETH_DMADescTypeDef dmaRxDscriptors[ETH_RXBUFNB] __attribute__ ((aligned(4)));

/// Ethernet Tx DMA descriptors
ETH_DMADescTypeDef dmaTxDescriptors[ETH_TXBUFNB] __attribute__ ((aligned(4)));

/// Ethernet receive buffers
uint8_t rxBuffers[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__ ((aligned(4)));

/// Ethernet transmit buffers
uint8_t txBuffers[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__ ((aligned(4)));

/// semaphore for communication between "Ethernet RX transfer completed" interrupt callback and Ethernet input thread
distortos::Semaphore ethernetInputSemaphore {1};

/// handle of Ethernet interface
ETH_HandleTypeDef ethernetHandle;

/// pin initializers for ETH
const distortos::chip::PinInitializer ethPinInitializers[]
{
#if defined(DISTORTOS_BOARD_ST_32F746GDISCOVERY)
		// RMII_REF_CLK
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa1,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MDIO
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa2,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_CRS_DV
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa7,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MDC
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc1,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_RXD0
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc4,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_RXD1
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc5,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TX_EN
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pg11,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TXD0
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pg13,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TXD1
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pg14,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
#elif defined(DISTORTOS_BOARD_ST_NUCLEO_F767ZI)
		// RMII_REF_CLK
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa1,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MDIO
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa2,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_CRS_DV
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pa7,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MDC
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc1,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_RXD0
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc4,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_RXD1
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pc5,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TX_EN
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pg11,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TXD0
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pg13,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
		// RMII_MII_TXD1
		distortos::chip::makeAlternateFunctionPinInitializer(distortos::chip::Pin::pb13,
				distortos::chip::PinAlternateFunction::af11,
				false,
				distortos::chip::PinOutputSpeed::veryHigh,
				distortos::chip::PinPull::none),
#else
#error "Unsupported board!"
#endif
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level initializer for ETH
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void ethLowLevelInitializer()
{
	for (auto& pinInitializer : ethPinInitializers)
		pinInitializer();

	NVIC_SetPriority(ETH_IRQn, DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI);
	NVIC_EnableIRQ(ETH_IRQn);

	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACRXEN | RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACEN;
}

BIND_LOW_LEVEL_INITIALIZER(60, ethLowLevelInitializer);

/**
 * \brief Low-lever Ethernet input function
 *
 * Should allocate a pbuf and transfer the bytes of the incoming packet from the interface into the pbuf.
 *
 * \return pbuf filled with the received packet (including MAC header), nullptr on memory error
 */

pbuf* lowLevelInput()
{
	if (HAL_ETH_GetReceivedFrame_IT(&ethernetHandle) != HAL_OK)
		return nullptr;

	const auto length = ethernetHandle.RxFrameInfos.length;
	pbuf* pbufChain {};
	if (length > 0)
		pbufChain = pbuf_alloc(PBUF_RAW, length, PBUF_POOL);

	{
		auto buffer = reinterpret_cast<uint8_t*>(ethernetHandle.RxFrameInfos.buffer);
		size_t bufferOffset {};
		auto dmaRxDescriptor = ethernetHandle.RxFrameInfos.FSRxDesc;

		for (auto pbuf = pbufChain; pbuf != nullptr; pbuf = pbuf->next)
		{
			size_t bytesLeft {pbuf->len};
			size_t payloadOffset {};

			while (bytesLeft + bufferOffset > ETH_RX_BUF_SIZE)
			{
				const auto chunk = ETH_RX_BUF_SIZE - bufferOffset;
				memcpy(static_cast<uint8_t*>(pbuf->payload) + payloadOffset,
						static_cast<const uint8_t*>(buffer) + bufferOffset, chunk);

				// advance to next descriptor
				dmaRxDescriptor = reinterpret_cast<ETH_DMADescTypeDef*>(dmaRxDescriptor->Buffer2NextDescAddr);
				buffer = reinterpret_cast<uint8_t*>(dmaRxDescriptor->Buffer1Addr);
				bufferOffset = {};
				bytesLeft -= chunk;
				payloadOffset += chunk;
			}

			memcpy(static_cast<uint8_t*>(pbuf->payload) + payloadOffset,
					static_cast<const uint8_t*>(buffer) + bufferOffset, bytesLeft);
			bufferOffset += bytesLeft;
		}
	}

	// release descriptors to DMA, go back to first descriptor
	auto dmaRxDescriptor = ethernetHandle.RxFrameInfos.FSRxDesc;
	// set "own" bit in rx descriptors - gives the buffers back to DMA
	for (uint32_t i {}; i < ethernetHandle.RxFrameInfos.SegCount; ++i)
	{
		dmaRxDescriptor->Status |= ETH_DMARXDESC_OWN;
		dmaRxDescriptor = reinterpret_cast<ETH_DMADescTypeDef*>(dmaRxDescriptor->Buffer2NextDescAddr);
	}

	ethernetHandle.RxFrameInfos.SegCount = {};	// clear Segment_Count

	// rx buffer unavailable flag is set?
	if ((ethernetHandle.Instance->DMASR & ETH_DMASR_RBUS) != 0)
	{
		ethernetHandle.Instance->DMASR = ETH_DMASR_RBUS;	// clear RBUS ETHERNET DMA flag
		ethernetHandle.Instance->DMARPDR = {};	// resume DMA reception
	}

	return pbufChain;
}

/**
 * \brief Low-lever Ethernet output function
 *
 * This function should do the actual transmission of the packet. The packet is contained in the pbuf that is passed to
 * the function. This pbuf might be chained.
 *
 * \note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to strange results. You might consider
 * waiting for space in the DMA queue to become available since the stack doesn't retry to send a packet dropped because
 * of memory failure (except for the TCP timers).
 *
 * \param [in] netif is the lwIP network interface structure for this Ethernet interface
 * \param [in] pbuf is the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *
 * \return ERR_OK if the packet could be sent, an err_t value if the packet couldn't be sent
 */

err_t lowLevelOutput(netif*, pbuf* pbuf)
{
	const auto scopeGuard = estd::makeScopeGuard(
			[]()
			{
				// transmit underflow flag is set?
				if ((ethernetHandle.Instance->DMASR & ETH_DMASR_TUS) != 0)
				{
					ethernetHandle.Instance->DMASR = ETH_DMASR_TUS;	// clear TUS ETHERNET DMA flag
					ethernetHandle.Instance->DMATPDR = {};	// resume DMA transmission
				}
			});

	auto buffer = reinterpret_cast<uint8_t*>(ethernetHandle.TxDesc->Buffer1Addr);
	auto dmaTxDescriptor = ethernetHandle.TxDesc;
	size_t frameLength {};
	size_t bufferOffset {};

	for (; pbuf != nullptr; pbuf = pbuf->next)
	{
		if ((dmaTxDescriptor->Status & ETH_DMATXDESC_OWN) != 0)	// buffer unavailable?
			return ERR_USE;

		size_t bytesLeft {pbuf->len};
		size_t payloadOffset {};

		while (bytesLeft + bufferOffset > ETH_TX_BUF_SIZE)
		{
			const auto chunk = ETH_TX_BUF_SIZE - bufferOffset;
			memcpy(static_cast<uint8_t*>(buffer) + bufferOffset,
					static_cast<const uint8_t*>(pbuf->payload) + payloadOffset, chunk);

			// advance to next descriptor
			dmaTxDescriptor = reinterpret_cast<ETH_DMADescTypeDef*>(dmaTxDescriptor->Buffer2NextDescAddr);

			if ((dmaTxDescriptor->Status & ETH_DMATXDESC_OWN) != 0)	// buffer unavailable?
				return ERR_USE;

			buffer = reinterpret_cast<uint8_t*>(dmaTxDescriptor->Buffer1Addr);
			bufferOffset = {};
			bytesLeft -= chunk;
			payloadOffset += chunk;
			frameLength += chunk;
		}

		memcpy(static_cast<uint8_t*>(buffer) + bufferOffset, static_cast<const uint8_t*>(pbuf->payload) + payloadOffset,
				bytesLeft);
		bufferOffset += bytesLeft;
		frameLength += bytesLeft;
	}

	HAL_ETH_TransmitFrame(&ethernetHandle, frameLength);

	return ERR_OK;
}

/**
 * \brief Ethernet input thread
 *
 * This function is the Ethernet interface input thread, it is processed when a packet is ready to be read from the
 * interface. It uses the function lowLevelInput() that should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and the appropriate input function is called.
 *
 * \param [in] netif is a reference to the lwIP network interface structure for this Ethernet interface
 */

void ethernetInterfaceInput(netif& netif)
{
	while (1)
	{
		const auto tryWaitForRet = ethernetInputSemaphore.tryWaitFor(std::chrono::seconds{1});

		LOCK_TCPIP_CORE();
		const auto unlockScopeGuard = estd::makeScopeGuard(
				[]()
				{
					UNLOCK_TCPIP_CORE();
				});

		if (tryWaitForRet == 0)
		{
			pbuf* pbuf;
			while (pbuf = lowLevelInput(), pbuf != nullptr)
				if (netif.input(pbuf, &netif) != ERR_OK)
					pbuf_free(pbuf);
		}
		else if (tryWaitForRet == ETIMEDOUT)
		{
			uint32_t basicStatus;
			{
				const auto ret = HAL_ETH_ReadPHYRegister(&ethernetHandle, PHY_BSR, &basicStatus);
				assert(ret == HAL_OK);
			}
			constexpr uint32_t linkMask {PHY_LINKED_STATUS | PHY_AUTONEGO_COMPLETE};
			const auto linkStatus = (basicStatus & linkMask) == linkMask;
			const auto previousLinkStatus = netif_is_link_up(&netif) != 0;
			if (linkStatus != previousLinkStatus)
			{
				if (linkStatus == true)
				{
					uint32_t phySpecialControlStatus;
					{
						const auto ret = HAL_ETH_ReadPHYRegister(&ethernetHandle, PHY_SR, &phySpecialControlStatus);
						assert(ret == HAL_OK);
					}

					ethernetHandle.Init.Speed = (phySpecialControlStatus & PHY_SPEED_STATUS) == 0 ?
							ETH_SPEED_100M : ETH_SPEED_10M;
					ethernetHandle.Init.DuplexMode = (phySpecialControlStatus & PHY_DUPLEX_STATUS) != 0 ?
							ETH_MODE_FULLDUPLEX : ETH_MODE_HALFDUPLEX;

					{
						const auto ret = HAL_ETH_ConfigMAC(&ethernetHandle, nullptr);
						assert(ret == HAL_OK);
					}

					netif_set_link_up(&netif);
				}
				else
					netif_set_link_down(&netif);
			}
		}
	}
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief ETH interrupt handler
 */

extern "C" void ETH_IRQHandler()
{
	HAL_ETH_IRQHandler(&ethernetHandle);
}

err_t ethernetInterfaceInitialize(netif* const netif)
{
	netif->name[0] = interfaceName[0];
	netif->name[1] = interfaceName[1];
	netif->output = etharp_output;
	netif->linkoutput = lowLevelOutput;

	// set netif MAC hardware address
	netif->hwaddr_len = std::size(netif->hwaddr);
	{
		const auto& uniqueDeviceId = *distortos::chip::uniqueDeviceId;
		static_assert(std::size(std::decay_t<decltype(uniqueDeviceId)>{}.uint8) == 2 *
				std::size(std::decay_t<decltype(*netif)>{}.hwaddr));
		for (size_t i {}; i < netif->hwaddr_len; ++i)
			netif->hwaddr[i] = uniqueDeviceId.uint8[i] ^ uniqueDeviceId.uint8[i + 6];

		// make sure the address is locally administered and unicast
		// https://en.wikipedia.org/wiki/MAC_address#Address_details
		netif->hwaddr[0] = 0x2 | ((netif->hwaddr[0] << 2) ^ (netif->hwaddr[0] & 0xc0));
	}

	netif->mtu = 1500;	// set netif maximum transfer unit
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;	// accept broadcast address and ARP traffic

	ethernetHandle.Instance = ETH;
	ethernetHandle.Init.MACAddr = netif->hwaddr;
	ethernetHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_DISABLE;
	ethernetHandle.Init.Speed = ETH_SPEED_100M;
	ethernetHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
	ethernetHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
	ethernetHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
	ethernetHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
	ethernetHandle.Init.PhyAddress = 0;

	HAL_ETH_Init(&ethernetHandle);	/// \todo error handling?

	{
		constexpr uint16_t autoNegotiationAdvertisementRegister {4};
		constexpr uint16_t advertise100BaseTxFullDuplex {1 << 8};
		constexpr uint16_t advertise100BaseTx {1 << 7};
		constexpr uint16_t advertise10BaseTFullDuplex {1 << 6};
		constexpr uint16_t advertise10BaseT {1 << 5};
		constexpr uint16_t advertiseAll {advertise100BaseTxFullDuplex | advertise100BaseTx |
				advertise10BaseTFullDuplex | advertise10BaseT};

		uint32_t autoNegotiationAdvertisement;
		{
			const auto ret = HAL_ETH_ReadPHYRegister(&ethernetHandle, autoNegotiationAdvertisementRegister,
					&autoNegotiationAdvertisement);
			assert(ret == HAL_OK);
		}
		{
			const auto ret = HAL_ETH_WritePHYRegister(&ethernetHandle, autoNegotiationAdvertisementRegister,
					autoNegotiationAdvertisement | advertiseAll);
			assert(ret == HAL_OK);
		}
	}
	{
		uint32_t basicControl;
		{
			const auto ret = HAL_ETH_ReadPHYRegister(&ethernetHandle, PHY_BCR, &basicControl);
			assert(ret == HAL_OK);
		}
		{
			const auto ret = HAL_ETH_WritePHYRegister(&ethernetHandle, PHY_BCR, basicControl | PHY_AUTONEGOTIATION);
			assert(ret == HAL_OK);
		}
	}

	/// \todo error handling?
	HAL_ETH_DMARxDescListInit(&ethernetHandle, dmaRxDscriptors, &rxBuffers[0][0], ETH_RXBUFNB);
	/// \todo error handling?
	HAL_ETH_DMATxDescListInit(&ethernetHandle, dmaTxDescriptors, &txBuffers[0][0], ETH_TXBUFNB);

#ifndef LWIP_DEBUG
	constexpr size_t stackSize {1024};
#else	// def LWIP_DEBUG
	constexpr size_t stackSize {2048};
#endif	// def LWIP_DEBUG

	// create, start and detach the thread that handles Ethernet input
	distortos::makeAndStartDynamicThread({stackSize, 1}, ethernetInterfaceInput, std::ref(*netif)).detach();

	/// \todo error handling?
	HAL_ETH_Start(&ethernetHandle);

	return ERR_OK;
}

/**
 * \brief "Ethernet RX transfer completed" interrupt callback
 *
 * Posts the semaphore to wake Ethernet input thread.
 */

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef*)
{
	ethernetInputSemaphore.post();
}
