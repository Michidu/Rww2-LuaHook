#pragma once

#include <Windows.h>
#include <vector>

namespace rww2hook
{
	namespace signatures
	{
		std::vector<BYTE> GetRawData(intptr_t* dw_module_base);
		LPVOID Search(const std::vector<unsigned char>& data, intptr_t base_address, const char* lp_pattern,
		              const char* psz_mask, int offset, int result_usage);
	}
}
