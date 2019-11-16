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
#include "struct_define.h"

using namespace std;

int main() {
  string file3 = "/home/ding/Documents/doc/hirain/gangkou/report/CANoe/Lidar_other.dbc";
  string file = "/home/ding/Documents/doc/hirain/gangkou/report/CANoe/LaserObjs.dbc";
  string file1 = "/home/ding/Documents/doc/hirain/gangkou/report/CANoe/LidarFS_polar.dbc";
  string file2 = "/home/ding/Documents/doc/hirain/gangkou/report/CANoe/Lidar_other.dbc";
   // dbcAnalysis;
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file);
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file1);
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file2);
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file3);
  dbc_analysis::DbcAnalysis::getInstance()->analysisFiles();
  // dbcAnalysis.printMessages();

  double valueOut[4];
  Canmsg test;
  test.id = 1280;
  test.length = 8;
  test.data[0] = 0xe3;
  test.data[1] = 0x01;
  test.data[2] = 0xfa;
  test.data[3] = 0x30;
  test.data[4] = 0x7e;
  test.data[5] = 0x8c;
  test.data[6] = 0x00;
  test.data[7] = 0x00;
  can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 4, valueOut);

  for (int i = 0; i < 4; i++) {
    printf("%lf ", valueOut[i]);
  }
  printf("\n");


  float valueIn[4] = {-37.74, 81.92, -0.000002, -0.000002};
  Canmsg msg = {0};
  can_util::packCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[1280], 4, valueIn, &msg);

  for (int i = 0; i < 8; i++) {
    printf("%x ", msg.data[i]);
  }
  printf("\n");
  return 0;
}
