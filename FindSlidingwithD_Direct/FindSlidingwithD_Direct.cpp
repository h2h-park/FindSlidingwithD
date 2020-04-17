#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <iostream>
#define ll long long

using namespace std;

struct pts {
	ll x1, y1, x2, y2;
	bool operator == (const pts& rhs) {
		return (x1 == rhs.x1) && (x2 == rhs.x2) && (y1 == rhs.y1) && (y2 == rhs.y2);
	}
};
struct triangle {
	ll D, s, a, b, c;
	pts v;
};
vector<triangle> minTriangles;
vector<triangle> previousTri;
vector<triangle> minimalTriangles;
ll minDoubledArea;

void getTriangles(ll x, ll y, ll D) {
	// Find triangle such that D = l1 <= l2 <= l3 based on the point (x, y)
	ll sidesSquared[3];
	pts v;
	
	ll p = (ll)(-sqrt(3.0) * (double)y + x) / 2;
	ll q = (ll)(sqrt(3.0) * (double)x + y) / 2;
	// Rotate (x, y) by 60 degrees

	// Examine the neighborhood of the given point
	for (ll i = x - 3; i < x + 3; i++) {
		for (ll j = y - 3; j < y + 3; j++) {
			ll l1 = i * i + j * j;
			// Examine the neighborhood of the rotated point
			for (ll k = p - 3; k < p + 3; k++) {
				for (ll l = q - 2; l < q + 3; l++) {
					ll l2 = k * k + l * l;
					ll l3 = (k - i) * (k - i) + (l - j) * (l - j);
					v.x1 = i, v.x2 = k, v.y1 = j, v.y2 = l;
					// v has the container that has coordinates of two points
					sidesSquared[0] = l1;
					sidesSquared[1] = l2;
					sidesSquared[2] = l3;
					sort(sidesSquared, sidesSquared + 3);
					// If the shortest length of the triangle is not D, ignore
					if (sidesSquared[0] != D) continue;

					// Get the area of this triangle
					ll s = abs(i * l - j * k);

					// Check this area is minimal
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
	int i, sz = minTriangles.size(), previousS = 0;
	struct sliding {
		ll D, s;
		pts v1, v2;
	};
	vector<sliding> slidings;
	for (i = sz - 1; i > 0; i--) {
		triangle& t1 = minTriangles[i];
		triangle& t2 = minTriangles[i - 1];

		// The triangle t1 and t2 is identical, ignore.
		if (t1.v == t2.v) continue;
		
		// Since the triangle t1 and t2 is not identical, we need to check they share the same point.
		if (t1.v.x1 == t2.v.x1 && t1.v.y1 == t2.v.y1) {
			// Ignore if this triangle is updated by the previous D
			if (!slidings.empty() && slidings.back().s == t1.s) continue;
			slidings.push_back({ t1.D, t1.s, t1.v, t2.v });
			printf("D : %lld S(D) = %lld base = (%lld, %lld) points = (%lld, %lld), (%lld, %lld)\n", t1.D, t1.s, t1.v.x1, t1.v.y1, t1.v.x2, t1.v.y2, t2.v.x2, t2.v.y2);
		}
		if (t1.v.x2 == t2.v.x2 && t1.v.y2 == t2.v.y2) {
			if (!slidings.empty() && slidings.back().s == t1.s) continue;
			slidings.push_back({ t1.D, t1.s, t1.v, t2.v });
			printf("D : %lld S(D) = %lld base = (%lld, %lld) points = (%lld, %lld), (%lld, %lld)\n", t1.D, t1.s, t1.v.x2, t1.v.y2, t1.v.x1, t1.v.y1, t2.v.x1, t2.v.y1);
		}
	}
}

void getMinimalTriangles(ll D) {
	ll j = sqrt(D);
	minDoubledArea = D; // Initialize for finding the minimum value
	for (ll i = 0; i * i <= D; i++) {
		if (i * i + j * j == D) {
			getTriangles(j, i, D);
			getTriangles(j, -i, D);
			j--;
		}
		else if (i * i + j * j > D) j--;
		if (i > j) break; // It is enough to consider area between y = x and y = -x with x > 0
	}

	// If there is no triangle with the shortest side length of D, ignore.
	if (minimalTriangles.size() == 0) return;

	//minimalTriangles has information of the minimal triangle with "D = l1 <= l2 <= l3"

	// If the minimal doubled area is larger than the previous one, we need to replace it with the previous one.
	if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) minimalTriangles = previousTri;
	else previousTri = minimalTriangles;

	// Now minimalTriangles has information of the minimal triangle with "D <= l1 <= l2 <= l3" because we compared and updated with the previous one
	for (auto triangles : minimalTriangles) {
		triangles.D = D;
		//Push every single information of minimal triangles to minTriangle
		minTriangles.push_back(triangles);
		//pts v = triangles.v;
		//printf("%lld %lld %lld %lld %lld : %lld %lld %lld %lld\n", D, triangles.s, triangles.a, triangles.b, triangles.c, v.x1, v.y1, v.x2, v.y2);
	}
	minimalTriangles.clear();
}

int main(void) {
	ll D;
	DWORD start = GetTickCount();
	freopen("SlidingswithD.txt", "w", stdout);
	//cin >> D;
	for (D = 100000; D >= 1000; D--) { // need to be iterated by descending order
		getMinimalTriangles(D);
	}
	getSliding();
	minTriangles.clear();
	printf("%dms\n", GetTickCount() - start);

	fclose(stdout);
	return 0;
}
