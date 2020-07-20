#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#define ll long long

using namespace std;

ll minDoubledArea;
double sqrt3 = sqrt(3.0);

void getTriangles(ll x, ll y, ll D) {
	// Find triangle such that D = l1 <= l2 <= l3 based on the point (x, y)

	// Rotate (x, y) by 60 degrees
	ll p = (ll)(-sqrt(3.0) * (double)y + x) / 2;
	ll q = (ll)(sqrt(3.0) * (double)x + y) / 2;

	ll l1 = x * x + y * y;
	// Examine the neighborhood of the rotated point
	for (ll k = p - 2; k < p + 3; k++) {
		for (ll l = q - 2; l < q + 3; l++) {
			ll l2 = k * k + l * l;
			ll l3 = (k - x) * (k - x) + (l - y) * (l - y);

			if (l2 < D || l3 < D) continue;
			
			// Get the area of this triangle
			ll s = abs(x * l - y * k);

			// Check this area is minimal
			if (minDoubledArea > s)	{
				minDoubledArea = s;
			}
		}
	}
}

bool isSliding(ll D, ll area) {

	minDoubledArea = D; // Initialize for S(D)
	for (ll d = D; d <= D + 100; d++) { // consider proper intervals to get S(D)
		if (d % 4 == 3) continue; // there is no integer point of distance d^2 if d = 3 (mod 4)
		ll j = sqrt(d);
		for (ll i = 0; i * i <= d; i++) {
			if (i > j) break; // It is enough to consider area between y = x and y = -x with x > 0
			if (i * i + j * j == d) { // if the distance of (i, j) == d^2
				getTriangles(j, i, d); 
				getTriangles(j, -i, d);
				if (minDoubledArea < area) return false;
				j--;
			}
			else if (i * i + j * j > d) i--, j--;
		}
	}

	return true;
}

int main(void) {
	ll D, x;
	DWORD start = GetTickCount();
	//freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	D = 3000; // set the limit of D

	ll dx = 1, dy = 0; // (dx, dy) : the vector between two third points of two triangles
	// by input, we can assume that the base is parellel to this vector

	for (ll k = 9; ; k++) { // iterate on the base line
		ll x = k * dx, y = k * dy; // pick integer points (x, y) on the base line
		// rotate (x, y)
		ll p = 0.5 * (double)x - sqrt3 / 2 * (double)y;
		ll q = sqrt3 / 2 * (double)x + 0.5 * (double)y; 
		ll L1 = x * x + y * y; // the length squared of (x, y)
		if (L1 > D * D)	break;
		
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// pick up the neighbor int points around the rotated point
				// let the point be (p1, q1)
				ll p1 = p + i, q1 = q + j; 

				// we can draw a triangle with three points (0, 0), (x, y), (p1, q1)
				ll L2 = p1 * p1 + q1 * q1; 
				ll L3 = (p1 - x) * (p1 - x) + (q1 - y) * (q1 - y);
				ll area = abs(x * q1 - y * p1); // area of the triangle

				//isSliding(D, area) : return if S(D) > area
				if (!isSliding(min(min(L1, L2), L3), area)) continue;

				// move the point other than O, W along with the vector
				p1 = p1 + dx, q1 = q1 + dy;
				L2 = p1 * p1 + q1 * q1;
				L3 = (p1 - x) * (p1 - x) + (q1 - y) * (q1 - y);
				if (!isSliding(min(min(L1, L2), L3), area)) continue;

				// if program is here, (x, y) is the base of a sliding
				printf("%lld %lld\n", x, y);
				
			}
		}
		
	}
	printf("%dms\n", GetTickCount() - start);
	return false;
}