/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#include "dbc_canmsg_unpack.h"

void unpackCanmsg (const message &m, const can_msg &c) {
  for(std::vector<signal>::const_iterator s = m.signals.begin(); s != m.signals.end(); s++) {
    // double v = getSignalValue(s->factor, s->offset, s->type, s->start_position, s->length, s->is_unsigned, c, m.length);
    //
    // mv.signals.insert(std::map<std::string, double>::value_type (s->name, v));
  }
}
