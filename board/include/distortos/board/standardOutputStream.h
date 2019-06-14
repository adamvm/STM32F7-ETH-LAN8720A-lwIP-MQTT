/**
 * \file
 * \brief Declaration of standardOutputStream object
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BOARD_INCLUDE_DISTORTOS_BOARD_STANDARDOUTPUTSTREAM_H_
#define BOARD_INCLUDE_DISTORTOS_BOARD_STANDARDOUTPUTSTREAM_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/** pointer to standard output stream object */
extern FILE* standardOutputStream;

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* BOARD_INCLUDE_DISTORTOS_BOARD_STANDARDOUTPUTSTREAM_H_ */
