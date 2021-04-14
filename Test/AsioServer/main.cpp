#include <string>
#include <iostream>
#include <chrono>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std::chrono;
#define BUFFER_SIZE 100000
#define TEST_TIMES 100

#define Interval_Millisceonds 1
void Precision_Sleep()
{
	std::string buffer;
	buffer.assign(BUFFER_SIZE, 0);
	buffer.clear();
	int i = TEST_TIMES;
	uint64_t total_used = 0;
	while (i) {
		i--;
		high_resolution_clock::time_point time_begin = high_resolution_clock::now();
		::Sleep(Interval_Millisceonds);
		high_resolution_clock::time_point time_end = high_resolution_clock::now();
		char tmp[128] = { 0 };
		uint64_t used = duration_cast<microseconds>(time_end - time_begin).count();
		snprintf(tmp, 128, "%s Sleep %d ms, time used : %lld us\n",
			__FUNCTION__, Interval_Millisceonds, used);
		total_used += used;
		buffer += tmp;
	}
	printf("%s\n", buffer.c_str());
	printf("%s Sleep %d ms, avatar %lld us\n\n",
		__FUNCTION__, Interval_Millisceonds, total_used / TEST_TIMES);

}

int main()
{
	::timeBeginPeriod(1);
	Precision_Sleep();
	::timeEndPeriod(1);
	system("pause");
	return 0;
}