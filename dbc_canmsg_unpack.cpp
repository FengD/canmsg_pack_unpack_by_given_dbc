#include "dbc_canmsg_unpack.h"
#include "util.h"

message_value unpackCanmsg (message m, can_msg c) {
  message_value mv;
  mv.id = m.id;

  for(std::vector<signal>::const_iterator s = m.signals.begin(); s != m.signals.end(); s++) {
    // double v = getSignalValue(s->factor, s->offset, s->type, s->start_position, s->length, s->is_unsigned, c, m.length);
    //
    // mv.signals.insert(std::map<std::string, double>::value_type (s->name, v));
  }

  return mv;
}
