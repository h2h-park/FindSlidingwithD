#include <stdio.h>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include <time.h>
#define ll long long

using namespace std;

ll sidesSquared[3];

struct dat {
	ll s, a, b, c;
};
vector<dat> minimalTriangles;
vector<dat> previousTri;

int main(void) {
	ll i, j, D;
	DWORD start = GetTickCount();
	freopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\Points.txt", "r", stdin);
	freopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\MinTriangles.txt", "w", stdout); 

	ll previousD = 0;
	ll minDoubledArea;
	scanf("%lld", &i);
	for (; scanf("%lld %lld %lld", &i, &j, &D) == 3;) {
		ll p = (ll)(-sqrt(3.0) * (double)j + i) / 2;
		ll q = (ll)(sqrt(3.0) * (double)i + j) / 2;
		//and selecting site B in 5x5 neighborhood of (p, q)
		if (i == 0 && j == 0) continue;
		if (previousD != D) {
			if (previousTri.size() != 0 && previousTri[0].s < minimalTriangles[0].s) {
				minimalTriangles = previousTri;
			}
			for (auto triangles : minimalTriangles) {
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

	printf("%d ms\n", GetTickCount() - start);
	fclose(stdout);
	return 0;
}