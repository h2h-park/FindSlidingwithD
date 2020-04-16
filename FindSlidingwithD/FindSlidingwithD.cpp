#include <stdio.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <time.h>
#include <Windows.h>
#include <iostream>
#define ll long long

using namespace std;
vector<tuple<ll, ll, ll> > points;
struct triangle {
	ll D, s, a, b, c;
};
vector<triangle> minTriangles;

void getPointsD(ll D) {
	for (ll k = 100; k >= 0; k--) {
		ll j = sqrt(D + k);
		for (ll i = 0; i * i <= D + k; i++) {
			if (i * i + j * j == D + k) {
				points.push_back({ i, j, D + k });
				points.push_back({ j, i, D + k });
				j--;
			}
			else if (i * i + j * j > D + k) j--;
		}
	}
}

void getMinTriangles(void) {

	ll sidesSquared[3];

	struct dat {
		ll s, a, b, c;
	};
	vector<dat> minimalTriangles;
	vector<dat> previousTri;

	ll previousD = 0;
	ll minDoubledArea;
	for (auto p : points) {
		ll i = get<0>(p);
		ll j = get<1>(p);
		ll D = get<2>(p);

		ll p = (ll)(-sqrt(3.0) * (double)j + i) / 2;
		ll q = (ll)(sqrt(3.0) * (double)i + j) / 2;
		//and selecting site B in 5x5 neighborhood of (p, q)
		if (i == 0 && j == 0) continue;
		if (previousD != D) {
			if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) {
				minimalTriangles = previousTri;
			}
			for (auto triangles : minimalTriangles) {
				minTriangles.push_back({ previousD, triangles.s, triangles.a, triangles.b, triangles.c });
				printf("%lld %lld %lld %lld %lld\n", previousD, triangles.s, triangles.a, triangles.b, triangles.c);
			}
			previousD = D;
			minDoubledArea = D; // initializing s
			previousTri = minimalTriangles;
			minimalTriangles.clear();
		}
		for (ll k = p - 3; k < p + 3; k++) {
			for (ll l = q - 2; l < q + 3; l++) {
				//For each new triangle AOB we calculate doubled triangle area s
				ll s = abs(i * l - j * k);
				//as well as ordered squared side lengths
				sidesSquared[0] = D;
				sidesSquared[1] = k * k + l * l;
				sidesSquared[2] = (i - k) * (i - k) + (j - l) * (j - l);
				sort(sidesSquared, sidesSquared + 3);

				//The triangle AOB is the candidate to be a new minimal triangle only if
				//OA is its shortest side and its longest side is not longer than |OA| * sqrt(2).
				if (D == sidesSquared[0] && sidesSquared[2] <= 2 * sidesSquared[0]) {

					if (minDoubledArea > s)
					{
						minimalTriangles.clear();
						minDoubledArea = s;
						minimalTriangles.push_back({ s, sidesSquared[0], sidesSquared[1], sidesSquared[2] });
					}
					else if (minDoubledArea == s) {
						//We want to store each triangle only once and we understand a triangle as
						//the ordered triple of squared side lengths.
						bool fAlreadyExists = false;
						for (int n = 0; n < minimalTriangles.size(); n++)
							if (minimalTriangles[n].s == s
								&& minimalTriangles[n].a == sidesSquared[0]
								&& minimalTriangles[n].b == sidesSquared[1]
								&& minimalTriangles[n].c == sidesSquared[2]) {
								fAlreadyExists = true;
								break;
							}
						//Each newly encountered minimal triangle is added to the list as the integer array with 4 elements:
						//doubled minimal area and ordered squared side lengths.
						if (!fAlreadyExists)
							minimalTriangles.push_back({ s, sidesSquared[0], sidesSquared[1], sidesSquared[2] });
					}

				}
			}
		}
	}
	if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) {
		minimalTriangles = previousTri;
	}
	for (auto triangles : minimalTriangles) {
		minTriangles.push_back({ previousD, triangles.s, triangles.a, triangles.b, triangles.c });
		printf("%lld %lld %lld %lld %lld\n", previousD, triangles.s, triangles.a, triangles.b, triangles.c);
	}
}

void getSliding(void) {
	struct dat {
		ll s, a, b, c;
		bool operator < (const dat& rhs) const {
			return (this->s != rhs.s) ? this->s < rhs.s : ((this->a != rhs.a) ? this->a < rhs.a : this->b < rhs.b);
		}
		bool operator == (const dat& rhs) const {
			return (this->s == rhs.s) && (this->a == rhs.a) && (this->b == rhs.b) && (this->c == rhs.c);
		}
	};
	ll tmpMinimalTriangles[4], A[3];
	dat previous = { 0, 0, 0, 0 };

	for (auto now : minTriangles) {
		if (previous.s == now.s && previous.a == now.a && previous.b == now.b && previous.c == now.c) continue;
		previous.s = now.s;
		previous.a = now.a;
		previous.b = now.b;
		previous.c = now.c;

		ll valueToSkip = 0;
		if (now.a == now.b)
			valueToSkip = 1;
		if (now.b == now.c)
			valueToSkip = 3;
		tmpMinimalTriangles[0] = now.s;
		tmpMinimalTriangles[1] = now.a;
		tmpMinimalTriangles[2] = now.b;
		tmpMinimalTriangles[3] = now.c;



		//We select a side number t (where t=1,2,3),
		for (ll t = 1; t < 4 && t != valueToSkip; t++) {
			//extract the squared lengths of the minimal triangle sides as an array
			//with selected side being the first element in the array.
			A[0] = tmpMinimalTriangles[t];
			A[1] = tmpMinimalTriangles[1 + (t % 3)];
			A[2] = tmpMinimalTriangles[1 + ((t + 1) % 3)];

			bool fSlidingFound = false;
			for (auto point : points) {
				if (A[0] != get<2>(point)) continue;
				ll i = get<0>(point);
				ll j = get<1>(point);
				if (j <= i) {
					//and calculate two possible positions for the third vertex of the minimal triangle.
					ll x = (i * ((ll)A[0] + (ll)A[1] - (ll)A[2]) - (ll)2 * j * (ll)now.s);
					ll y = (j * ((ll)A[0] + (ll)A[1] - (ll)A[2]) + (ll)2 * i * (ll)now.s);
					ll v = (i * ((ll)A[0] + (ll)A[2] - (ll)A[1]) - (ll)2 * j * (ll)now.s);
					ll w = (j * ((ll)A[0] + (ll)A[2] - (ll)A[1]) + (ll)2 * i * (ll)now.s);
					//Check if both positions have integer coordinates (which means sliding).
					if (x % (2 * A[0]) == 0 && y % (2 * A[0]) == 0 && v % (2 * A[0]) == 0
						&& w % (2 * A[0]) == 0 && A[1] != A[2]) {
						//Calculate these coordinates
						x = x / 2 / A[0];
						y = y / 2 / A[0];
						v = v / 2 / A[0];
						w = w / 2 / A[0];
						if (!fSlidingFound) {
							printf("s = %lld, [%lld|%lld|%lld], (0,0) -- (%lld,%lld) -- (%lld,%lld), -- (%lld, %lld)\n",
								now.s, tmpMinimalTriangles[1], tmpMinimalTriangles[2], tmpMinimalTriangles[3],
								i, j, x, y, v, w);
						}
						fSlidingFound = true;
					}
				}
			}
		}
	}
}

int main(void) {
	ll D;
	freopen("SlidingswithD.txt", "w", stdout);
	cin >> D;

	getPointsD(D);
	getMinTriangles();
	getSliding();


	fclose(stdout);
	return 0;
}

//external sorting