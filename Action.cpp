#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Cube.h"

int main() {
	Rubik_Cube cube;
	std::ofstream tests;
	tests.open("Test results.txt");
	cube.Test(tests);

	cube.Random_Generate();
	cube.Rotate_Myself();
}