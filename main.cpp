#include <iostream>
#include <fstream>
#include <vector>

#include "Structures.h"
#include "Allocator.h"
#include "Benchmark.h"


int main() {
	size_t* sizez = new size_t[3]{2000, 1000, 10};
	size_t* amountz = new size_t[3]{5, 15, 1000};
	Allocator<int> all(sizez, amountz, 3);

	size_t* BigS = new size_t[12]{8000, 8000000, 4000000, 2000000, 1000000, 500000, 250000, 125000, 64000, 32000, 16000, 16000000};
	size_t* BigA = new size_t[12]{2048, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 1};
	Allocator<int> Huge_Allocator(BigS, BigA, 12);

	std::ofstream Output_Stream;
	Output_Stream.open("Test results.txt");

	Benchmark Speed_Comparison;
	Speed_Comparison.Compare_Allocator_With_std(Huge_Allocator, Output_Stream);
	return 0;	
}
