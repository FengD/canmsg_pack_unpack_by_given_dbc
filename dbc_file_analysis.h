/**
* Copyright (C) 2019 Hirain Technologies
* License: Modified BSD Software License Agreement
* Author: Feng DING
* Description:
*/

#ifndef _DBC_FILE_ANALYSIS_H_
#define _DBC_FILE_ANALYSIS_H_

#include <fstream>
#include <string>
#include <iostream>
#include <pthread.h>
#include "canmsg_define.h"

#define MSSAGEHEAD "BO_"
#define SIGNALHEAD "SG_"

namespace dbc_analysis {
class DbcAnalysis {
 private:
  DbcAnalysis();
  ~DbcAnalysis();
  DbcAnalysis(const DbcAnalysis&);
  DbcAnalysis& operator=(const DbcAnalysis&);
  void transformMessageFromLine(std::string line, Message& m);
 	void transformSignalFromLine(std::string line, Message &m);
 	void getPosInfoTypeUnsignedFromStr(std::string str, Signal &s);
 	void getFactorOffsetFromStr(std::string str, Signal &s);
 	void getMaxMinFromStr(std::string str, Signal &s);
 	void getUnitFromStr(std::string str, Signal &s);
	std::map<long, Message> messages_;
	std::vector<std::string> files_;
  static DbcAnalysis* instance;
  static pthread_mutex_t mutex;

 public:
  static DbcAnalysis *getInstance();
	void printMessages();
	void analysisFiles();
	void addOneDbcFile(const std::string &filePath);
	std::map<long, Message>& getMessages();
};
} // namespace dbc_analysis

#endif //_DBC_FILE_ANALYSIS_H_
