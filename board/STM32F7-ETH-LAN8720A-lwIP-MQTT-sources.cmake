#
# file: STM32F7-ETH-LAN8720A-lwIP-MQTT-sources.cmake
#
# author: Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(STM32F7-ETH-LAN8720A-lwIP-MQTT PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/initializeStreams.cpp)
