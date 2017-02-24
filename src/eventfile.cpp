#include "eventfile.h"

namespace angel{
	EvtCommand::EvtCommand(BYTE*data, int len, EventFile*_parent):
		evtfile(_parent),_codes_str((char*)data,len) {}

	EvtCommand::~EvtCommand() {}
	
	EventFile::EventFile(const std::string& _mapname, int _version): version(_version),mapname(_mapname) {}

	EventFile::~EventFile()	 {}

	std::vector<const EvtCommand*> EventFile::GetEvent(size_t evtn) {}

	const std::string& EventFile::GetLocalString(size_t n) {}
}