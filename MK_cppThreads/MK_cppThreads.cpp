#include <iostream>
#include <thread>
#include <ctime> 
#include <string>
#include <vector>
#include "stdio.h"

#define THREADS 4
#define PI 3.1415

using namespace std;

vector<vector<float>> FillArr();
int allRandom(unsigned int &seed);
float RndKoeff(int a, int b, unsigned int &seed);
float g(int a, int b);
void MK(long int thr);

vector<float> sol(THREADS);
int an = 10, bn = 10, inn = 5, jnn = 5;
long int N = 10000000;

vector<vector<float>> ar = FillArr();

int main()
{
	thread th[THREADS];

	float beg = clock();
	for (long int i = 0; i < THREADS; i++)
	{
		th[i] = thread(MK, i);
	}
	for (int i = 0; i < THREADS; i++)
	{
		th[i].join();
	}

	float fullSol = 0;

	for (int i = 0; i < THREADS; i++)
	{
		fullSol += sol[i];
	}

	printf("Solution at point [%i, %i] = %g\n", inn, jnn, fullSol);
	printf("time = %g\n", (clock() - beg) / 1000.0);

	return 0;
}

int allRandom(unsigned int &seed) {
	seed = (8253729 * seed + 2396403);

	// Берем стартовое число и возвращаем значение в диапазоне от 0 до 3
	return (seed * 32768) / 32767 % 4;
}

float RndKoeff(int a, int b, unsigned int &seed) {
	while (a != 0 && a != an - 1 && b != 0 && b != bn - 1) {
		int rnd = allRandom(seed);
		if (rnd == 0) {
			a--;
		}
		if (rnd == 1) {
			a++;
		}
		if (rnd == 2) {
			b--;
		}
		if (rnd == 3) {
			b++;
		}
	}
	return ar[a][b];
}

float g(int a, int b) {
	return sin(a + PI / 2) * sin(3 * b + PI / 2);
}

vector<vector<float>> FillArr() {
	vector<vector<float>> ar(an, vector <float>(bn));
	for (int i = 0; i < an; i++) {
		for (int j = 0; j < bn; j++) {
			if (i != 0 && i != an - 1 && j != 0 && j != bn - 1) {
				ar[i][j] = 0;
			}
			else {
				ar[i][j] = g(i, j);
			}
		}
	}
	return ar;
}

void MK(long int thr) {
	float ret = 0;
	unsigned int seed = thr + 1567;
	for (long int k = thr; k < N; k += THREADS) {
		ret += RndKoeff(inn, jnn, seed);
	}
	sol[thr] = ret / N;
}
