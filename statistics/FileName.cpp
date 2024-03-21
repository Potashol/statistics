#include <cmath>
#include <random>
#include <iostream>
#include <functional>

#define PI 3.14159265358979323846

namespace stat_mod_1
{
	const int MIN_ITER_COUNT = 10000;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution <> urd(0, 1);
	double uniform()
	{
		return urd(gen);
	}

	int random_value()
	{
		int k = 1;
		double a = uniform();
		double p = (90 / pow(PI, 4));
		a = a - p;
		while (a > 0)
		{
			a = a - p;
			++k;
			p = p * pow(k / (k + 1), 4);
		}
		return k;
	}

	double expected_value(std::function <double()> rand_distr, double precision, int
		min_iter_count = MIN_ITER_COUNT)
	{
		double sum = 0;
		double square_sum = 0;
		double n = 0;
		bool is_precision_obtained = false;
		while (!is_precision_obtained)
		{
			double r = rand_distr();
			sum += r;
			square_sum += (r * r);
			n++;
			if (n < min_iter_count)
				continue;
			if (n == min_iter_count)
				break;
			double dispersion = (1.0 / (n - 1)) * square_sum - (1.0 / (n * (n
				- 1))) * sum * sum;
			double estimated_iter_count = 9 * dispersion / (precision *
				precision);
			is_precision_obtained = (n > estimated_iter_count);
		}
		return sum / n;
	}

	double calculated_value_1()
	{
		return expected_value(random_value, 0.01);
	}

	double calculated_value_2() {
		auto random_value2 = [&]()->double {
			auto n = random_value();
			return (1 / (pow(n, 2) + 1) / 90 * pow(PI, 4));
		};
		return expected_value(random_value2, 0.01);
	}

	double multiple_integral_value(double a, double b)
	{
		auto rand_value = [=]() -> double
		{
			double z = uniform() * (a - 1) + 1;
			return 2 * PI / std::sqrt(z) * cos(b * z) * (a - 1);
		};
		return expected_value(rand_value, 0.001);
	}
}

int main()
{
	std::cout << stat_mod_1::random_value() << std::endl;
	std::cout << stat_mod_1::calculated_value_1() << std::endl;
	std::cout << stat_mod_1::calculated_value_2() << std::endl;
	std::cout << stat_mod_1::multiple_integral_value(2, 4) << std::endl;
	std::cout << stat_mod_1::multiple_integral_value(3, 5) << std::endl;
}
