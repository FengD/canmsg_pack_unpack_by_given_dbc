#include "dbc_file_analysis.h"
#include "util.h"
#include <algorithm>

namespace dbc_analysis{

DbcAnalysis::DbcAnalysis() {}

DbcAnalysis::~DbcAnalysis() {}

void DbcAnalysis::messageLineTransform(std::string line, message &m) {
  line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
  std::vector<std::string> strSplited;
  split(line, " ", &strSplited);
  int counter = 0;
  for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, info++) {
    switch(counter) {
      case 1:
        m.id = atol(info->c_str());
        break;
      case 2:
        m.name = *info;
        break;
      case 3:
        m.length = atoi(info->c_str());
        break;
    }
  }
}

void DbcAnalysis::signalLineTransform(std::string line, message &m) {
  signal s;
  line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
  std::vector<std::string> strSplited;
  split(line, " ", &strSplited);
  int counter = 0;
  for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, info++) {
    switch(counter) {
      case 1:
        s.name = *info;
        break;
      case 2:
        getPosInfoTypeUnsignedFromStr(*info, s);
        break;
      case 3:
        getFactorOffsetFromStr(*info, s);
        break;
      case 4:
        getMaxMinFromStr(*info, s);
        break;
      case 5:
        getUnitFromStr(*info, s);
        break;
    }
  }
  m.signals.push_back(s);
}

void DbcAnalysis::getPosInfoTypeUnsignedFromStr(std::string str, signal &s) {
  if(str.find("+") > 0) {
    s.is_unsigned = 1;
    str.erase(std::remove(str.begin(), str.end(), '+'), str.end());
  } else {
    s.is_unsigned = 0;
    str.erase(std::remove(str.begin(), str.end(), '-'), str.end());
  }
  str.replace(str.find("|"), 1, " ");
  str.replace(str.find("@"), 1, " ");
  std::vector<std::string> strSplited;
  split(str, " ", &strSplited);
  s.startBit = atoi(strSplited.front().c_str());
  s.length = atoi(strSplited.at(1).c_str());
  s.dataType = atoi(strSplited.back().c_str());
}

void DbcAnalysis::getFactorOffsetFromStr(std::string str, signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '('), str.end());
  str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
  std::vector<std::string> strSplited;
  split(str, ",", &strSplited);
  s.factor = atof(strSplited.front().c_str());
  s.offset = atof(strSplited.back().c_str());
}

void DbcAnalysis::getMaxMinFromStr(std::string str, signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '['), str.end());
  str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
  std::vector<std::string> strSplited;
  split(str, "|", &strSplited);
  s.maximum = atof(strSplited.back().c_str());
  s.minimum = atof(strSplited.front().c_str());
}

void DbcAnalysis::getUnitFromStr(std::string str, signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
  if(str.find("NA") != 0) {
    s.unit = str;
  }
}

void DbcAnalysis::fileAnalysis() {
  for (std::string &filename : files_) {
    std::ifstream in(filename.c_str());
    std::string line;
    if(in) {
      while (getline (in, line)) {
    		if(line.find( MSSAGEHEAD ) == 0){
          message newMessage;
          messageLineTransform(line, newMessage);
          while(getline (in, line)){
            if(line.find( SIGNALHEAD ) == 1){
              signalLineTransform(line, newMessage);
              continue;
            }
            sort(newMessage.signals.begin(), newMessage.signals.end());
            break;
          }
          // if(newMessage.id < 4096) {
          messages_.insert(std::map<long, message>::value_type (newMessage.id, newMessage));
          // }
        }
      }
    } else {
      std::cout << "no file named " << filename << std::endl;
    }
  }
}

void DbcAnalysis::addOneDbcFile(const std::string &filePath) {
  files_.push_back(filePath);
}

std::map<long, message> DbcAnalysis::getMessages() {
  return messages_;
}

void DbcAnalysis::printMessages() {
  std::cout << "structure: " << std::endl;
  std::cout << "nb of messages_: " << messages_.size() << std::endl;
  for(std::map<long, message>::const_iterator m = messages_.begin(); m != messages_.end(); m++) {
    std::cout << "  id: " << m->first << std::endl;
    std::cout << "  name: " << m->second.name << std::endl;
    std::cout << "  length: " << m->second.length << std::endl;
    std::cout << "  nb of signals: " << m->second.signals.size() << std::endl;
    for(std::vector<signal>::const_iterator s = m->second.signals.begin(); s != m->second.signals.end(); s++) {
      std::cout << "    name: " << s->name << std::endl;
      std::cout << "    (startBit, length): (" << s->startBit << ", " << s->length << ")" << std::endl;
      std::cout << "    (factor, offset): (" << s->factor << ", " << s->offset << ")" << std::endl;
      std::cout << "    (minimum, maximum): (" << s->minimum << ", " << s->maximum << ")" << std::endl;
      // 关于起始位置，和Intel格式或者是Motorola格式是有关的，如果是Intel格式，起始位通常是0
      std::cout << "    dataType: " << s->dataType << " (motolora: 0 <LITTLEENDIAN>, intel: 1 <BIGENDIAN>)" << std::endl;
      std::cout << "    unsigned: " << s->is_unsigned << " (unsigned: 1, signed: 0)" << std::endl;
      std::cout << "    unit: " << s->unit << std::endl;
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
}

}
