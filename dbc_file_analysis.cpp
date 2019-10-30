#include "dbc_file_analysis.h"
#include "util.h"
#include <algorithm>

namespace dbc_analysis{

  DbcAnalysis::DbcAnalysis() {

  }

  DbcAnalysis::DbcAnalysis(std::string fn) : filename(fn) {

  }

  DbcAnalysis::~DbcAnalysis() {

  }

  void DbcAnalysis::messageLineTransform(std::string line, message &m) {
    line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
    std::vector<std::string> strSplited;
    split(line, " ", strSplited);
    int counter = 0;
    for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, info++) {
      // std::cout << counter << " " <<  *info << std::endl;
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
    split(line, " ", strSplited);
    int counter = 0;
    for(std::vector<std::string>::iterator info = strSplited.begin(); info != strSplited.end(); ++counter, info++) {
      // std::cout << counter << " " <<  *info << std::endl;
      switch(counter) {
        case 2:
          s.name = *info;
          break;
        case 3:
          getPosInfoTypeUnsignedFromStr(*info, s);
          break;
        case 4:
          getFactorOffsetFromStr(*info, s);
          break;
        case 5:
          getMaxMinFromStr(*info, s);
          break;
        case 6:
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
    split(str, " ", strSplited);
    // std::cout << str << std::endl;
    s.start_position = atoi(strSplited.front().c_str());
    s.length = atoi(strSplited.at(1).c_str());
    s.type = atoi(strSplited.back().c_str());
  }

  void DbcAnalysis::getFactorOffsetFromStr(std::string str, signal &s) {
    str.erase(std::remove(str.begin(), str.end(), '('), str.end());
    str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
    std::vector<std::string> strSplited;
    split(str, ",", strSplited);
    // std::cout << str << std::endl;
    s.factor = atof(strSplited.front().c_str());
    s.offset = atof(strSplited.back().c_str());
  }

  void DbcAnalysis::getMaxMinFromStr(std::string str, signal &s) {
    str.erase(std::remove(str.begin(), str.end(), '['), str.end());
    str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
    std::vector<std::string> strSplited;
    split(str, "|", strSplited);
    // std::cout << str << std::endl;
    s.max_value = atof(strSplited.back().c_str());
    s.min_value = atof(strSplited.front().c_str());
  }

  void DbcAnalysis::getUnitFromStr(std::string str, signal &s) {
    str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
    // std::cout << str << std::endl;
    if(str.find("NA") != 0) {
      s.unit = str;
    }
  }

  void DbcAnalysis::fileAnalysis() {
    std::ifstream in(filename.c_str());
    std::string line;
    if(in) {
      while (getline (in, line)) {
    		if(line.find( MSSAGEHEAD ) == 0){
          message newMessage;
    			// std::cout << line << std::endl;
          messageLineTransform(line, newMessage);
          while(getline (in, line)){
            if(line.find( SIGNALHEAD ) == 1){
              // std::cout << line << std::endl;
              signalLineTransform(line, newMessage);
              continue;
            }
            sort(newMessage.signals.begin(), newMessage.signals.end());
            break;
          }
          if(newMessage.id < 4096) {
            messages.insert(std::map<long, message>::value_type (newMessage.id, newMessage));
          }
        }
      }
    }
    else {
      // std::cout <<"no such file" << std::endl;
    }
  }

  void DbcAnalysis::setFileName(std::string fn) {
    filename = fn;
  }

  std::map<long, message> DbcAnalysis::getMessages() {
      return messages;
  }

}
