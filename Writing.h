#pragma once

class Letter_Teller {
public:
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
