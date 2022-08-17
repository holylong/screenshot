#ifndef _BASE_UTIL_H_
#define _BASE_UTIL_H_

#include <string>
#include <Windows.h>

namespace sshot{
	class SystemUtil{
		public:
		static auto to_string(double val, int fixed) {
				auto str = std::to_string(val);
				return str.substr(0, str.find(".") + fixed + 1);
		}

		static uint64_t getCPUTimeInMicroseconds()
		{
				LARGE_INTEGER frequency = {0};
				LARGE_INTEGER counter = {0};
				uint64_t ret;

				if (0 == ::QueryPerformanceFrequency(&frequency)
								|| 0 == ::QueryPerformanceCounter(&counter)
								|| 0 == frequency.QuadPart)
				{
						ret = ::GetTickCount() * 1000;
				}
				else
				{
						ret = 1000000 * (counter.QuadPart) / (frequency.QuadPart);
				}

				return ret;
		}

		static uint64_t getCPUTime()
		{
				return getCPUTimeInMicroseconds() / 1000;
		}
	};

}

#endif //_BASE_UTIL_H_