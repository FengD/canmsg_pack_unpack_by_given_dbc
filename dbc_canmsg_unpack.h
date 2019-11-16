/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#ifndef _DBC_CANMSG_UNPACK_H_
#define _DBC_CANMSG_UNPACK_H_

#include <map>
#include <string>
#include <string.h>
#include "struct_define.h"

namespace can_util {

/** the dbc unpack used to transform the can msg to value
* @param in [struct Message] the message that you want to unpack
* @param in [struct Canmsg] the canmsg
* @param in [unsigned int] the size of the value list
* @param out [double array] the array of the values
*/
void unpackCanmsg (const Message &m, const Canmsg &msg, const size_t valueSize, double *value);

/**
* pack one Signal
* @param in [struct Signal] the Signal that you want to unpack
* @param in [unsigned char array] the data array of the can msg
* @return [double] the value that you unpack from the Signal
*/
real64_T unpackSignal (const Signal &s, const uint8_T *data);

} // namespace can_util

#endif //_DBC_CANMSG_UNPACK_H_
