#include <stdio.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <time.h>
#include <Windows.h>
#include <iostream>

using namespace std;

const long long maxD = 50000;
const long long maxInteger = maxD + 10;
vector < tuple<long long, long long, long long > > decompositions;
int main(void) {
	DWORD start = GetTickCount();
	freopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\Points.txt", "w", stdout);
	cout << maxInteger * (maxInteger + 1) / 2  << '\n';

	for (long long i = 0; i < maxInteger; i++) {
		for (long long j = 0; j <= i; j++) {
			long long m = i * i + j * j;
			printf("%lld %lld %lld\n", i, j, m);
		}
	}
	cout << GetTickCount() - start << "ms\n";
	fclose(stdout);
	return 0;
}

//external sorting