/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#include "dbc_canmsg_unpack.h"
#include <iostream>

#define BITCALCULATEUNPACK \
tempValue = tempValue | ((((data[startIndex]) & ( (uint8_T)\
                        (1)<< shift)) >> shift)<<i);

#define UNPACKVALUE(type) \
type unpackedValue = 0;\
type tempValue = (type) (0);\
if (s.dataType) {\
  for (int i = 0; i < s.length; i++) {\
    BITCALCULATEUNPACK;\
    shift++;\
    if (shift == 8) {\
      shift = 0;\
      startIndex++;\
    }\
  }\
} else {\
  for (int i = 0; i < s.length; i++) {\
    BITCALCULATEUNPACK;\
    shift++;\
    if (shift == 8) {\
      shift = 0;\
      startIndex--;\
    }\
  }\
}\
unpackedValue = tempValue;\
outValue = (real64_T) (unpackedValue);

namespace can_util {

void unpackCanmsg (const Message &m, const Canmsg &msg, const size_t valueSize, double *value) {
  // if the message has the correct number of signals
  if (valueSize != m.signals.size()) {
    printf("value given error\n");
    return;
  }
  // double check the id and the length
  if (m.id != msg.id || m.length != msg.length) {
    printf("canmsg length or id error\n");
    return;
  }

  int index = 0;
  // unpack the signals
  for (std::vector<Signal>::const_iterator s = m.signals.begin(); s != m.signals.end(); s++) {
    value[index] = unpackSignal(*s, msg.data);
    index++;
  }
}

real64_T unpackSignal (const Signal &s, const uint8_T *data) {
  // --------------- START Unpacking Signal ------------------
  //   startBit                = s.startBit
  //   length                  = s.length
  //   desiredSignalByteLayout = s.dataType
  //   dataType                = s.is_unsigned
  //   factor                  = s.factor
  //   offset                  = s.offset
  //  -----------------------------------------------------------------------

  int startBit = s.startBit;
  {
    // if the motolora type <BEGENDIAN> the startbit needs to be recalculated
    if (!s.dataType) {
      int tmp1 = startBit / 8;
      int tmp2 = tmp1 * 8 + 7 - (startBit % 8) + s.length - 1;
      int tmp3 = tmp2 / 8;
      startBit = tmp3 * 8 + 7 - tmp2 % 8;
    }
  }

  int startIndex = startBit / 8;
  int shift = startBit % 8;

  real64_T outValue = 0;
  long bitValue = pow(2, s.length);
  double max, min;
  if (s.is_unsigned) {
    max = (bitValue - 1) * s.factor + s.offset;
    min = 0.0 * s.factor + s.offset;
    if (s.length <= 8) {
      UNPACKVALUE(uint8_T);
    } else if (s.length > 8 && s.length <= 16) {
      UNPACKVALUE(uint16_T);
    } else if (s.length > 16 && s.length <= 32) {
      UNPACKVALUE(uint32_T);
    } else if (s.length > 32) {
      UNPACKVALUE(uint64_T);
    }
  } else {
    max = (bitValue / 2.0 - 1.0);
    min = (-1.0) * max - 1.0;
    max = max * s.factor + s.offset;
    min = min * s.factor + s.offset;
    if (s.length <= 8) {
      UNPACKVALUE(int8_T);
    } else if (s.length > 8 && s.length <= 16) {
      UNPACKVALUE(int16_T);
    } else if (s.length > 16 && s.length <= 32) {
      UNPACKVALUE(int32_T);
    } else if (s.length > 32) {
      UNPACKVALUE(int64_T);
    }
  }

  // TODO mask for the signed value

  real64_T result = (real64_T) outValue;
  result = (result * s.factor) + s.offset;
  if (result < min) {
    result = min;
  }

  if (result > max) {
    result = max;
  }

  return result;
}
} // namespace can_util
