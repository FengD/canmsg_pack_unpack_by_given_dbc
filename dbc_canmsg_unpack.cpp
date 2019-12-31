/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#include "dbc_canmsg_unpack.h"
#include <math.h>

#define BITCALCULATEUNPACK(type) \
tempValue = tempValue | (type)((type)((type)((data[startIndex]) & (type) ((type)\
                        (1)<< shift)) >> shift)<<i);

#define UNPACKVALUE(unsignedType, type) \
type unpackedValue = 0;\
unsignedType tempValue = (unsignedType) (0);\
if (s.dataType) {\
  for (int i = 0; i < s.length; i++) {\
    BITCALCULATEUNPACK(unsignedType);\
    shift++;\
    if (shift == 8) {\
      shift = 0;\
      startIndex++;\
    }\
  }\
} else {\
  for (int i = 0; i < s.length; i++) {\
    BITCALCULATEUNPACK(unsignedType);\
    shift++;\
    if (shift == 8) {\
      shift = 0;\
      startIndex--;\
    }\
  }\
}\
unsignedType mask = (unsignedType)(1);\
unsignedType bitValue = tempValue & (unsignedType) (mask << (s.length - 1));\
if (bitValue != 0 && !s.is_unsigned) {\
  int len = 8 - s.length % 8;\
  int tmp = (s.length / 8 + 1) * 8;\
  for (int i = 0; i < len; i++) {\
    tempValue = tempValue | (unsignedType) (mask << (tmp - i - 1));\
  }\
}\
unpackedValue = (type) tempValue;\
outValue = (real64_T) (unpackedValue);

namespace can_util {

int unpackCanmsg (const Message &m, const Canmsg &msg, const size_t valueSize, double *value) {
  if (valueSize != m.signals.size()) {
    printf("value given error\n");
    return SIGNAL_SIZE_MISMATCH;
  }
  // double check the id and the length
  if (m.id != msg.id) {
    printf("canmsg id mismatch, %ld need but %ld given\n", m.id, msg.id);
    return UNPACK_ID_MISMATCH;
  }

  if (m.length != msg.length) {
    printf("canmsg length mismatch, %d need but %d given\n", m.length, msg.length);
    return UNPACK_LENGTH_MISMATCH;
  }

  int index = 0;
  std::for_each(m.signals.begin(), m.signals.end(), [&](Signal s){
    value[index] = unpackSignal(s, msg.data);
    index++;
  });
  return PACK_UNPACK_SUCCESS;
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
      UNPACKVALUE(uint8_T, uint8_T);
    } else if (s.length > 8 && s.length <= 16) {
      UNPACKVALUE(uint16_T, uint16_T);
    } else if (s.length > 16 && s.length <= 32) {
      UNPACKVALUE(uint32_T, uint32_T);
    } else if (s.length > 32) {
      UNPACKVALUE(uint64_T, uint64_T);
    }
  } else {
    max = (bitValue / 2.0 - 1.0);
    min = (-1.0) * max - 1.0;
    max = max * s.factor + s.offset;
    min = min * s.factor + s.offset;
    if (s.length <= 8) {
      UNPACKVALUE(uint8_T, int8_T);
    } else if (s.length > 8 && s.length <= 16) {
      UNPACKVALUE(uint16_T, int16_T);
    } else if (s.length > 16 && s.length <= 32) {
      UNPACKVALUE(uint32_T, int32_T);
    } else if (s.length > 32) {
      UNPACKVALUE(uint64_T, int64_T);
    }
  }

  real64_T result = (real64_T) outValue;
  result = (result * s.factor) + s.offset;
  result = result < min ? min : result;
  result = result > max ? max : result;

  return result;
}
} // namespace can_util
