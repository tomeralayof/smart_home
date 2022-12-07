#include <iostream>

#include "pack_protocol.hpp"
#include "app_db.hpp"

namespace device {

std::vector<unsigned char> Pack(std::string a_id, const std::string &a_payLoad)
{
	size_t size = sizeof(uint32_t) + a_payLoad.length();
	uint64_t htonSize = static_cast<uint64_t>(size);
	unsigned char* sizeTemp = reinterpret_cast<unsigned char*> (&htonSize);
	std::vector<unsigned char> buffer;

	for(size_t i = 0; i < sizeof(uint64_t); ++i){
		buffer.push_back(sizeTemp[i]);
	}

	device::AppDB deviceTable;
	uint32_t tempID = deviceTable.DeviceID(a_id);
	
	unsigned char* chTempID = reinterpret_cast<unsigned char*> (&tempID);

	for(size_t i = 0; i < sizeof(uint32_t); ++i){
		buffer.push_back(chTempID[i]);
	}

	unsigned char * chTemp = reinterpret_cast<unsigned char*>(const_cast<char*>(a_payLoad.c_str()));

	size_t sizeStr = a_payLoad.length();
	
	for(size_t i = 0; i < sizeStr; ++i){
		buffer.push_back(chTemp[i]);
	}
	
	return buffer;
}

} //device