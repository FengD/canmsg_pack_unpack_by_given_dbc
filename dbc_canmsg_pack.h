#ifndef _DBC_CANMSG_PACK_H_
#define _DBC_CANMSG_PACK_H_ 1

#include <map>
#include <string>
#include <string.h>
#include "struct_define.h"


// the dbc unpack used to transform the value to can msg


can_msg unpackCanmsg (message m, message_value mv);

#endif //_DBC_CANMSG_PACK_H_
