/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#ifndef _DBC_CANMSG_PACK_H_
#define _DBC_CANMSG_PACK_H_

#include <map>
#include <string>
#include <string.h>
#include "struct_define.h"

/** the dbc pack used to transform the value to can msg
* @param in [struct Message] the message that you want to pack
* @param in [unsigned int] the size of the value list
* @param in [float array] the array of the values
* @param out [struct Canmsg] the canmsg
*/
void packCanmsg (const Message &m, const size_t &valueSize, const float *value, Canmsg *msg);

/**
* pack one Signal
* @param in [struct Signal] the Signal that you want to pack
* @param in [double] the value that you want to pack in the Signal
* @param out [unsigned char array] the data array of the can msg
*/
void packSignal (const Signal &s, const double &value, uint8_T *data);

#endif //_DBC_CANMSG_PACK_H_
