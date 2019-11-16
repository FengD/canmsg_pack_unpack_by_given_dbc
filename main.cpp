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
  // string file2 = "./HMI260.dbc";
   // dbcAnalysis;
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file);
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file1);
  // dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file2);
  dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(file3);
  dbc_analysis::DbcAnalysis::getInstance()->analysisFiles();
  dbc_analysis::DbcAnalysis::getInstance()->printMessages();

  // double valueOut[3];
  // Canmsg test;
  // test.id = 2024;
  // test.length = 8;
  // test.data[0] = 255;
  // test.data[1] = 255;
  // test.data[2] = 224;
  // test.data[3] = 193;
  // test.data[4] = 0;
  // test.data[5] = 7;
  // test.data[6] = 83;
  // test.data[7] = 0;
  // can_util::unpackCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[test.id], test, 3, valueOut);
  //
  // for (int i = 0; i < 3; i++) {
  //   printf("%lf ", valueOut[i]);
  // }
  // printf("\n");


  float valueIn1[4] = {10, 200, 19, 37};
  Canmsg msg = {0};
  can_util::packCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[1280], 4, valueIn1, &msg);

  for (int i = 0; i < 8; i++) {
    printf("%d ", msg.data[i]);
  }
  printf("\n");
  msg = {0};
  float valueIn2[3] = {-20, 0, 0.005};
  can_util::packCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[2024], 3, valueIn2, &msg);
  for (int i = 0; i < 8; i++) {
    printf("%d ", msg.data[i]);
  }
  printf("\n");
  msg = {0};
  float valueIn3[4] = {22, 0, 1, 0};
  can_util::packCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[2023], 4, valueIn3, &msg);
  for (int i = 0; i < 8; i++) {
    printf("%d ", msg.data[i]);
  }
  printf("\n");
  msg = {0};
  float valueIn4[6] = {1, 2, 1,20,15,1};
  can_util::packCanmsg(dbc_analysis::DbcAnalysis::getInstance()->getMessages()[1024], 6, valueIn4, &msg);
  for (int i = 0; i < 8; i++) {
    printf("%d ", msg.data[i]);
  }
  printf("\n");

  return 0;
}
