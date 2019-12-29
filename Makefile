CC = gcc
CXX = g++
CFLAGS = -pipe -g -Wall -Wextra -Werror -O2  -D_REENTRANT
CXXFLAGS = -pipe -g -Wall -W -D_REENTRANT
LDFLAGS = -L..
STD11 = -std=c++11

dbc_analysis: util.h dbc_canmsg_pack.h dbc_canmsg_unpack.h dbc_file_analysis.h \
		util.cpp dbc_canmsg_pack.cpp dbc_canmsg_unpack.cpp dbc_file_analysis.cpp main.cpp
		${CXX} -o demo \
			util.cpp dbc_canmsg_pack.cpp dbc_canmsg_unpack.cpp dbc_file_analysis.cpp main.cpp ${STD11}

clean:
	rm -f $(OBJS) *.out *~
