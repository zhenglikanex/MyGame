#include<Windows.h>
#include<iostream>
#include<time.h>

using namespace std;
int main() {
	LARGE_INTEGER li;
	li.QuadPart = -10000;

	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (!SetWaitableTimer(hTimer, &li, 1000, NULL, NULL, 0)) {
		cout << "error" << endl;
		CloseHandle(hTimer);
		return 0;
	}
	while (1) {
		clock_t c_beg = clock();
		WaitForSingleObject(hTimer, INFINITE);
		SetWaitableTimer(hTimer, &li, 1000, NULL, NULL, 0);
		clock_t end = clock() - c_beg;
		cout << "time:" << end << endl;
	}
	CloseHandle(hTimer);
	system("pause");
	return 0;
}