#include <stdio.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <time.h>
#include <Windows.h>
#include <iostream>
#define ll long long

using namespace std;

vector<tuple<int, int, int> > points;
struct pts {
	ll x1, y1, x2, y2;
};
struct triangle {
	ll D, s, a, b, c;
	pts v;
};
vector<triangle> minTriangles;
vector<triangle> previousTri;
vector<triangle> minimalTriangles;
ll minDoubledArea;

void getMinTriangles(ll x, ll y, ll D) {
	//Find triangle such that D = l1 <= l2 <= l3
	ll sidesSquared[3];
	pts v;
	
	ll p = (ll)(-sqrt(3.0) * (double)y + x) / 2;
	ll q = (ll)(sqrt(3.0) * (double)x + y) / 2;

	for (ll i = x - 3; i < x + 3; i++) {
		for (ll j = y - 3; j < y + 3; j++) {
			ll l1 = i * i + j * j;
			for (ll k = p - 3; k < p + 3; k++) {
				for (ll l = q - 2; l < q + 3; l++) {
					ll l2 = k * k + l * l;
					ll l3 = (k - i) * (k - i) + (l - j) * (l - j);
					v.x1 = i, v.x2 = k, v.y1 = j, v.y2 = l;
					sidesSquared[0] = l1;
					sidesSquared[1] = l2;
					sidesSquared[2] = l3;
					sort(sidesSquared, sidesSquared + 3);
					if (sidesSquared[0] != D) continue;
					ll s = abs(i * l - j * k);

					if (minDoubledArea > s)
					{
						minimalTriangles.clear();
						minDoubledArea = s;
						minimalTriangles.push_back({ D, s, sidesSquared[0], sidesSquared[1], sidesSquared[2], v });
					}
					else if (minDoubledArea == s) {
						minimalTriangles.push_back({D, s, sidesSquared[0], sidesSquared[1], sidesSquared[2], v });
					}
				}
			}
		}
	}
}

void getSliding(void) {
	int i, sz = minTriangles.size();
	for (i = sz - 1; i > 0; i--) {
		triangle& t1 = minTriangles[i];
		triangle& t2 = minTriangles[i - 1];
		if (t1.D != t2.D) return;
		
		if (t1.v.x1 == t2.v.x1 && t1.v.y1 == t2.v.y1) {
			printf("D : %lld S(D) = %lld base = (%lld, %lld) points = (%lld, %lld), (%lld, %lld)\n", t1.D, t1.s, t1.v.x1, t1.v.y1, t1.v.x2, t1.v.y2, t2.v.x2, t2.v.y2);
		}
		if (t1.v.x2 == t2.v.x2 && t1.v.y2 == t2.v.y2) {
			printf("D : %lld S(D) = %lld base = (%lld, %lld) points = (%lld, %lld), (%lld, %lld)\n", t1.D, t1.s, t1.v.x2, t1.v.y2, t1.v.x1, t1.v.y1, t2.v.x1, t2.v.y1);
		}
	}
}

void getPointsD(ll D) {
	for (ll k = 10; k >= 0; k--) {
		ll j = sqrt(D + k);
		minDoubledArea = D + k;
		for (ll i = 0; i * i <= D + k; i++) {
			if (i * i + j * j == D + k) {
				getMinTriangles(j, i, D + k);
				points.push_back({ i, j, D + k });
				points.push_back({ j, i, D + k });
				j--;
			}
			else if (i * i + j * j > D + k) j--;
			if (i > j) break;
		}

		if (minimalTriangles.size() == 0) continue;

		if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) minimalTriangles = previousTri;
		else previousTri = minimalTriangles;

		for (auto triangles : minimalTriangles) {
			triangles.D = D + k;
			minTriangles.push_back(triangles);
			pts v = triangles.v;
			printf("%lld %lld %lld %lld %lld : %lld %lld %lld %lld\n", D + k, triangles.s, triangles.a, triangles.b, triangles.c, v.x1, v.y1, v.x2, v.y2);
		}
		minimalTriangles.clear();
	}
}

int main(void) {
	ll D;
	freopen("SlidingswithD.txt", "w", stdout);
	cin >> D;

	getPointsD(D);
	getSliding();

	fclose(stdout);
	return 0;
}

//external sorting