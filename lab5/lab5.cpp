/*
* 1. –еализовать скал€рное произведение векторов (Reduction)
*/

#include <iostream>
#include <omp.h>
#include <chrono>

int main()
{
	std::srand(std::time(nullptr));
	const int N = 3;
	int result = 0;
	
	int* vec1 = new int[N];
	int* vec2 = new int[N];

	for (int i = 0; i < N; i++) {
		vec1[i] = rand() % 10;
		vec2[i] = rand() % 10;
	}

	for (int i = 0; i < N; ++i) {
		std::cout << vec1[i] << ' ';
	}
	std::cout << std::endl;

	for (int i = 0; i < N; ++i) {
		std::cout << vec2[i] << ' ';
	}
	std::cout << std::endl;

	#pragma omp parallel for shared(vec1, vec2) reduction(+:result)
	for (int i = 0; i < N; i++) {
		result += vec1[i] * vec2[i];
	}

	std::cout << "result = " << result << std::endl;
	
	delete[] vec1;
	delete[] vec2;
}