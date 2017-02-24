#include "stdafx.h"
#include "mmstrfile.h"
namespace angel {
	MMStrFile::MMStrFile(const std::string& mapname) {
		
		std::string::size_type pos1 = mapname.rfind('.');
		if(pos1 == std::string::npos)
			pos1 = mapname.size();
		std::string strname = mapname.substr(0,pos1)+".str";
		if( LoadFromFile(strname))
			return;

		std::string::size_type pos2 = mapname.rfind('/');
		if(pos2 == std::string::npos)
			pos2 = mapname.size();
		strname = std::string("icons") + mapname.substr(pos2,pos1-pos2)+".str";
		if( LoadFromFile(strname))
			return;
		strname = std::string("language") + mapname.substr(pos2,pos1-pos2)+".str";
		if( LoadFromFile(strname))
			return;
	}

	MMStrFile::~MMStrFile() {}

	bool MMStrFile::LoadFromFile(const std::string& filename) {
		pLodData ldata = LodManager.LoadFileData(filename);
		if(!ldata)
			return false;
		std::string val;
		for( LodData::const_iterator ii = ldata->begin(); ii != ldata->end(); ii++) {
			char ch = (char)*ii;
			if(ch == 0 )
			{
				strs.push_back(val);
				val="";
				continue;
			}
			val += ch;
		}
		strs.push_back(val);
		return true;
	}

}