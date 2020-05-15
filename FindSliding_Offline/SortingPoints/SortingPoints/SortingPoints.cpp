#include <stdio.h>
#include  <algorithm>
#include <vector>
#define ll long long

int level = 10;
int numBlocks = 1024;
char fname[100] = "C:\\Users\\hbp5148\\Desktop\\Slidings\\ExternalSorting\\FILE  _    .txt";
char fname1[100] = "C:\\Users\\hbp5148\\Desktop\\Slidings\\ExternalSorting\\FILE  _    .txt";
char fname2[100] = "C:\\Users\\hbp5148\\Desktop\\Slidings\\ExternalSorting\\FILE  _    .txt";
int p = 54;
ll d2, x, y;
struct data {
	ll D, i, j;
};
std::vector<data> vec;

void NamingFile(char* fn, int lev, int x) {
	fn[p] = (lev / 10) + '0';
	fn[p + 1] = (lev % 10) + '0';
	fn[p + 3] = (x / 1000) + '0';
	fn[p + 4] = (x % 1000) / 100 + '0';
	fn[p + 5] = (x % 100) / 10 + '0';
	fn[p + 6] = (x % 10) + '0';
}

void merge(char* in1, char* in2, char* out) {
	FILE* fin1 = fopen(in1, "r");
	FILE* fin2 = fopen(in2, "r");
	FILE* fout = fopen(out, "w");

	ll n1, n2;
	ll p1 = 0, p2 = 0;
	data x1, x2;
	fscanf(fin1, "%lld", &n1);
	fscanf(fin2, "%lld", &n2);
	fprintf(fout, "%lld\n", n1 + n2);
	p1++, p2++;
	fscanf(fin1, "%lld %lld %lld", &x1.i, &x1.j, &x1.D);
	fscanf(fin2, "%lld %lld %lld", &x2.i, &x2.j, &x2.D);

	for (;;) {
		if (p1 > n1 && p2 > n2) break;
		if (p2 > n2) {
			fprintf(fout, "%lld %lld %lld\n", x1.i, x1.j, x1.D);
			p1++;
			if (p1 <= n1) {
				fscanf(fin1, "%lld %lld %lld", &x1.i, &x1.j, &x1.D);
			}
		}
		else if (p1 > n1) {
			fprintf(fout, "%lld %lld %lld\n", x2.i, x2.j, x2.D);
			p2++;
			if (p2 <= n2) {
				fscanf(fin2, "%lld %lld %lld", &x2.i, &x2.j, &x2.D);
			}
		}
		else {
			if (x1.D >= x2.D) {
				fprintf(fout, "%lld %lld %lld\n", x1.i, x1.j, x1.D);
				p1++;
				if (p1 <= n1) {
					fscanf(fin1, "%lld %lld %lld", &x1.i, &x1.j, &x1.D);
				}
			}
			else {
				fprintf(fout, "%lld %lld %lld\n", x2.i, x2.j, x2.D);
				p2++;
				if (p2 <= n2) {
					fscanf(fin2, "%lld %lld %lld", &x2.i, &x2.j, &x2.D);
				}
			}
		}
	}
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
}

int main(void) {
	FILE* fin = fopen("C:\\Users\\hbp5148\\Desktop\\Slidings\\Points.txt", "r");
	ll n;
	fscanf(fin, "%lld", &n);
	ll pointsPerFile = n / numBlocks, rest = n % numBlocks;
	for (int i = 1; i <= numBlocks; i++) {
		NamingFile(fname, 1, i);
		FILE* fout = fopen(fname, "w");
		ll numofPoints = pointsPerFile + ((i <= rest) ? 1 : 0);
		fprintf(fout, "%d\n", numofPoints);
		vec.clear();
		for (ll j = 1; j <= numofPoints; j++) {
			fscanf(fin, "%lld %lld %lld", &x, &y, &d2);
			vec.push_back({ d2, x, y });
		}
		std::sort(vec.begin(), vec.end(), [](const data& a, const data& b) {
			return a.D > b.D;
			});
		for (auto v : vec) {
			fprintf(fout, "%lld %lld %lld\n", v.i, v.j, v.D);
		}
		fclose(fout);
	}

	fclose(fin);
	for (int i = 1; i <= level; i++) { // level
		for (int j = 1; j <= numBlocks / (1 << (i - 1)); j += 2) { // number of files in the ith level
			NamingFile(fname1, i, j);
			NamingFile(fname2, i, j + 1);
			NamingFile(fname, i + 1, (j + 1) / 2);
			merge(fname1, fname2, fname);
		}
	}
	return false;
}