#
# file: CMakeLists.txt
#
# author: Copyright (C) 2019-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
target_include_directories(distortos INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/board/include)

#-----------------------------------------------------------------------------------------------------------------------
# STM32F7xx_HAL_Driver library
#-----------------------------------------------------------------------------------------------------------------------

set(STM32F7XX_HAL_DRIVER_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/STM32F7xx_HAL_Driver")
set(STM32F7XX_HAL_DRIVER_CONFIGURATION_H_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
add_subdirectory(STM32F7xx_HAL_Driver-integration)

#-----------------------------------------------------------------------------------------------------------------------
# lwIP library
#-----------------------------------------------------------------------------------------------------------------------

set(LWIP_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lwIP")
set(LWIP_CONFIGURATION_H_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(lwIP-integration)

#-----------------------------------------------------------------------------------------------------------------------
# STM32F7-ETH-LAN8720A-lwIP-MQTT application
#-----------------------------------------------------------------------------------------------------------------------

add_executable(STM32F7-ETH-LAN8720A-lwIP-MQTT
		ethernetInterfaceInitialize.cpp
		main.cpp)
target_compile_features(STM32F7-ETH-LAN8720A-lwIP-MQTT PRIVATE
		cxx_std_17)
target_link_libraries(STM32F7-ETH-LAN8720A-lwIP-MQTT PRIVATE
		distortos::distortos
		lwipallapps
		lwipcore
		STM32F7xx_HAL_Driver)
distortosTargetLinkerScripts(STM32F7-ETH-LAN8720A-lwIP-MQTT $ENV{DISTORTOS_LINKER_SCRIPT})

include(${CMAKE_CURRENT_LIST_DIR}/board/STM32F7-ETH-LAN8720A-lwIP-MQTT-sources.cmake)

distortosBin(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.bin)
distortosDmp(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.dmp)
distortosHex(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.hex)
distortosLss(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.lss)
distortosMap(STM32F7-ETH-LAN8720A-lwIP-MQTT STM32F7-ETH-LAN8720A-lwIP-MQTT.map)
distortosSize(STM32F7-ETH-LAN8720A-lwIP-MQTT)
