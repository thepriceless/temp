#pragma once
#include <algorithm>
#include <ctime>
#include <chrono>
#include <functional>
#include <set>
#include <list>
#include <map>
#include <string>

class Benchmark {
public:
	template <class T>
	void Compare_Allocator_With_std(Allocator<T>& allocator, std::ostream& output) {
		std::allocator<T> origin;

		auto testFunc = [this](Allocator<T>& allocator)
		{
			unsigned int start_time = clock();
			std::vector<int, Allocator<T>> test_vector(allocator);

			for (int att = 0; att < 25; ++att) {
				for (int i = 1; i < 100000; ++i) {
					if (i % 2 == 0) {
						test_vector.push_back(56);
					}
					else {
						test_vector.resize(i);
					}
				}
				test_vector.clear();
			}
			return clock() - start_time;
			//return Test_on_Vector(allocator);
		};


		auto testFunc2 = [this](std::allocator<T>& origin)
		{
			return Test_on_Vector(origin);
		};

		Vector_Testing<Allocator<T>, std::allocator<T>>(allocator, origin, testFunc, testFunc2, output);
		Set_Testing(allocator, origin, output);
		List_Testing(allocator, origin, output);
		Map_Testing(allocator, origin, output);
	}


	template <class All, class stdAll>
	void Vector_Testing(All& allocator, stdAll& origin, std::function<unsigned int(All&)> custom_test_func, std::function<unsigned int(stdAll&)> std_test_func, std::ostream& output, int amount_of_tests = 20) {
		int std_time, custom_time;
		double total_ratio = 0, min_ratio = 1000, max_ratio = -1000;

		for (int test_num = 1; test_num < amount_of_tests + 1; ++test_num) {
			std_time = std_test_func(origin);
			custom_time = custom_test_func(allocator);
			std::cout << "\nstandard: " << std_time << ", custom: " << custom_time;
			total_ratio += static_cast<double>(static_cast<double>(custom_time) / static_cast<double>(std_time));
			min_ratio = std::min(min_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
			max_ratio = std::max(max_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
		}

		Print_Results(amount_of_tests, total_ratio, min_ratio, max_ratio, output, "std::vector");
	}


	template <class All, class stdAll>
	void Set_Testing(All& allocator, stdAll& origin, std::ostream& output, int amount_of_tests = 20) {
		int std_time, custom_time;
		double total_ratio = 0, min_ratio = 1000, max_ratio = -1000;

		for (int test_num = 1; test_num < amount_of_tests + 1; ++test_num) {
			std_time = Test_on_Set(origin);
			custom_time = Test_on_Set(allocator);
			std::cout << "\nstandard: " << std_time << ", custom: " << custom_time;
			total_ratio += static_cast<double>(static_cast<double>(custom_time) / static_cast<double>(std_time));
			min_ratio = std::min(min_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
			max_ratio = std::max(max_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
		}

		Print_Results(amount_of_tests, total_ratio, min_ratio, max_ratio, output, "std::set");
	}


	template <class All, class stdAll>
	void List_Testing(All& allocator, stdAll& origin, std::ostream& output, int amount_of_tests = 20) {
		int std_time, custom_time;
		double total_ratio = 0, min_ratio = 1000, max_ratio = -1000;

		for (int test_num = 1; test_num < amount_of_tests + 1; ++test_num) {
			std_time = Test_on_List(origin);
			custom_time = Test_on_List(allocator);
			std::cout << "\nstandard: " << std_time << ", custom: " << custom_time;
			total_ratio += static_cast<double>(static_cast<double>(custom_time) / static_cast<double>(std_time));
			min_ratio = std::min(min_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
			max_ratio = std::max(max_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
		}

		Print_Results(amount_of_tests, total_ratio, min_ratio, max_ratio, output, "std::list");
	}

	template <class All, class stdAll>
	void Map_Testing(All& allocator, stdAll& origin, std::ostream& output, int amount_of_tests = 20) {
		int std_time, custom_time;
		double total_ratio = 0, min_ratio = 1000, max_ratio = -1000;

		for (int test_num = 1; test_num < amount_of_tests + 1; ++test_num) {
			std_time = Test_on_Map(origin);
			custom_time = Test_on_Map(allocator);
			std::cout << "\nstandard: " << std_time << ", custom: " << custom_time;
			total_ratio += static_cast<double>(static_cast<double>(custom_time) / static_cast<double>(std_time));
			min_ratio = std::min(min_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
			max_ratio = std::max(max_ratio, static_cast<double>(custom_time) / static_cast<double>(std_time));
		}

		Print_Results(amount_of_tests, total_ratio, min_ratio, max_ratio, output, "std::map");
	}

private:
	template <class All>
	unsigned int Test_on_Vector(All& allocator) {
		unsigned int start_time = clock();
		std::vector<int, All> test_vector(allocator);

		for (int att = 0; att < 25; ++att) {
			for (int i = 1; i < 100000; ++i) {
				if (i % 2 == 0) {
					test_vector.push_back(56);
				}
				else {
					test_vector.resize(i);
				}
			}
			test_vector.clear();
		}
		return clock() - start_time;
	}

	template <class All>
	unsigned int Test_on_Set(All& allocator) {
		unsigned int start_time = clock();
		std::set<std::string, std::less<std::string>, All> test_set(allocator);
		std::set<std::string, std::less<std::string>, All> test_set_S(allocator);

		for (int att = 0; att < 25; ++att) {
			for (int i = 0; i < 100; ++i) {
				test_set.insert("fly Dubai");
				test_set_S.insert("it makes a world of difference");
			}
		}
		return clock() - start_time;
	}

	template <class All>
	unsigned int Test_on_List(All& allocator) {
		unsigned int start_time = clock();
		std::list<char, All> test_list(allocator);

		for (int att = 0; att < 25; ++att) {
			for (int i = 6; i < 1000; ++i) {
				if (i % 2 == 0) {
					test_list.push_back('f');
				}
				else {
					test_list.push_front('f');
				}
			}
			test_list.clear();
		}
		return clock() - start_time;
	}

	template <class All>
	unsigned int Test_on_Map(All& allocator) {
		unsigned int start_time = clock();
		std::map<std::string, int, std::less<std::string>, All> test_map(allocator);
		
		for (int att = 0; att < 25; ++att) {
			for (int i = 1; i < 1000; ++i) {
				test_map.insert(std::pair<std::string, int>(std::to_string(i), 28));
			}
			test_map.clear();
		}
		return clock() - start_time;
	}


	void Print_Results(int Tamount, double ratio, double min_ratio, double max_ratio, std::ostream& output, std::string name) {
		output << "\nTesting (on " << name << ") is finished, " << Tamount << " tests have been run.\nstd::allocator works (on average) ";
		if (Print_Ratio(ratio / Tamount, output)) {
			output << "faster than the custom Allocator.";
		}
		else {
			output << "slower than the custom Allocator.";
		}

		output << "\nIn the fastest test custom Allocator performs ";
		if (Print_Ratio(min_ratio, output)) {
			output << "worse than std::allocator.";
		}
		else {
			output << "better than std::allocator.";
		}

		output << "\nIn the slowest test custom Allocator performs ";
		if (Print_Ratio(max_ratio, output)) {
			output << "worse than std::allocator.\n";
		}
		else {
			output << "better than std::allocator.\n";
		}
	}


	bool Print_Ratio(double ratio, std::ostream& output) {
		if (ratio >= 1) {
			output << (ratio - 1) * 100 << " % ";
			return true;
		}
		else {
			output << (1 - ratio) * 100 << " % ";
			return false;
		}
	}
};
