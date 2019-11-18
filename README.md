#CANMSG PACK AND UNPACK BY DBC
## Description
This package is used to transform the values to can messages or reverse by the dbc added.

## Project Structure
> **dbc_file_analysis.cpp** & **dbc_file_analysis.h**
>> IT has a singleton class named `DbcAnalysis` which used to analysis the DBC files to the info needed.
>> ``` c++
/**
* Singleton interface.
* @return [DbcAnalysis]
*/
static DbcAnalysis *getInstance();
/**
* Print the information of all the messages and the signals found from the DBC files.
*/
void printMessages();
/**
* This method is used to execute the analysis action.
*/
void analysisFiles();
/**
* And one DBC file.
*/
void addOneDbcFile(const std::string &filePath);
/**
* Get the map of the can message list.
* @return [map<id, message>] the map of the messages.
*/
std::map<long, Message>& getMessages();
>> ```

> **dbc_canmsg_unpack.cpp** & **dbc_canmsg_unpack.h**
>> Unpack the can message to values.
>> ``` c++
/** the dbc unpack used to transform the can msg to value
* @param in [struct Message] the message that you want to unpack
* @param in [struct Canmsg] the canmsg
* @param in [unsigned int] the size of the value list
* @param out [double array] the array of the values
*/
void unpackCanmsg (const Message &m, const Canmsg &msg, const size_t valueSize, double *value);
/**
* pack one Signal
* @param in [struct Signal] the Signal that you want to unpack
* @param in [unsigned char array] the data array of the can msg
* @return [double] the value that you unpack from the Signal
*/
real64_T unpackSignal (const Signal &s, const uint8_T *data);
>> ```

> **dbc_canmsg_pack.cpp** & **dbc_canmsg_pack.h**
>> Pack the value to can message.
>> ``` c++
/** the dbc pack used to transform the value to can msg
* @param in [struct Message] the message that you want to pack
* @param in [unsigned int] the size of the value list
* @param in [float array] the array of the values
* @param out [struct Canmsg] the canmsg
*/
void packCanmsg (const Message &m, const size_t &valueSize, const float *value, Canmsg *msg);
/**
* pack one Signal
* @param in [struct Signal] the Signal that you want to pack
* @param in [double] the value that you want to pack in the Signal
* @param out [unsigned char array] the data array of the can msg
*/
void packSignal (const Signal &s, const double &value, uint8_T *data);
>> ```
> **canmsg_define.h**
>> struct define.
> **util.h**
>> Tool box.

## HOW TO USE
``` c++
// the more than one dbc files
dbc_analysis::DbcAnalysis::getInstance()->addOneDbcFile(filePaath);
// Analysis the files.
dbc_analysis::DbcAnalysis::getInstance()->analysisFiles();
// Check the analysis result. If need.
dbc_analysis::DbcAnalysis::getInstance()->printMessages();
// get one message by id
Message canMsgStruct = dbc_analysis::DbcAnalysis::getInstance()->getMessages()[id];
// pack
float valueIn[4] = {10, 200, 19, 37};
Canmsg msg = {0};
can_util::packCanmsg(canMsgStruct, 4, valueIn, msg);
// pack end
// unpack
double valueOut[4];
msg = {0};
msg.id = 1280;
msg.length = 8;
msg.data[0] = 136;
msg.data[1] = 19;
can_util::unpackCanmsg(canMsgStruct, msg, 4, valueOut);
// unpack end
```

## Attention
Still has bugs for unpack signed signals. The pack module is good. The unpack could be used if all the signals are unsigned.
