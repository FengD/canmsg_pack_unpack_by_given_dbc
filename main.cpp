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

void valuesPack(const long &id, const int valueSize, const float *valueInput, Canmsg *msg) {
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
    std::cout << file << std::endl;
    dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file);
  }
  // string file2 = "./HMI260.dbc";
   // dbcAnalysis;
  // dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file);
  // dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file1);
  // // dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file2);
  // dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file3);
  dbc_analysis::DbcAnalysis::getInstance()->analysisFiles();
  dbc_analysis::DbcAnalysis::getInstance()->printMessages();

  double valueOut1[4], valueOut2[3], valueOut3[4], valueOut4[6];
  Canmsg test;
  test.id = 1280;
  test.length = 8;
  test.data[0] = 136;
  test.data[1] = 19;
  test.data[2] = 166;
  test.data[3] = 14;
  test.data[4] = 113;
  test.data[5] = 98;
  test.data[6] = 184;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 4, valueOut1);

  for (int i = 0; i < 4; i++) {
    printf("%lf ", valueOut1[i]);
  }
  printf("\n");

  test.id = 2024;
  test.length = 8;
  test.data[0] = 255;
  test.data[1] = 254;
  test.data[2] = 199;
  test.data[3] = 132;
  test.data[7] = 16;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 3, valueOut2);

  for (int i = 0; i < 3; i++) {
    printf("%lf ", valueOut2[i]);
  }
  printf("\n");

  test.id = 2023;
  test.length = 8;
  test.data[0] = 85;
  test.data[1] = 239;
  test.data[4] = 64;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 4, valueOut3);

  for (int i = 0; i < 4; i++) {
    printf("%lf ", valueOut3[i]);
  }
  printf("\n");

  test.id = 1024;
  test.length = 8;
  test.data[0] = 1;
  test.data[1] = 18;
  test.data[2] = 20;
  test.data[4] = 15;
  test.data[5] = 1;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 6, valueOut4);

  for (int i = 0; i < 6; i++) {
    printf("%lf ", valueOut4[i]);
  }
  printf("\n");


  float valueIn1[4] = {10, 200, 19, 37};
  Canmsg msg = {0};
  valuesPack(1280, 4, valueIn1, &msg);
  msgPrint(msg);
  msg = {0};
  float valueIn2[3] = {-20, 0, 0.005};
  valuesPack(2024, 3, valueIn2, &msg);
  msgPrint(msg);
  msg = {0};
  float valueIn3[4] = {22, 0, 1, 0};
  valuesPack(2023, 4, valueIn3, &msg);
  msgPrint(msg);
  msg = {0};
  float valueIn4[6] = {1, 2, 1,20,15,1};
  valuesPack(1024, 6, valueIn4, &msg);
  msgPrint(msg);

  return 0;
}
