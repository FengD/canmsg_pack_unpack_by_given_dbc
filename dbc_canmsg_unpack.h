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


// the dbc unpack used to transform the can msg to value


void unpackCanmsg (const message &m, const can_msg &c);



#endif //_DBC_CANMSG_UNPACK_H_
