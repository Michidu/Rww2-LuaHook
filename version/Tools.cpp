#include "Tools.h"

#include <ctime>

namespace rww2hook
{
	namespace tools
	{
		std::string GetTime()
		{
			time_t now = time(nullptr);

			char buf[80];

			tm tstruct;
			localtime_s(&tstruct, &now);

			strftime(buf, sizeof buf, "%Y-%m-%d.%X", &tstruct);

			return buf;
		}
	}
}
