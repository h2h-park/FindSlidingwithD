#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>
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
double sqrt3 = sqrt(3.0);

struct dat {
	ll x, y, D;
	bool operator < (const dat& rhs) const {
		return D < rhs.D;
	}
};
std::priority_queue<dat> pq;

class pt {
public:
	ll x, y;
//	friend std::hash<pt>;
	bool operator == (const pt& rhs) const {
		return (x == rhs.x) && (y == rhs.y);
	}
	bool operator < (const pt& rhs) const {
		return (x*x + y*y) < (rhs.x * rhs.x + rhs.y * rhs.y);
	}
};

// define hash function for the unordered map
namespace std {
	template<>
	class hash<pt> {
	public:
		size_t operator()(const pt& x) const {
			using std::hash;
			return hash<long long>()(x.x) ^ hash<long long>()(x.y) << 1;
		}
	};
}

std::unordered_set<pt> st; // this is an unordered_set for visit checking in bfs

void getTriangles(ll x, ll y, ll D) {
	// Find triangle such that D = l1 <= l2 <= l3 based on the point (x, y)
	ll sidesSquared[3];
	pts v;

	ll p = (ll)(-sqrt3 * (double)y + x) / 2;
	ll q = (ll)(sqrt3 * (double)x + y) / 2;
	// Rotate (x, y) by 60 degrees

	// Examine the neighborhood of the given point
	for (ll i = x; i < x + 3; i++) {
		for (ll j = y; j < y + 3; j++) {
			ll l1 = i * i + j * j;
			// Examine the neighborhood of the rotated point
			for (ll k = p - 3; k < p + 3; k++) {
				for (ll l = q - 3; l < q + 3; l++) {
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
						minimalTriangles.push_back({ D, s, sidesSquared[0], sidesSquared[1], sidesSquared[2], v });
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

int main(void) {
	ll D;
	DWORD start = GetTickCount();
	DWORD pqtime = 0, sttime = 0, fortime = 0, getTtime = 0;
	DWORD t;
	freopen("SlidingswithD.txt", "w", stdout);
	D = 200;
	//cin >> D;

	int cnt = 0;
	pq.push({ D, D, D * D + D * D }); // Start with (D, D)
	for (; !pq.empty();) { // run until the priority queue is empty
		dat now = pq.top(); // read the item from pq
		if (now.D < 100) break; // ignore small distance cases
		t = GetTickCount();
		pq.pop();
		pqtime += GetTickCount() - t;

		t = GetTickCount();
		st.erase({ now.x, now.y });
		sttime += GetTickCount() - t;

		// consider the left point of the current point
		t = GetTickCount();
		auto it = st.find({ now.x - 1, now.y });
		sttime += GetTickCount() - t;

		t = GetTickCount();
		auto it2 = st.end();
		sttime += GetTickCount() - t;


		if (now.x > 0 && now.x - 1 >= now.y && it == it2) {
			t = GetTickCount();
			pq.push({ now.x - 1, now.y, (now.x - 1) * (now.x - 1) + now.y * now.y });
			pqtime += GetTickCount() - t;

			t = GetTickCount();
			st.insert({ now.x - 1, now.y }); // visit checking
			sttime += GetTickCount() - t;
		}
		// consider the the point below of the current point

		t = GetTickCount();
		auto it3 = st.find({ now.x, now.y - 1 });
		sttime += GetTickCount() - t;

		if (now.y > 0 && now.x >= now.y - 1 && it3 == it2) {
			t = GetTickCount();
			pq.push({ now.x, now.y - 1, now.x * now.x + (now.y - 1) * (now.y - 1) });
			pqtime += GetTickCount() - t;

			t = GetTickCount();
			st.insert({ now.x, now.y - 1 });
			sttime += GetTickCount() - t;
		}

		// get minimial triangles when the distance is smaller or equal to D*D
		if (now.D <= D * D) {
			t = GetTickCount();
			cnt += 2;
			getTriangles(now.x, now.y, now.D);
			getTriangles(now.x, -now.y, now.D);
			getTtime += GetTickCount() - t;
		}

		// at the point of change of the distance, we have to update minimal triangles of the current distance
		if (!pq.empty() && now.D != pq.top().D) {
			minDoubledArea = pq.top().D * pq.top().D; // Initialize minDoubledArea for the next D

			if (minimalTriangles.size() == 0) continue;

			//minimalTriangles has information of the minimal triangle with "D = l1 <= l2 <= l3"
			// If the minimal doubled area is larger than the previous one, we need to replace it with the previous one.
			if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) minimalTriangles = previousTri;
			else previousTri = minimalTriangles;

			// Now minimalTriangles has information of the minimal triangle with "D <= l1 <= l2 <= l3" because we compared and updated with the previous one
			t = GetTickCount();
			for (auto triangles : minimalTriangles) {
				triangles.D = now.D;
				//Push every minimal triangles to minTriangle
				minTriangles.push_back(triangles);
				//pts v = triangles.v;
				//printf("%lld %lld %lld %lld %lld : %lld %lld %lld %lld\n", D, triangles.s, triangles.a, triangles.b, triangles.c, v.x1, v.y1, v.x2, v.y2);
			}
			fortime += GetTickCount() - t;
			minimalTriangles.clear();
		}
	}

	t = GetTickCount();
	getSliding();
	printf("Sliding %dms\n", GetTickCount() - t);
	minTriangles.clear();
	printf("%dms\n", GetTickCount() - start);
	printf("pq time : %dms\n", pqtime);
	printf("set time : %dms\n", sttime);
	printf("for loop time : %dms\n", fortime);
	printf("get tri time : %d calling times : %d\n", getTtime, cnt);

	fclose(stdout);
	return 0;
}