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
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "struct_define.h"

#define MSSAGEHEAD "BO_"
#define SIGNALHEAD "SG_"

namespace dbc_analysis {
class DbcAnalysis {
 private:
  void messageLineTransform(std::string line, message& m);
 	void signalLineTransform(std::string line, message &m);
 	void getPosInfoTypeUnsignedFromStr(std::string str, signal &s);
 	void getFactorOffsetFromStr(std::string str, signal &s);
 	void getMaxMinFromStr(std::string str, signal &s);
 	void getUnitFromStr(std::string str, signal &s);
	std::map<long, message> messages_;
	std::vector<std::string> files_;

 public:
	DbcAnalysis();
	DbcAnalysis(std::string fn);
	~DbcAnalysis();

	void printMessages();
	void fileAnalysis();
	void addOneDbcFile(const std::string &filePath);
	std::map<long, message> getMessages();
};
} // namespace dbc_analysis

#endif //_DBC_FILE_ANALYSIS_H_
