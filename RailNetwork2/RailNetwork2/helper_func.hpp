#pragma once
#include <vector>
#include <algorithm>
using namespace std;

// минимальный элемент вектора без учета индексов в except
template <typename T>
auto excep_min(vector <T>& A, vector <int> except) {
	for (unsigned i = 0; i < except.size(); ++i) {
		if (except[i] < 0 or except[i] >= A.size()) except.erase(except.begin() + i);
	}
	T temp(100000000);
	auto pointer = A.end();
	for (unsigned i = 0; i < A.size(); ++i) {
		bool excepted = false;
		for (auto now : except) {
			if (i == now) excepted = true;
		}
		if (!excepted) {
			if (A[i] < temp) {
				temp = A[i];
				pointer = A.begin() + i;
			}
		}
	}
	return pointer;
}
// ????максимальный элемент вектора без учета индексов в except
template <typename T>
T excep_max(const vector <T>& A, vector <int>& except) {
	vector <T> B = A;
	for (unsigned i = 0; i < except.size(); ++i) {
		if (except[i] < 0) except.erase(except.begin() + i);
		else if (except[i] >= A.size()) continue;
		else B.erase(B.begin() + except[i]);
	}
	return *max_element(B.begin(), B.end());
}