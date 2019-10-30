#ifndef _STRUCT_DEFINE_H_
#define _STRUCT_DEFINE_H_ 1

#include <vector>
#include <map>
#include <algorithm>
#include <functional>


struct signal {
	std::string name;
	int start_position;
	int length;
	float factor;
	float offset;
	float max_value;
	float min_value;
	int type;
	int is_unsigned;
	std::string unit;

	bool operator <(const signal& s) const // 升序排序时必须写的函数
  {
      return start_position < s.start_position;
  }
  bool operator >(const signal& s) const // 降序排序时必须写的函数
  {
      return start_position > s.start_position;
  }
};

struct message {
	long id;
	std::string name;
	int length;
	std::vector<signal> signals;

	bool operator <(const message& msg) const // 升序排序时必须写的函数
  {
      return id < msg.id;
  }
  bool operator >(const message& msg) const // 降序排序时必须写的函数
  {
      return id > msg.id;
  }
};

struct message_value {
  long id;
  std::map<std::string, double> signals;
};

struct can_msg {
  long id;
  uint8_t  length;
  uint8_t  data[8];
};

#endif //_STRUCT_DEFINE_H_
