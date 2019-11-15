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
  dbc_analysis::DbcAnalysis dbcAnalysis;
  dbcAnalysis.addOneDbcFile(file);
  dbcAnalysis.addOneDbcFile(file1);
  dbcAnalysis.addOneDbcFile(file2);
  dbcAnalysis.addOneDbcFile(file3);
  dbcAnalysis.fileAnalysis();
  // dbcAnalysis.printMessages();

  double valueOut[4] = {3, 3, 3, 3};
  Canmsg test;
  test.id = 1280;
  test.length = 8;
  test.data[0] = 0xe2;
  test.data[1] = 0x01;
  test.data[2] = 0xfa;
  test.data[3] = 0x40;
  test.data[4] = 0x7e;
  test.data[5] = 0x90;
  test.data[6] = 0x00;
  test.data[7] = 0x00;
  unpackCanmsg(dbcAnalysis.getMessages()[test.id], test, 4, valueOut);

  for (int i = 0; i < 4; i++) {
    printf("%lf ", valueOut[i]);
  }
  printf("\n");


  float valueIn[4] = {0, 0, 0, 0};
  Canmsg msg = {0};
  packCanmsg(dbcAnalysis.getMessages()[1280], 4, valueIn, &msg);

  for (int i = 0; i < 8; i++) {
    printf("%x ", msg.data[i]);
  }
  printf("\n");
  return 0;
}
