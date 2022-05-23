#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include "../Topic_1/log_duration.hpp"

int theory_1()
{
	// Даны кошки и собаки
	std::vector<float> cats(1'000'000'000);
	std::vector<float> dogs(1'000'000'000);

	// Вычисляем их колличество (общее)	
	
	{ // Последовательная реализация
		LOG_DURATION("seq");
		auto sum = std::reduce(cats.begin(), cats.end());
	}

	{ // Ассинхронная реализация
		LOG_DURATION("async");
		auto cats_sum = 
			std::async([&cats] {return std::reduce(cats.begin(), cats.end()); });

		auto dogs_sum = std::reduce(dogs.begin(), dogs.end());

		auto sum = cats_sum.get() + dogs_sum;
	}

	return 0;
}