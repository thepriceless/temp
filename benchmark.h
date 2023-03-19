#pragma once

#include <string>
#include <chrono>
#include <functional>
#include <set>
#include <list>
#include <map>

#include "Writing.h"


class Benchmark {
public:

	typedef std::chrono::duration<double> duration;

	template <class T>
	void Compare_Allocator_With_std(Allocator<T>& allocator, std::ostream& output) {
		std::allocator<T> origin;

		Testing_on_Containers<Allocator<T>, std::allocator<T>>(allocator, origin,
			[this](Allocator<T>& allocator) {return Test_on_Vector(allocator); },
			[this](std::allocator<T>& origin) {return Test_on_Vector(origin); },
			"std::vector", output); //vector

		Testing_on_Containers<Allocator<T>, std::allocator<T>>(allocator, origin,
			[this](Allocator<T>& allocator) {return Test_on_Set(allocator); },
			[this](std::allocator<T>& origin) {return Test_on_Set(origin); },
			"std::set", output); //set

		Testing_on_Containers<Allocator<T>, std::allocator<T>>(allocator, origin,
			[this](Allocator<T>& allocator) {return Test_on_List(allocator); },
			[this](std::allocator<T>& origin) {return Test_on_List(origin); },
			"std::list", output); //list

		Testing_on_Containers<Allocator<T>, std::allocator<T>>(allocator, origin,
			[this](Allocator<T>& allocator) {return Test_on_Map(allocator); },
			[this](std::allocator<T>& origin) {return Test_on_Map(origin); },
			"std::map", output); //map
	}


	template <class All, class stdAll>
	void Testing_on_Containers(All& allocator, stdAll& origin,
		std::function<duration(All&)> custom_test_func, std::function<duration(stdAll&)> std_test_func,
		std::string name, std::ostream& output, int amount_of_tests = 20) {

		duration std_time, custom_time;
		double total_ratio = 0, min_ratio = 1000, max_ratio = -1000;

		for (int test_num = 1; test_num < amount_of_tests + 1; ++test_num) {
			std_time = std_test_func(origin);
			custom_time = custom_test_func(allocator);
			std::cout << "\nstandard: " << std_time.count() << ", custom: " << custom_time.count();
			total_ratio += (custom_time / std_time);
			min_ratio = std::min(min_ratio, custom_time / std_time);
			max_ratio = std::max(max_ratio, custom_time / std_time);
		}

		printer.Print_Results(amount_of_tests, total_ratio, min_ratio, max_ratio, output, name);
	}


private:

	Letter_Teller printer;


	template <class All>
	duration Test_on_Vector(All& allocator) {
		auto start_time = std::chrono::steady_clock::now();
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

		return duration(std::chrono::steady_clock::now() - start_time);
	}

	template <class All>
	duration Test_on_Set(All& allocator) {
		auto start_time = std::chrono::steady_clock::now();
		std::set<std::string, std::less<std::string>, All> test_set(allocator);

		for (int att = 0; att < 25; ++att) {
			for (int i = 0; i < 2000; ++i) {
				test_set.insert(std::to_string(i));
			}
		}

		return duration(std::chrono::steady_clock::now() - start_time);
	}

	template <class All>
	duration Test_on_List(All& allocator) {
		auto start_time = std::chrono::steady_clock::now();
		std::list<char, All> test_list(allocator);

		for (int att = 0; att < 25; ++att) {
			for (int i = 6; i < 2000; ++i) {
				if (i % 2 == 0) {
					test_list.push_back('f');
				}
				else {
					test_list.push_front('r');
				}
			}
			test_list.clear();
		}

		return duration(std::chrono::steady_clock::now() - start_time);
	}

	template <class All>
	duration Test_on_Map(All& allocator) {
		auto start_time = std::chrono::steady_clock::now();
		std::map<std::string, int, std::less<std::string>, All> test_map(allocator);
		
		for (int att = 0; att < 25; ++att) {
			for (int i = 1; i < 2000; ++i) {
				test_map.insert(std::pair<std::string, int>(std::to_string(i), 28));
			}
			test_map.clear();
		}

		return duration(std::chrono::steady_clock::now() - start_time);
	}
};
