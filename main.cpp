#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "dbc_file_analysis.h"
#include "dbc_canmsg_unpack.h"
#include "dbc_canmsg_pack.h"
#include "canmsg_define.h"
#include "util.h"

using namespace std;

void valuesPack(const long &id, const int valueSize, const double *valueInput, Canmsg *msg) {
  Message canMsgStruct = dbc_analysis::DbcAnalysis::getInstance()->getMessages()[id];
  can_util::packCanmsg(canMsgStruct, valueSize, valueInput, msg);
  msg->length = canMsgStruct.length;
  msg->id = id;
}

void msgPrint(const Canmsg &msg) {
  printf("id: %ld\n", msg.id);
  printf("length: %d\n", msg.length);
  for (int i = 0; i < 8; i++) {
    printf("%d ", msg.data[i]);
  }
  printf("\n");
}

int main() {
  string folderpath = "./dbcs";
  std::vector<std::string> files;
  getAllFilesInFolder(folderpath, &files);
  for (std::string &file : files) {
    dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file);
  }

  dbc_analysis::DbcAnalysis::getInstance()->analysisFiles();
  // dbc_analysis::DbcAnalysis::getInstance()->printMessages();

  double valueOut1[3];
  Canmsg test;
  test.id = 2021;
  test.length = 8;
  test.data[0] = 255;
  test.data[1] = 255;
  test.data[2] = 231;
  test.data[3] = 63;
  test.data[4] = 255;
  test.data[5] = 243;
  test.data[6] = 143;
  test.data[7] = 159;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 3, valueOut1);
  for (int i = 0; i < 3; i++) {
    printf("%lf ", valueOut1[i]);
  }
  printf("\n");

  double valueIn1[3] = {-1, -2, 400};
  Canmsg msg = {0};
  valuesPack(2021, 3, valueIn1, &msg);
  msgPrint(msg);

  return 0;
}
