#
# file: CMakeLists.txt
#
# author: Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.8)
project(STM32F7-ETH-LAN8720A-lwIP-MQTT)

#-----------------------------------------------------------------------------------------------------------------------
# distortos library
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(distortos)

#-----------------------------------------------------------------------------------------------------------------------
# STM32F7-ETH-LAN8720A-lwIP-MQTT application
#-----------------------------------------------------------------------------------------------------------------------

add_executable(STM32F7-ETH-LAN8720A-lwIP-MQTT
		main.cpp)
target_compile_features(STM32F7-ETH-LAN8720A-lwIP-MQTT PRIVATE
		cxx_std_17)
target_link_libraries(STM32F7-ETH-LAN8720A-lwIP-MQTT PRIVATE
		distortos::distortos)
distortosTargetLinkerScripts(STM32F7-ETH-LAN8720A-lwIP-MQTT $ENV{DISTORTOS_LINKER_SCRIPT})

distortosBin(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.bin)
distortosDmp(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.dmp)
distortosHex(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.hex)
distortosLss(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.lss)
distortosMap(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.map)
distortosSize(STM32F7-ETH-LAN8720A-lwIP-MQTT)
