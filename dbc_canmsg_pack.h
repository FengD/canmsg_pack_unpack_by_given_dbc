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


// the dbc unpack used to transform the value to can msg

void packCanmsg (const message &m, const int &valueSize, const float *value, unsigned char *data);

void packSignal (const signal &s, const double &value, unsigned char *data);

#endif //_DBC_CANMSG_PACK_H_
