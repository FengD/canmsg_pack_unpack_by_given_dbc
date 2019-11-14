#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "dbc_file_analysis.h"
#include "dbc_canmsg_unpack.h"
#include "struct_define.h"
using namespace std;

int main() {
  string file = "./HMI260.dbc";
  string file2 = "./HMI260.dbc1";
  string file3 = "./HMI260.dbc12";
  dbc_analysis::DbcAnalysis dbcAnalysis;
  dbcAnalysis.addOneDbcFile(file);
  dbcAnalysis.addOneDbcFile(file2);
  dbcAnalysis.addOneDbcFile(file3);
  dbcAnalysis.fileAnalysis();
  dbcAnalysis.printMessages();
  can_msg test;
  test.id = 513;
  test.length = 5;
  test.data[0] = 123;
  test.data[1] = 123;
  test.data[2] = 123;
  test.data[3] = 22;
  test.data[4] = 23;
  test.data[5] = 0;
  test.data[6] = 0;
  test.data[7] = 0;
  unpackCanmsg (dbcAnalysis.getMessages()[test.id], test);
  return 0;
}
