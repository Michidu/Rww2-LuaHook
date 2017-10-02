#include "signatures.h"

#include <algorithm>

namespace rww2hook
{
	namespace signatures
	{
		std::vector<BYTE> GetRawData(intptr_t* dw_module_base)
		{
			*dw_module_base = reinterpret_cast<intptr_t>(GetModuleHandle(nullptr));
			const PIMAGE_DOS_HEADER p_dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(*dw_module_base);

			if (p_dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				return std::vector<BYTE>();

			const PIMAGE_NT_HEADERS p_nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(*dw_module_base + p_dos_header->e_lfanew);

			*dw_module_base += p_nt_headers->OptionalHeader.BaseOfCode;
			const DWORD dw_module_size = p_nt_headers->OptionalHeader.SizeOfCode;

			unsigned char* data = reinterpret_cast<unsigned char*>(*dw_module_base);

			std::vector<BYTE> rawdata(data, data + dw_module_size);

			return rawdata;
		}

		LPVOID Search(const std::vector<unsigned char>& data, intptr_t base_address,
		              const char* lp_pattern, const char* psz_mask, int offset, int result_usage)
		{
			std::vector<std::pair<char, bool>> pattern;

			const size_t len = strlen(psz_mask);
			for (size_t x = 0; x < len; ++x)
				pattern.push_back(std::make_pair(lp_pattern[x], psz_mask[x] == 'x'));

			int result_count = 0;
			auto scan_start = data.begin();

			while (true)
			{
				auto ret = std::search(scan_start, data.end(), pattern.begin(), pattern.end(),
				                       [&](char curr, std::pair<char, bool> curr_pattern)
			                       {
				                       return !curr_pattern.second || curr == curr_pattern.first;
			                       });

				// Did we find a match.. 
				if (ret == data.end())
					break;

				// If we hit the usage count, return the result.. 
				if (result_count == result_usage || result_usage == 0)
					return reinterpret_cast<LPVOID>((std::distance(data.begin(), ret) + base_address) + offset);

				// Increment the found count and scan again.. 
				++result_count;
				scan_start = ++ret;
			}

			return nullptr;
		}
	}
}
