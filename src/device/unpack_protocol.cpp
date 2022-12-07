#include <iostream>

#include "unpack_protocol.hpp"
#include "app_db.hpp"

namespace device {

std::pair<std::string, std::string> Unpack(std::vector<unsigned char> a_vec)
{
	size_t idStart = sizeof(int64_t);
	size_t idEnd = sizeof(int64_t) + sizeof(int32_t);
	unsigned char *chID;
	std::copy (a_vec.begin() + idStart, a_vec.begin() + idEnd, 
						reinterpret_cast<unsigned char*>(&chID));
	
	uint32_t *idBeforeBetoh = reinterpret_cast<uint32_t*>(&chID);
	//uint32_t id = be32toh(*idBeforeBetoh);
	device::AppDB deviceTable;
	//std::string stdID = deviceTable.DeviceName(id);

	std::string stdID = deviceTable.DeviceName(*idBeforeBetoh);

	size_t strStart = sizeof(int64_t) + sizeof(int32_t);
	std::string payload;
	for (size_t i = strStart; i < a_vec.size(); ++i) {
			payload.push_back(a_vec[i]);
	}

	return {stdID, payload};
}

} //device