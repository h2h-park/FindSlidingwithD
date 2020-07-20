#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

vector<pair<double, double> > intervals;

bool check_covering(void) {
	sort(intervals.begin(), intervals.end()); // sort intervals by their starting points
	if (intervals.back().second < 1.0) return false; // if the last interval is under 1, return false
	for (int i = 0; i < intervals.size() - 1; i++) {
		if (intervals[i].second < intervals[i + 1].first) { // two consecutive intervals are not overlapped, return false
			return false;
		}
	}
	return true;
}

double get_C1(double eps) {
	int s;
	for (s = 0; ; s++) {
		double d = (double)s * sqrt(3.0);
		d -= (int)d; // consider only decimal points of s * sqrt3
		intervals.push_back({ d - eps, d + eps }); // push the interval [d - eps, d + eps];
		if (check_covering()) { // check if it covers [0, 1] and break
			break;
		}
	}
	double C = eps * s, C1; // get C and C1
	C1 = C * C;
	printf("eps = %lf, s = %d, C = %lf, C_1 = %lf ", eps, s, C, C1);
	return C1;
}

// numerator of (3)
double num(double D, double eps) {
	double ret, t;
	ret = 2 * eps * eps;
	t = (4 - sqrt(8.0)) * eps;
	ret = D * t + ret;
	return ret;
}

// denominator of (3)
double den(double D, double eps) {
	double ret, N;
	ret = 1 / (eps * eps);
	N = (D + sqrt(2.0) * eps) / 2.0;
	N = sqrt(N);
	N *= pow(eps, -1.5);
	ret += N;
	return ret * 4.0;
}

int main(void) {
	int alpha2;
	double alpha, eps;
	freopen("output.txt", "w", stdout);
	scanf("%d", &alpha2); // get alpha^2
	alpha = sqrt((double)alpha2);
	eps = 0.06 * alpha;
	double C1 = get_C1(eps);

	double f = 1000, l = 100000000;
	for (int it = 1; it <= 1000; it++) { // get the minimum D by binary search
		double D = (f + l) / 2;
		double rhs = num(D, eps) / den(D, eps); // rhs of the inequality (3)

		if (rhs > C1) l = D;
		else f = D;
	}
	// get the minimum D that we have to consider
	printf("D = %d\n", (int)(f + 1.0));

	return 0;
}