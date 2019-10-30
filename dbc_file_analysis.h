#ifndef _DBC_FILE_ANALYSIS_H_
#define _DBC_FILE_ANALYSIS_H_ 1

#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "struct_define.h"

#define MSSAGEHEAD "BO_"
#define SIGNALHEAD "SG_"


namespace dbc_analysis{
	class DbcAnalysis {
		private:
			std::map<long, message> messages;
			std::string filename;
			void messageLineTransform(std::string line, message& m);
			void signalLineTransform(std::string line, message &m);
			void getPosInfoTypeUnsignedFromStr(std::string str, signal &s);
			void getFactorOffsetFromStr(std::string str, signal &s);
			void getMaxMinFromStr(std::string str, signal &s);
			void getUnitFromStr(std::string str, signal &s);

		public:
			DbcAnalysis();
			DbcAnalysis(std::string fn);
			~DbcAnalysis();

			friend std::ostream &operator<<(std::ostream &stream, const DbcAnalysis &dbc) {
		    stream << "dbc structure: " << std::endl;
				stream << "nb of messages: " << dbc.messages.size() << std::endl;
		    for(std::map<long,message>::const_iterator m = dbc.messages.begin(); m != dbc.messages.end(); m++) {
		      stream << "  id: " << m->first << std::endl;
		      stream << "  name: " << m->second.name << std::endl;
		      stream << "  length: " << m->second.length << std::endl;
					stream << "  nb of signals: " << m->second.signals.size() << std::endl;
					for(std::vector<signal>::const_iterator s = m->second.signals.begin(); s != m->second.signals.end(); s++) {
						stream << "    name: " << s->name << std::endl;
						stream << "    (start_position, length): (" << s->start_position << ", " << s->length << ")" << std::endl;
						stream << "    (factor, offset): (" << s->factor << ", " << s->offset << ")" << std::endl;
						stream << "    (min_value, max_value): (" << s->min_value << ", " << s->max_value << ")" << std::endl;
            // 关于起始位置，和Intel格式或者是Motorola格式是有关的，如果是Intel格式，起始位通常是0
						stream << "    type: " << s->type << " (motolora: 0, intel: 1)" << std::endl;
						stream << "    unsigned: " << s->is_unsigned << " (unsigned: 1, signed: 0)" << std::endl;
						stream << "    unit: " << s->unit << std::endl;
						stream << std::endl;
					}
					stream << std::endl;
					stream << std::endl;
		    }

		    return stream;
		  }
			void fileAnalysis();
			void setFileName(std::string fn);
			std::map<long, message> getMessages();
	};
};

#endif //_DBC_FILE_ANALYSIS_H_
