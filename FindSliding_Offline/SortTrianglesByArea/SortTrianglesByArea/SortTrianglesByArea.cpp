#include <stdio.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <list>
#include <tuple>
#include <Windows.h>
#include <time.h>
#define ll long long

using namespace std;

struct dat {
	ll s, a, b, c;
	bool operator < (const dat& rhs) const {
		return (this->s != rhs.s) ? this->s < rhs.s : ((this->a != rhs.a) ? this->a < rhs.a : this->b < rhs.b);
	}
	bool operator == (const dat& rhs) const {
		return (this->s == rhs.s) && (this->a == rhs.a) && (this->b == rhs.b) && (this->c == rhs.c);
	}
};

const ll maxD = 50000;
const ll maxInteger = maxD + 10;
const ll range = 100;
list<tuple<ll, ll, ll> > decompositions;
dat previous;

int main(void) {
	ll D, s, a, b, c;
	ll nxta, nxtb, nxtc;
	DWORD start = GetTickCount();
	ifstream in("C:\\Users\\hbp5148\\Desktop\\Slidings\\Points200.txt");
	freopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\MinTriangles200.txt", "r", stdin);
	freopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\Slidings.txt", "w", stdout);

	printf("All cases of sliding for s > 10\n");

	ll tmpMinimalTriangles[4], A[3];
	
	//in >> D;

	for (ll i = 0; i < range; i++) {
		in >> a >> b >> c;
		decompositions.push_back(tuple<ll, ll, ll>(c, a, b) );
	}
	
	in >> nxta >> nxtb >> nxtc;
	
	for (; scanf("%lld %lld %lld %lld %lld", &D, &s, &a, &b, &c) == 5;) {
		if (s < 11) break;
		if (previous.s == s && previous.a == a && previous.b == b && previous.c == c) continue;
		previous.s = s;
		previous.a = a;
		previous.b = b;
		previous.c = c;

		while (a <= nxtc) {
			decompositions.pop_front();
			decompositions.push_back(tuple<ll, ll, ll>(nxtc, nxta, nxtb) );
			in >> nxta >> nxtb >> nxtc;
		}
		if (s >= maxInteger * maxInteger) continue;

		ll valueToSkip = 0;
		if (a == b)
			valueToSkip = 1;
		if (b == c)
			valueToSkip = 3;
		tmpMinimalTriangles[0] = s;
		tmpMinimalTriangles[1] = a;
		tmpMinimalTriangles[2] = b;
		tmpMinimalTriangles[3] = c;

		

		//We select a side number t (where t=1,2,3),
		for (ll t = 1; t < 4 && t != valueToSkip; t++) {
			//extract the squared lengths of the minimal triangle sides as an array
			//with selected side being the first element in the array.
			A[0] = tmpMinimalTriangles[t];
			A[1] = tmpMinimalTriangles[1 + (t % 3)];
			A[2] = tmpMinimalTriangles[1 + ((t + 1) % 3)];

			bool fSlidingFound = false;
			for (auto point : decompositions) {
				if (A[0] != get<0>(point)) continue;
				ll i = get<1>(point);
				ll j = get<2>(point);
				if (j <= i) {
					//and calculate two possible positions for the third vertex of the minimal triangle.
					ll x = (i * ((ll)A[0] + (ll)A[1] - (ll)A[2]) - (ll)2 * j * (ll)s);
					ll y = (j * ((ll)A[0] + (ll)A[1] - (ll)A[2]) + (ll)2 * i * (ll)s);
					ll v = (i * ((ll)A[0] + (ll)A[2] - (ll)A[1]) - (ll)2 * j * (ll)s);
					ll w = (j * ((ll)A[0] + (ll)A[2] - (ll)A[1]) + (ll)2 * i * (ll)s);
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
								s, tmpMinimalTriangles[1], tmpMinimalTriangles[2], tmpMinimalTriangles[3],
								i, j, x, y, v, w);
						}
						fSlidingFound = true;
					}
				}
			}
		}
	}
	cout << GetTickCount() - start << "ms\n";
	return 0;
}