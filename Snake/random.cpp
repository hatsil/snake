#include <random>

#include "random.hpp"

std::random_device rd;

float snake::random() {
	double number = rd();
	double ans = number / rd.max();
	return (float)ans;
}