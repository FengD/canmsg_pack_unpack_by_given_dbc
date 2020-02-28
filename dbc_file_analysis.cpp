#include "dbc_file_analysis.h"
#include <algorithm>
#include "util.h"

namespace dbc_analysis {

pthread_mutex_t DbcAnalysis::mutex_;

DbcAnalysis* DbcAnalysis::getInstance() {
  pthread_mutex_lock(&mutex_);
  static DbcAnalysis obj;
  pthread_mutex_unlock(&mutex_);
  return &obj;
}

DbcAnalysis::DbcAnalysis() { }

DbcAnalysis::~DbcAnalysis() { }

DbcAnalysis::DbcAnalysis(const DbcAnalysis&) { }

DbcAnalysis& DbcAnalysis::operator=(const DbcAnalysis&) {
  return *this;
}

void DbcAnalysis::transformMessageFromLine(std::string line, Message &m) {
   // remove the ":" in_ the line
  line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
  std::vector<std::string> strSplited;
  split(line, " ", &strSplited);
  int counter = 0;
  for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, ++info) {
    switch(counter) {
       // get the id of the msg
      case 1:
        m.id = atol(info->c_str());
        break;
       // get the name
      case 2:
        m.name = *info;
        break;
       // get the length
      case 3:
        m.length = atoi(info->c_str());
        break;
    }
  }
}

void DbcAnalysis::transformSignalFromLine(std::string line, Message &m) {
  Signal s;
   // remove the ":" in_ the line
  line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
  std::vector<std::string> strSplited;
  split(line, " ", &strSplited);
  int counter = 0;
  for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, ++info) {
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

void DbcAnalysis::getPosInfoTypeUnsignedFromStr(std::string str, Signal &s) {
  int plusIndex = str.find("+");
  int strLength = str.size();
  if(plusIndex > 0 && plusIndex < strLength) {
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

void DbcAnalysis::getFactorOffsetFromStr(std::string str, Signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '('), str.end());
  str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
  std::vector<std::string> strSplited;
  split(str, ",", &strSplited);
  s.factor = atof(strSplited.front().c_str());
  s.offset = atof(strSplited.back().c_str());
}

void DbcAnalysis::getMaxMinFromStr(std::string str, Signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '['), str.end());
  str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
  std::vector<std::string> strSplited;
  split(str, "|", &strSplited);
  s.maximum = atof(strSplited.back().c_str());
  s.minimum = atof(strSplited.front().c_str());
}

void DbcAnalysis::getUnitFromStr(std::string str, Signal &s) {
  str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
  if(str.find("NA") != 0) {
    s.unit = str;
  }
}

void DbcAnalysis::analysisFiles() {
  if (files_.size() == 0) {
    printf("No file given!\n");
    return;
  }
  for (std::string &filename : files_) {
    in_.open(filename.c_str());
    std::string line;
    if (in_) {
      while (getline (in_, line)) {
        analysisMessage(line);
      }
    } else {
      printf("No file named %s\n", filename.c_str());
    }
  }
}

void DbcAnalysis::analysisMessage(std::string line) {
  if (line.find( MSSAGEHEAD ) == 0) {
    Message newMessage;
    transformMessageFromLine(line, newMessage);
    while(getline (in_, line)){
      if (line.find( SIGNALHEAD ) == 1) {
        transformSignalFromLine(line, newMessage);
        continue;
      } else {
        analysisMessage(line);
      }
      sort(newMessage.signals.begin(), newMessage.signals.end());
      break;
    }

    messages_.insert(std::map<long, Message>::value_type (newMessage.id, newMessage));
  }
}

void DbcAnalysis::addOneDbcFile(const std::string &filePath) {
  files_.push_back(filePath);
}

std::map<long, Message>& DbcAnalysis::getMessages() {
  return messages_;
}

void DbcAnalysis::printMessages() {
  std::cout << "structure: " << std::endl;
  std::cout << "nb of messages_: " << messages_.size() << std::endl;
  for(std::map<long, Message>::const_iterator m = messages_.begin(); m != messages_.end(); ++m) {
    std::cout << "  id: " << m->first << std::endl;
    std::cout << "  name: " << m->second.name << std::endl;
    std::cout << "  length: " << m->second.length << std::endl;
    std::cout << "  nb of signals: " << m->second.signals.size() << std::endl;
    for(std::vector<Signal>::const_iterator s = m->second.signals.begin(); s != m->second.signals.end(); ++s) {
      std::cout << "    name: " << s->name << std::endl;
      std::cout << "    (startBit, length): (" << s->startBit << ", " << s->length << ")" << std::endl;
      std::cout << "    (factor, offset): (" << s->factor << ", " << s->offset << ")" << std::endl;
      std::cout << "    (minimum, maximum): (" << s->minimum << ", " << s->maximum << ")" << std::endl;
       // 关于起始位置，和Intel格式或者是Motorola格式是有关的，如果是Intel格式，起始位通常是0
      std::cout << "    dataType: " << s->dataType << " (motolora: 0 <BIGENDIAN>, intel: 1 <LITTLEENDIAN>)" << std::endl;
      std::cout << "    unsigned: " << s->is_unsigned << " (unsigned: 1, signed: 0)" << std::endl;
      std::cout << "    unit: " << s->unit << std::endl;
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
}

}  // namespace dbc_analysis
