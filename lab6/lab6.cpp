#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

using namespace std;

const int l = 1000, m = 500, n = 500;

void fill(vector<vector<int>>& x, int k)
{
	srand(k);
	for (auto& row : x)
	{
		for (auto& cell : row)
		{
			cell = rand() % 100;
		}
	}
}

vector<vector<int>> MatrixMulty(const vector<vector<int>>& a, const vector<vector<int>>& b, int numTr)
{
	vector<vector<int>> res(l, vector<int>(n));
	#pragma omp parallel for num_threads(numTr)
	for (int k = 0; k < l; k++)
	{
		//std::cout << omp_get_num_threads() << ' ';
		//std::cout << std::endl;
		for (int j = 0; j < m; j++) {
		
			for (int i = 0; i < n; i++)
			{
				res[k][i] += a[k][j] * b[j][i];
			}
		}
	}

	return res;
}

int main()
{
	vector<vector<int>> a(l, vector<int>(m));
	vector<vector<int>> b(m, vector<int>(n));
	fill(a, 1);
	fill(b, 2);

	auto begin_time = clock();
	auto c = MatrixMulty(a, b, 8);
	auto end_time = clock();
	cout << "Time: " << (end_time - begin_time) / CLOCKS_PER_SEC << " sec" << endl;

	//begin_time = clock();
	//c = MatrixMulty(a, b, 1);
	//end_time = clock();
	//cout << "Time: " << (end_time - begin_time) / CLOCKS_PER_SEC << " sec" << endl;

	return 0;
}
