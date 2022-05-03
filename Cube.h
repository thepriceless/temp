#pragma once
#include <set>
#include "Side.h"

struct One_Colored_Cell {
	int color;
	bool exist = false;
};

struct Two_Colored_Cell {
	std::set<int> colors;
	bool exist = false;
};

struct Three_Colored_Cell {
	std::set<int> colors;
	bool exist = false;
};

class Rubik_Cube {
private:
	int size;
	Side front;
	Side back;
	Side left;
	Side right;
	Side top;
	Side bottom;
	int error_rotations = 0;
	int Sides_Rotations = 0;
	int Cube_Rotations = 0;

	bool Check_Edge(const Cell& good_side_cell, const Cell& bad_side_cell) {
		if (good_side_cell == 0 || good_side_cell == 5) {
			return true;
		}
		if (bad_side_cell == 0 || bad_side_cell == 5) {
			return false;
		}
		if (good_side_cell == 1 || good_side_cell == 4) {
			return true;
		}
		if (good_side_cell == 1 || good_side_cell == 4) {
			return false;
		}
	}

	int Check_Corner(const Cell& top_cell, const Cell& right_cell, const Cell left_cell) const {
		if (top_cell == 0 || top_cell == 5) {
			return 0;
		}
		if (left_cell == 0 || left_cell == 5) {
			return 1;
		}
		return 2;
	}

	void Bottom_White_Edges() {
		error_rotations += 3;
		if (error_rotations > 200) {
			Shake();
		}
		if (bottom[0][1] == 0) {
			if (front[2][1] == 1) {
				Rotate_Bottom_Clockwise();
				Rotate_Bottom_Clockwise();
				Rotate_Back_Clockwise();
				Rotate_Back_Clockwise();
			}
			else if (front[2][1] == 2) {
				Rotate_Bottom_Clockwise();
				Rotate_Right_Clockwise();
				Rotate_Right_Clockwise();
			}
			else if (front[2][1] == 3) {
				Rotate_Bottom_CounterClockwise();
				Rotate_Left_Clockwise();
				Rotate_Left_Clockwise();
			}
			else {
				Rotate_Front_Clockwise();
				Rotate_Front_Clockwise();
			}
			Bottom_White_Edges();
		}
		if (bottom[1][2] == 0) {
			if (right[2][1] == 1) {
				Rotate_Bottom_Clockwise();
				Rotate_Back_Clockwise();
				Rotate_Back_Clockwise();
			}
			else if (right[2][1] == 2) {
				Rotate_Right_Clockwise();
				Rotate_Right_Clockwise();
			}
			else if (right[2][1] == 3) {
				Rotate_Bottom_CounterClockwise();
				Rotate_Bottom_CounterClockwise();
				Rotate_Left_Clockwise();
				Rotate_Left_Clockwise();
			}
			else {
				Rotate_Bottom_CounterClockwise();
				Rotate_Front_Clockwise();
				Rotate_Front_Clockwise();
			}
			Bottom_White_Edges();
		}
		if (bottom[2][1] == 0) {
			if (back[2][1] == 1) {
				Rotate_Back_Clockwise();
				Rotate_Back_Clockwise();
			}
			else if (back[2][1] == 2) {
				Rotate_Bottom_CounterClockwise();
				Rotate_Right_Clockwise();
				Rotate_Right_Clockwise();
			}
			else if (back[2][1] == 3) {
				Rotate_Bottom_Clockwise();
				Rotate_Left_Clockwise();
				Rotate_Left_Clockwise();
			}
			else {
				Rotate_Bottom_CounterClockwise();
				Rotate_Bottom_CounterClockwise();
				Rotate_Front_Clockwise();
				Rotate_Front_Clockwise();
			}
			Bottom_White_Edges();
		}
		if (bottom[1][0] == 0) {
			if (left[2][1] == 1) {
				Rotate_Bottom_CounterClockwise();
				Rotate_Back_Clockwise();
				Rotate_Back_Clockwise();
			}
			else if (left[2][1] == 2) {
				Rotate_Bottom_CounterClockwise();
				Rotate_Bottom_CounterClockwise();
				Rotate_Right_Clockwise();
				Rotate_Right_Clockwise();
			}
			else if (left[2][1] == 3) {
				Rotate_Left_Clockwise();
				Rotate_Left_Clockwise();
			}
			else {
				Rotate_Bottom_Clockwise();
				Rotate_Front_Clockwise();
				Rotate_Front_Clockwise();
			}
			Bottom_White_Edges();
		}
	}

	void FLBR_White_Edges() {  //FrontLeftBackRight 
		++error_rotations;
		if (error_rotations > 200) {
			Shake();
		}
		if (front[1][2] == 0) {
			Rotate_Right_CounterClockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (front[1][0] == 0) {
			Rotate_Left_Clockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (left[1][2] == 0) {
			Rotate_Front_CounterClockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (left[1][0] == 0) {
			Rotate_Back_Clockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (back[1][2] == 0) {
			Rotate_Left_CounterClockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (back[1][0] == 0) {
			Rotate_Right_Clockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (right[1][2] == 0) {
			Rotate_Back_CounterClockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}
		if (right[1][0] == 0) {
			Rotate_Front_Clockwise();
			Bottom_White_Edges();
			FLBR_White_Edges();
		}

		if (front[0][1] == 0 || front[2][1] == 0) {
			Rotate_Front_Clockwise();
			FLBR_White_Edges();
		}

		if (left[0][1] == 0 || left[2][1] == 0) {
			Rotate_Left_Clockwise();
			FLBR_White_Edges();
		}
		if (back[0][1] == 0 || back[2][1] == 0) {
			Rotate_Back_Clockwise();
			FLBR_White_Edges();
		}
		if (right[0][1] == 0 || right[2][1] == 0) {
			Rotate_Right_Clockwise();
			FLBR_White_Edges();
		}
	}

	void Top_Incorrect_White_Edges() {
		++error_rotations;
		if (error_rotations > 200) {
			Shake();
		}
		if (top[0][1] == 0 && back[0][1] != 1) {
			Rotate_Back_Clockwise();
			FLBR_White_Edges();
		}
		if (top[1][2] == 0 && right[0][1] != 2) {
			Rotate_Right_Clockwise();
			FLBR_White_Edges();
		}
		if (top[2][1] == 0 && front[0][1] != 4) {
			Rotate_Front_Clockwise();
			FLBR_White_Edges();
		}
		if (top[1][0] == 0 && left[0][1] != 3) {
			Rotate_Left_Clockwise();
			FLBR_White_Edges();
		}
	}

	bool Lift_From_Bottom() {
		if (bottom[0][2] == 0 || front[2][2] == 0 || right[2][0] == 0) {
			if (!(bottom[0][2] == 0 && front[2][2] == 4 && right[2][0] == 3)) {
				Pif_Puff();
				return true;
			}
		}
		if (bottom[0][0] == 0 || front[2][0] == 0 || left[2][2] == 0) {
			if (!(bottom[0][0] == 0 && front[2][0] == 4 && left[2][2] == 2)) {
				Z_Rotate_Clockwise();
				Pif_Puff();
				Z_Rotate_CounterClockwise();
				return true;
			}
		}
		if (bottom[2][0] == 0 || back[2][2] == 0 || left[2][0] == 0) {
			if (!(bottom[2][0] == 0 && back[2][2] == 1 && left[2][0] == 2)) {
				Z_Rotate_Clockwise();
				Z_Rotate_Clockwise();
				Pif_Puff();
				Z_Rotate_CounterClockwise();
				Z_Rotate_CounterClockwise();
				return true;
			}
		}
		if (bottom[2][2] == 0 || back[2][0] == 0 || right[2][2] == 0) {
			if (!(bottom[2][2] == 0 && back[2][0] == 1 && right[2][2] == 3)) {
				Z_Rotate_CounterClockwise();
				Pif_Puff();
				Z_Rotate_Clockwise();
				return true;
			}
		}
		return false;
	}

	void Prepare_Corner() {
		int rotations_to_cycle = 0;
		while (top[2][2] != 0 && front[0][2] != 0 && right[0][0] != 0 && rotations_to_cycle < 4) {
			Rotate_Top_Clockwise();
			++rotations_to_cycle;
		}
		if (rotations_to_cycle == 4) {
			if (Lift_From_Bottom()) {
				Prepare_Corner();
			}
		}
	}

	void TL_Insert_Cell() {
		if (Prepare_Edge()) {
			Two_Layers_Combo();
			if (front[1][0] == left[1][1] && left[1][2] == front[1][1]) {
				Two_Layers_Combo();
				Rotate_Bottom_CounterClockwise();
				Two_Layers_Combo();
			}
		}
	}

	bool Prepare_Edge() {
		int rotations_to_cycle = 0;
		while (!((right[2][1] == front[1][1] && bottom[1][2] == left[1][1]) || (right[2][1] == left[1][1] && bottom[1][2] == front[1][1])) && rotations_to_cycle < 4) {
			Rotate_Bottom_CounterClockwise();
			++rotations_to_cycle;
		}
		if (rotations_to_cycle == 4) {
			if (Drop_To_Bottom()) {
				return Prepare_Edge();
			}
			return false;
		}
		return true;
	}

	bool Drop_To_Bottom() {
		if (front[1][0] == left[1][1] && left[1][2] == front[1][1]) {
			Two_Layers_Combo();
			return true;
		}
		if (front[1][0] == front[1][1] && left[1][2] == left[1][1]) {
			return false;
		}
		if ((left[1][0] == left[1][1] && back[1][2] == front[1][1]) || (left[1][0] == front[1][1] && back[1][2] == left[1][1])) {
			Z_Rotate_Clockwise();
			Two_Layers_Combo();
			Z_Rotate_CounterClockwise();
			return true;
		}
		if ((back[1][0] == left[1][1] && right[1][2] == front[1][1]) || (back[1][0] == front[1][1] && right[1][2] == left[1][1])) {
			Z_Rotate_Clockwise();
			Z_Rotate_Clockwise();
			Two_Layers_Combo();
			Z_Rotate_CounterClockwise();
			Z_Rotate_CounterClockwise();
			return true;
		}
		if ((right[1][0] == left[1][1] && front[1][2] == front[1][1]) || (right[1][0] == front[1][1] && front[1][2] == left[1][1])) {
			Z_Rotate_CounterClockwise();
			Two_Layers_Combo();
			Z_Rotate_Clockwise();
			return true;
		}
	}

	void Top_Side_Positioning() {
		while (front.Compare_Cells() + right.Compare_Cells() + left.Compare_Cells() + back.Compare_Cells() < 2) {
			Rotate_Top_Clockwise();
		}
	}

	bool Cube_OSC_Positioning() {
		if (!front.Compare_Cells() && (!right.Compare_Cells() || !back.Compare_Cells())) {
			Z_Rotate_CounterClockwise();
			return true;
		}
		else if (!left.Compare_Cells() && !back.Compare_Cells()) {
			Z_Rotate_Clockwise();
			return true;
		}
		else if (!back.Compare_Cells() && !right.Compare_Cells()) {
			Z_Rotate_Clockwise();
			Z_Rotate_Clockwise();
			return true;
		}
		if (front.Compare_Cells() && right.Compare_Cells() && left.Compare_Cells() && back.Compare_Cells()) {
			return false;
		}
		return true;
	}

	void Cube_Corners_Positioning() {
		Pif_Puff_Positioning();
		int rotations_to_cycle = 0;
		while (!((top[0][0] == 5 || left[0][0] == 5 || back[0][2] == 5) && (top[0][0] == left[1][1] || left[0][0] == left[1][1] || back[0][2] == left[1][1])
			&& (top[0][0] == back[1][1] || left[0][0] == back[1][1] || back[0][2] == back[1][1])) && rotations_to_cycle < 4) {
			Z_Rotate_CounterClockwise();
			++rotations_to_cycle;
		}
	}

public:
	Rubik_Cube(int side_size = 3) : size(side_size),
		front(Side("front")), back(Side("back")),
		left(Side("left")), right(Side("right")),
		top(Side("top")), bottom(Side("bottom")) {}

	Rubik_Cube(const Rubik_Cube& other) : size(other.size),
		front(other.front), back(other.back),
		left(other.left), right(other.right),
		bottom(other.bottom), top(other.top) {}

	void Read_From_File(const char* filename) {
		int side_size, temp_color;
		std::string side;
		std::ifstream File_Input;
		File_Input.open(filename);
		File_Input >> side_size;
		for (int i = 0; i < 6; ++i) {
			File_Input >> side;
			for (int row = 0; row < side_size; ++row) {
				for (int col = 0; col < side_size; ++col) {
					File_Input >> temp_color;
					if (side == "front") { front[row][col] = temp_color; }
					else if (side == "left") { left[row][col] = temp_color; }
					else if (side == "back") { back[row][col] = temp_color; }
					else if (side == "right") { right[row][col] = temp_color; }
					else if (side == "top") { top[row][col] = temp_color; }
					else if (side == "bottom") { bottom[row][col] = temp_color; }
					else { std::cout << "Incorrect side name"; }
				}
			}
		}
	}

	void Fout(const char* filename) const {
		std::ofstream File_Output;
		File_Output.open(filename);
		File_Output << size << '\n';
		front.Print_Side(File_Output);
		left.Print_Side(File_Output);
		back.Print_Side(File_Output);
		right.Print_Side(File_Output);
		bottom.Print_Side(File_Output);
		top.Print_Side(File_Output);
	}

	void Cout() const {
		std::cout << size << '\n';
		front.Print_Side(std::cout);
		left.Print_Side(std::cout);
		back.Print_Side(std::cout);
		right.Print_Side(std::cout);
		bottom.Print_Side(std::cout);
		top.Print_Side(std::cout);
		std::cout << "\n\n";
	}

	void Random_Generate() {
		Read_From_File("Completed.txt");
		int rotate_type;
		srand(static_cast<unsigned int>(time(0)));
		for (int i = 0; i < 30; ++i) {
			rotate_type = rand() % 7;
			if (rotate_type == 0) {
				Shake();
			}
			else if (rotate_type == 1) {
				X_Rotate_Clockwise();
				Shake();
			}
			else if (rotate_type == 2) {
				X_Rotate_CounterClockwise();
				Shake();
			}
			else if (rotate_type == 3) {
				Y_Rotate_Clockwise();
				Shake();
			}
			else if (rotate_type == 4) {
				Y_Rotate_CounterClockwise();
				Shake();
			}
			else if (rotate_type == 5) {
				Z_Rotate_Clockwise();
				Shake();
			}
			else {
				Z_Rotate_CounterClockwise();
				Shake();
			}
		}
	}

	void Rotate_Myself() {
		std::string command;
		std::cin >> command;
		while (command != "stop") {
			if (command == "R") {
				Rotate_Right_Clockwise();
			}
			else if (command == "R'") {
				Rotate_Right_CounterClockwise();
			}
			else if (command == "L") {
				Rotate_Left_Clockwise();
			}
			else if (command == "L'") {
				Rotate_Left_CounterClockwise();
			}
			else if (command == "B") {
				Rotate_Back_Clockwise();
			}
			else if (command == "B'") {
				Rotate_Back_CounterClockwise();
			}
			else if (command == "F") {
				Rotate_Front_Clockwise();
			}
			else if (command == "F'") {
				Rotate_Front_CounterClockwise();
			}
			else if (command == "U") {
				Rotate_Top_Clockwise();
			}
			else if (command == "U'") {
				Rotate_Top_CounterClockwise();
			}
			else if (command == "D") {
				Rotate_Bottom_Clockwise();
			}
			else if (command == "D'") {
				Rotate_Bottom_CounterClockwise();
			}
			else if (command == "R2") {
				Rotate_Right_Clockwise();
				Rotate_Right_Clockwise();
			}
			else if (command == "L2") {
				Rotate_Left_Clockwise();
				Rotate_Left_Clockwise();
			}
			else if (command == "B2") {
				Rotate_Back_Clockwise();
				Rotate_Back_Clockwise();
			}
			else if (command == "F2") {
				Rotate_Front_Clockwise();
				Rotate_Front_Clockwise();
			}
			else if (command == "U2") {
				Rotate_Top_Clockwise();
				Rotate_Top_Clockwise();
			}
			else if (command == "D2") {
				Rotate_Bottom_Clockwise();
				Rotate_Bottom_Clockwise();
			}
			else {
				std::cout << "Unknown command\n";
			}
			std::cin >> command;
		}
	}


	void X_Rotate_Clockwise() {
		Side temp(front);
		front = top;
		top = back;
		top.Rotate_Clockwise();
		top.Rotate_Clockwise();
		back = bottom;
		back.Rotate_Clockwise();
		back.Rotate_Clockwise();
		bottom = temp;
		left.Rotate_Clockwise();
		right.Rotate_CounterClockwise();
		++Cube_Rotations;
	}

	void X_Rotate_CounterClockwise() {
		Side temp(front);
		front = bottom;
		bottom = back;
		bottom.Rotate_CounterClockwise();
		bottom.Rotate_CounterClockwise();
		back = top;
		back.Rotate_CounterClockwise();
		back.Rotate_CounterClockwise();
		top = temp;
		right.Rotate_Clockwise();
		left.Rotate_CounterClockwise();
		++Cube_Rotations;
	}

	void Y_Rotate_Clockwise() {
		Side temp(right);
		right = top;
		right.Rotate_Clockwise();
		top = left;
		top.Rotate_Clockwise();
		left = bottom;
		left.Rotate_Clockwise();
		bottom = temp;
		bottom.Rotate_Clockwise();
		front.Rotate_Clockwise();
		back.Rotate_CounterClockwise();
		++Cube_Rotations;
	}

	void Y_Rotate_CounterClockwise() {
		Side temp(right);
		right = bottom;
		right.Rotate_CounterClockwise();
		bottom = left;
		bottom.Rotate_CounterClockwise();
		left = top;
		left.Rotate_CounterClockwise();
		top = temp;
		top.Rotate_CounterClockwise();
		back.Rotate_Clockwise();
		front.Rotate_CounterClockwise();
		++Cube_Rotations;
	}

	void Z_Rotate_Clockwise() {
		Side temp(front);
		front = left;
		left = back;
		back = right;
		right = temp;
		bottom.Rotate_Clockwise();
		top.Rotate_CounterClockwise();
		++Cube_Rotations;
	}
	
	void Z_Rotate_CounterClockwise() {
		Side temp(front);
		front = right;
		right = back;
		back = left;
		left = temp;
		top.Rotate_Clockwise();
		bottom.Rotate_CounterClockwise();
		++Cube_Rotations;
	}

	void Rotate_Front_Clockwise() {
		front.Rotate_Clockwise();
		Cell temp_first(top[2][0]), temp_second(top[2][1]), temp_third(top[2][2]);
		top[2][0] = left[2][2];
		top[2][1] = left[1][2];
		top[2][2] = left[0][2];
		left[2][2] = bottom[0][2];
		left[1][2] = bottom[0][1];
		left[0][2] = bottom[0][0];
		bottom[0][2] = right[0][0];
		bottom[0][1] = right[1][0];
		bottom[0][0] = right[2][0];
		right[0][0] = temp_first;
		right[1][0] = temp_second;
		right[2][0] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Front_CounterClockwise() {
		front.Rotate_CounterClockwise();
		Cell temp_first(top[2][0]), temp_second(top[2][1]), temp_third(top[2][2]);
		top[2][0] = right[0][0];
		top[2][1] = right[1][0];
		top[2][2] = right[2][0];
		right[0][0] = bottom[0][2];
		right[1][0] = bottom[0][1];
		right[2][0] = bottom[0][0];
		bottom[0][2] = left[2][2];
		bottom[0][1] = left[1][2];
		bottom[0][0] = left[0][2];
		left[2][2] = temp_first;
		left[1][2] = temp_second;
		left[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Left_Clockwise() {
		left.Rotate_Clockwise();
		Cell temp_first(top[0][0]), temp_second(top[1][0]), temp_third(top[2][0]);
		top[0][0] = back[2][2];
		top[1][0] = back[1][2];
		top[2][0] = back[0][2];
		back[2][2] = bottom[0][0];
		back[1][2] = bottom[1][0];
		back[0][2] = bottom[2][0];
		bottom[0][0] = front[0][0];
		bottom[1][0] = front[1][0];
		bottom[2][0] = front[2][0];
		front[0][0] = temp_first;
		front[1][0] = temp_second;
		front[2][0] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Left_CounterClockwise() {
		left.Rotate_CounterClockwise();
		Cell temp_first(top[0][0]), temp_second(top[1][0]), temp_third(top[2][0]);
		top[0][0] = front[0][0];
		top[1][0] = front[1][0];
		top[2][0] = front[2][0];
		front[0][0] = bottom[0][0];
		front[1][0] = bottom[1][0];
		front[2][0] = bottom[2][0];
		bottom[0][0] = back[2][2];
		bottom[1][0] = back[1][2];
		bottom[2][0] = back[0][2];
		back[2][2] = temp_first;
		back[1][2] = temp_second;
		back[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Right_Clockwise() {
		right.Rotate_Clockwise();
		Cell temp_first(top[2][2]), temp_second(top[1][2]), temp_third(top[0][2]);
		top[2][2] = front[2][2];
		top[1][2] = front[1][2];
		top[0][2] = front[0][2];
		front[2][2] = bottom[2][2];
		front[1][2] = bottom[1][2];
		front[0][2] = bottom[0][2];
		bottom[2][2] = back[0][0];
		bottom[1][2] = back[1][0];
		bottom[0][2] = back[2][0];
		back[0][0] = temp_first;
		back[1][0] = temp_second;
		back[2][0] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Right_CounterClockwise() {
		right.Rotate_CounterClockwise();
		Cell temp_first(top[2][2]), temp_second(top[1][2]), temp_third(top[0][2]);
		top[2][2] = back[0][0];
		top[1][2] = back[1][0];
		top[0][2] = back[2][0];
		back[0][0] = bottom[2][2];
		back[1][0] = bottom[1][2];
		back[2][0] = bottom[0][2];
		bottom[2][2] = front[2][2];
		bottom[1][2] = front[1][2];
		bottom[0][2] = front[0][2];
		front[2][2] = temp_first;
		front[1][2] = temp_second;
		front[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Back_Clockwise() {
		back.Rotate_Clockwise();
		Cell temp_first(top[0][2]), temp_second(top[0][1]), temp_third(top[0][0]);
		top[0][2] = right[2][2];
		top[0][1] = right[1][2];
		top[0][0] = right[0][2];
		right[2][2] = bottom[2][0];
		right[1][2] = bottom[2][1];
		right[0][2] = bottom[2][2];
		bottom[2][0] = left[0][0];
		bottom[2][1] = left[1][0];
		bottom[2][2] = left[2][0];
		left[0][0] = temp_first;
		left[1][0] = temp_second;
		left[2][0] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Back_CounterClockwise() {
		back.Rotate_CounterClockwise();
		Cell temp_first(top[0][2]), temp_second(top[0][1]), temp_third(top[0][0]);
		top[0][2] = left[0][0];
		top[0][1] = left[1][0];
		top[0][0] = left[2][0];
		left[0][0] = bottom[2][0];
		left[1][0] = bottom[2][1];
		left[2][0] = bottom[2][2];
		bottom[2][0] = right[2][2];
		bottom[2][1] = right[1][2];
		bottom[2][2] = right[0][2];
		right[2][2] = temp_first;
		right[1][2] = temp_second;
		right[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Top_Clockwise() {
		top.Rotate_Clockwise();
		Cell temp_first(back[0][0]), temp_second(back[0][1]), temp_third(back[0][2]);
		back[0][0] = left[0][0];
		back[0][1] = left[0][1];
		back[0][2] = left[0][2];
		left[0][0] = front[0][0];
		left[0][1] = front[0][1];
		left[0][2] = front[0][2];
		front[0][0] = right[0][0];
		front[0][1] = right[0][1];
		front[0][2] = right[0][2];
		right[0][0] = temp_first;
		right[0][1] = temp_second;
		right[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Top_CounterClockwise() {
		top.Rotate_CounterClockwise();
		Cell temp_first(back[0][0]), temp_second(back[0][1]), temp_third(back[0][2]);
		back[0][0] = right[0][0];
		back[0][1] = right[0][1];
		back[0][2] = right[0][2];
		right[0][0] = front[0][0];
		right[0][1] = front[0][1];
		right[0][2] = front[0][2];
		front[0][0] = left[0][0];
		front[0][1] = left[0][1];
		front[0][2] = left[0][2];
		left[0][0] = temp_first;
		left[0][1] = temp_second;
		left[0][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Bottom_Clockwise() {
		bottom.Rotate_Clockwise();
		Cell temp_first(back[2][0]), temp_second(back[2][1]), temp_third(back[2][2]);
		back[2][0] = right[2][0];
		back[2][1] = right[2][1];
		back[2][2] = right[2][2];
		right[2][0] = front[2][0];
		right[2][1] = front[2][1];
		right[2][2] = front[2][2];
		front[2][0] = left[2][0];
		front[2][1] = left[2][1];
		front[2][2] = left[2][2];
		left[2][0] = temp_first;
		left[2][1] = temp_second;
		left[2][2] = temp_third;
		++Sides_Rotations;
	}

	void Rotate_Bottom_CounterClockwise() {
		bottom.Rotate_CounterClockwise();
		Cell temp_first(back[2][0]), temp_second(back[2][1]), temp_third(back[2][2]);
		back[2][0] = left[2][0];
		back[2][1] = left[2][1];
		back[2][2] = left[2][2];
		left[2][0] = front[2][0];
		left[2][1] = front[2][1];
		left[2][2] = front[2][2];
		front[2][0] = right[2][0];
		front[2][1] = right[2][1];
		front[2][2] = right[2][2];
		right[2][0] = temp_first;
		right[2][1] = temp_second;
		right[2][2] = temp_third;
		++Sides_Rotations;
	}

	void Default_Positioning() {																						// top side center is white (0)
		if (right[1][1] == 0) {																							// front side center is green (4)
			Y_Rotate_CounterClockwise();
		}
		else if (left[1][1] == 0) {
			Y_Rotate_Clockwise();
		}
		else if (back[1][1] == 0) {
			X_Rotate_Clockwise();
		}
		else if (front[1][1] == 0) {
			X_Rotate_CounterClockwise();
		}
		else if (bottom[1][1] == 0) {
			Y_Rotate_Clockwise();
			Y_Rotate_Clockwise();
		}
		if (right[1][1] == 4) {
			Z_Rotate_CounterClockwise();
		}
		else if (left[1][1] == 4) {
			Z_Rotate_Clockwise();
		}
		else if (back[1][1] == 4) {
			Z_Rotate_Clockwise();
			Z_Rotate_Clockwise();
		}
	}

	void Pif_Puff_Positioning() {																							// top center is yellow (5)
		Default_Positioning();																								// front center is green (4)
		Y_Rotate_Clockwise();
		Y_Rotate_Clockwise();
	}

	void Boomerang_Positioning() {
		Pif_Puff_Positioning();
		if (top[0][1] == 5) {
			if (top[1][0] == 5) {
				Z_Rotate_CounterClockwise();
			}
		}
		else if (top[1][2] == 5) {
			Z_Rotate_Clockwise();
		}
		else if (top[2][1] == 5) {
			Z_Rotate_Clockwise();
			Z_Rotate_Clockwise();
		}
	}



	void Shake() {
		Left_Final_Step_Combo();
		Regular_Opposite_Side_Cross_Combo();
		Rotate_Corners_Clockwise_Combo();
		Two_Layers_Combo();
		Opposite_Side_Cross_Combo();
		error_rotations = 0;
	}

	void Regular_Cross() {
		Bottom_White_Edges();
		FLBR_White_Edges();
		Top_Incorrect_White_Edges();
	}

	void Full_Face() {
		Pif_Puff_Positioning();
		Prepare_Corner();
		if ((top[2][2] == 0 || front[0][2] == 0 || right[0][0] == 0) && (top[2][2] == 3 || front[0][2] == 3 || right[0][0] == 3) && (top[2][2] == 4 || front[0][2] == 4 || right[0][0] == 4)) {
			while (bottom[0][2] != 0 || front[2][2] != 4 || right[2][0] != 3) {
				Pif_Puff();
			}
			Full_Face();
		}
		else {
			Rotate_Top_Clockwise();
			if ((top[2][0] == 0 || front[0][0] == 0 || left[0][2] == 0) && (top[2][0] == 2 || front[0][0] == 2 || left[0][2] == 2) && (top[2][0] == 4 || front[0][0] == 4 || left[0][2] == 4)) {
				Z_Rotate_Clockwise();
				while (bottom[0][2] != 0 || front[2][2] != 2 || right[2][0] != 4) {
					Pif_Puff();
				}
				Z_Rotate_CounterClockwise();
				Full_Face();
			}
			else {
				Rotate_Top_Clockwise();
				if ((top[0][0] == 0 || back[0][2] == 0 || left[0][0] == 0) && (top[0][0] == 2 || back[0][2] == 2 || left[0][0] == 2) && (top[0][0] == 1 || back[0][2] == 1 || left[0][0] == 1)) {
					Z_Rotate_Clockwise();
					Z_Rotate_Clockwise();
					while (bottom[0][2] != 0 || front[2][2] != 1 || right[2][0] != 2) {
						Pif_Puff();
					}
					Z_Rotate_CounterClockwise();
					Z_Rotate_CounterClockwise();
					Full_Face();
				}
				else {
					Rotate_Top_Clockwise();
					if ((top[0][2] == 0 || back[0][0] == 0 || right[0][2] == 0) && (top[0][2] == 3 || back[0][0] == 3 || right[0][2] == 3) && (top[0][2] == 1 || back[0][0] == 1 || right[0][2] == 1)) {
						Z_Rotate_CounterClockwise();
						while (bottom[0][2] != 0 || front[2][2] != 3 || right[2][0] != 1) {
							Pif_Puff();
						}
						Z_Rotate_Clockwise();
						Full_Face();
					}
				}
			}
		}
	}

	void Pif_Puff() {
		Rotate_Right_Clockwise();
		Rotate_Top_Clockwise();
		Rotate_Right_CounterClockwise();
		Rotate_Top_CounterClockwise();
	}
	
	void First_Two_Layers() {
		Default_Positioning();
		for (int i = 0; i < 4; ++i) {
			TL_Insert_Cell();
			Z_Rotate_Clockwise();
		}
	}

	void Two_Layers_Combo() {
		Rotate_Left_Clockwise();
		Rotate_Bottom_CounterClockwise();
		Rotate_Left_CounterClockwise();
		Rotate_Bottom_CounterClockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Bottom_Clockwise();
		Rotate_Front_Clockwise();
	}

	void Opposite_Side_Cross() {
		Boomerang_Positioning();
		if (top[0][1] == 5) {
			if (top[1][2] == 5) {
				if (!(top[2][1] == 5 && top[1][0] == 5)) {
					Opposite_Side_Cross_Combo();
				}
			}
			else {
				Opposite_Side_Cross_Combo();
				Opposite_Side_Cross_Combo();
			}
		}
		else {
			Opposite_Side_Cross_Combo();
			if (top[0][1] != 5) {
				Rotate_Top_Clockwise();
			}
			Opposite_Side_Cross_Combo();
			Opposite_Side_Cross_Combo();
		}
	}

	void Opposite_Side_Cross_Combo() {
		Rotate_Left_Clockwise();
		Rotate_Top_Clockwise();
		Rotate_Front_Clockwise();
		Rotate_Top_CounterClockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_CounterClockwise();
	}

	void Regular_Opposite_Side_Cross() {
		Pif_Puff_Positioning();
		Top_Side_Positioning();
		while (Cube_OSC_Positioning()) {
			Regular_Opposite_Side_Cross_Combo();
			Top_Side_Positioning();
		}
	}

	void Regular_Opposite_Side_Cross_Combo() {
		Rotate_Right_Clockwise();
		Rotate_Top_Clockwise();
		Rotate_Right_CounterClockwise();
		Rotate_Top_Clockwise();
		Rotate_Right_Clockwise();
		Rotate_Top_Clockwise();
		Rotate_Top_Clockwise();
		Rotate_Right_CounterClockwise();
	}

	void Place_Corners() {
		Cube_Corners_Positioning();
		while (!((top[2][2] == 5 || front[0][2] == 5 || right[0][0] == 5) && (top[2][2] == right[1][1] || front[0][2] == right[1][1] || right[0][0] == right[1][1])
			&& (top[2][2] == front[1][1] || front[0][2] == front[1][1] || right[0][0] == front[1][1]))) {
			Rotate_Corners_Clockwise_Combo();
			Cube_Corners_Positioning();
		}
	}

	void Rotate_Corners_Clockwise_Combo() {
		Rotate_Right_CounterClockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_CounterClockwise();
		Rotate_Front_Clockwise();
		Rotate_Right_Clockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_Clockwise();
		Rotate_Front_Clockwise();
	}

	void Final_Step() {
		Pif_Puff_Positioning();
		for (int i = 0; i < 4; ++i) {
			if (front[0][0] == 5) {
				Front_Final_Step_Combo();
			}
			else if (left[0][2] == 5) {
				Left_Final_Step_Combo();
			}
			Rotate_Top_Clockwise();
		}
		while (!front.Compare_Cells()) {
			Rotate_Top_Clockwise();
		}
	}

	void Left_Final_Step_Combo() {
		Rotate_Front_Clockwise();
		Rotate_Left_CounterClockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_Clockwise();
		Rotate_Front_Clockwise();
		Rotate_Left_CounterClockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_Clockwise();
	}

	void Front_Final_Step_Combo() {
		Rotate_Left_CounterClockwise();
		Rotate_Front_Clockwise();
		Rotate_Left_Clockwise();
		Rotate_Front_CounterClockwise();
		Rotate_Left_CounterClockwise();
		Rotate_Front_Clockwise();
		Rotate_Left_Clockwise();
		Rotate_Front_CounterClockwise();
	}



	bool Is_Correct() {
		Default_Positioning();
		int good_edges = 0, rotations = 0;
		std::vector<One_Colored_Cell> OneCcells = { {0}, {1}, {2}, {3}, {4}, {5} };
		std::vector<Two_Colored_Cell> TwoCcells = { {{0, 1}}, {{0, 2}}, {{0, 3}}, {{0, 4}}, {{5, 1}}, {{5, 2}}, {{5, 3}}, {{5, 4}}, {{1, 2}}, {{1, 3}}, {{4, 2}}, {{4, 3}} };
		std::vector<Three_Colored_Cell> ThreeCcells = { {{0, 1, 2}}, {{0, 1, 3}}, {{0, 4, 2}}, {{0, 4, 3}}, {{5, 1, 2}}, {{5, 1, 3}}, {{5, 4, 2}}, {{5, 4, 3}} };
		std::set<int>::iterator end;

		if (!(top[1][1] == 0 && left[1][1] == 3 && back[1][1] == 1 && right[1][1] == 2 && front[1][1] == 4 && bottom[1][1] == 5)) {
			return false;
		}

		for (int c = 0; c < OneCcells.size(); ++c) {
			if (top[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else if (front[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else if (back[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else if (left[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else if (right[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else if (bottom[1][1] == OneCcells[c].color && OneCcells[c].exist == false) {
				OneCcells[c].exist = true;
			}
			else {
				return false;
			}
		}		

		for (int c = 0; c < TwoCcells.size(); ++c) {
			end = TwoCcells[c].colors.end();
			if (TwoCcells[c].colors.find(top[0][1].Get_Color()) != end && TwoCcells[c].colors.find(back[0][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(top[1][2].Get_Color()) != end && TwoCcells[c].colors.find(right[0][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(top[2][1].Get_Color()) != end && TwoCcells[c].colors.find(front[0][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(top[1][0].Get_Color()) != end && TwoCcells[c].colors.find(left[0][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(bottom[0][1].Get_Color()) != end && TwoCcells[c].colors.find(front[2][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(bottom[1][2].Get_Color()) != end && TwoCcells[c].colors.find(right[2][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(bottom[2][1].Get_Color()) != end && TwoCcells[c].colors.find(back[2][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(bottom[1][0].Get_Color()) != end && TwoCcells[c].colors.find(left[2][1].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(front[1][0].Get_Color()) != end && TwoCcells[c].colors.find(left[1][2].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(front[1][2].Get_Color()) != end && TwoCcells[c].colors.find(right[1][0].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(back[1][0].Get_Color()) != end && TwoCcells[c].colors.find(right[1][2].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else if (TwoCcells[c].colors.find(back[1][2].Get_Color()) != end && TwoCcells[c].colors.find(left[1][0].Get_Color()) != end && TwoCcells[c].exist == false) {
				TwoCcells[c].exist = true;
			}
			else {
				return false;
			}
		}

		for (int c = 0; c < ThreeCcells.size(); ++c) {
			end = ThreeCcells[c].colors.end();
			if (ThreeCcells[c].colors.find(top[0][0].Get_Color()) != end && ThreeCcells[c].colors.find(back[0][2].Get_Color()) != end &&
				ThreeCcells[c].colors.find(left[0][0].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(top[0][2].Get_Color()) != end && ThreeCcells[c].colors.find(back[0][0].Get_Color()) != end &&
				ThreeCcells[c].colors.find(right[0][2].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(top[2][0].Get_Color()) != end && ThreeCcells[c].colors.find(front[0][0].Get_Color()) != end &&
				ThreeCcells[c].colors.find(left[0][2].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(top[2][2].Get_Color()) != end && ThreeCcells[c].colors.find(front[0][2].Get_Color()) != end &&
				ThreeCcells[c].colors.find(right[0][0].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(bottom[0][0].Get_Color()) != end && ThreeCcells[c].colors.find(front[2][0].Get_Color()) != end &&
				ThreeCcells[c].colors.find(left[2][2].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(bottom[0][2].Get_Color()) != end && ThreeCcells[c].colors.find(front[2][2].Get_Color()) != end &&
				ThreeCcells[c].colors.find(right[2][0].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(bottom[2][0].Get_Color()) != end && ThreeCcells[c].colors.find(back[2][2].Get_Color()) != end &&
				ThreeCcells[c].colors.find(left[2][0].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else if (ThreeCcells[c].colors.find(bottom[2][2].Get_Color()) != end && ThreeCcells[c].colors.find(back[2][0].Get_Color()) != end &&
				ThreeCcells[c].colors.find(right[2][2].Get_Color()) != end && ThreeCcells[c].exist == false) {
				ThreeCcells[c].exist = true;
			}
			else {
				return false;
			}
		}

		good_edges += Check_Edge(top[0][1], back[0][1]) + Check_Edge(top[1][2], right[0][1]) + Check_Edge(top[2][1], front[0][1]) + Check_Edge(top[1][0], left[0][1]);
		good_edges += Check_Edge(bottom[0][1], front[2][1]) + Check_Edge(bottom[1][2], right[2][1]) + Check_Edge(bottom[2][1], back[2][1]) + Check_Edge(bottom[1][0], left[2][1]);
		good_edges += Check_Edge(front[1][0], left[1][2]) + Check_Edge(front[1][2], right[1][0]) + Check_Edge(back[1][0], right[1][2]) + Check_Edge(back[1][2], left[1][0]);

		rotations += Check_Corner(top[0][0], left[0][0], back[0][2]) + Check_Corner(top[0][2], back[0][0], right[0][2]);
		rotations += Check_Corner(top[2][0], front[0][0], left[0][2]) + Check_Corner(top[2][2], right[0][0], front[0][2]);
		rotations += Check_Corner(bottom[0][0], left[2][2], front[2][0]) + Check_Corner(bottom[0][2], front[2][2], right[2][0]);
		rotations += Check_Corner(bottom[2][0], back[2][2], left[2][0]) + Check_Corner(bottom[2][2], right[2][2], back[2][0]);

		return !((rotations % 3) + (good_edges % 2));
	}

	bool Is_Ready() {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (top[i][j] != 0) {
					return false;
				}
			}
		}
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (front[i][j] != 4) {
					return false;
				}
			}
		}
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (left[i][j] != 3) {
					return false;
				}
			}
		}
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (back[i][j] != 1) {
					return false;
				}
			}
		}
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (right[i][j] != 2) {
					return false;
				}
			}
		}
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (bottom[i][j] != 5) {
					return false;
				}
			}
		}
		return true;
	}

	int Solve() {
		Cube_Rotations = 0;
		Sides_Rotations = 0;
		unsigned int start_time = 1, end_time = 0;
		if (Is_Correct()) {
			start_time = clock();
			Regular_Cross();
			Full_Face();
			First_Two_Layers();
			Opposite_Side_Cross();
			Regular_Opposite_Side_Cross();
			Place_Corners();
			Final_Step();
			end_time = clock();
		}
		return end_time - start_time;
	}


	bool One_Test(const char* filename, std::ostream& output) {
		Read_From_File(filename);
		unsigned int processing_time;
		processing_time = Solve();
		if (!Is_Correct()) {
			output << "Passed, Incorrect.\n";
			return true;
		}
		if (!Is_Ready()) {
			output << "Failed.\n";
			return false;
		}
		output << "Passed. Processing time: " << processing_time << " ms. Total amount of side rotations: " << Sides_Rotations << ". Total amount of cube rotations: " << Cube_Rotations << ".\n";
		//Cout();
		return true;
	}

	bool Test(std::ostream& output) {
		int passed = 0, failed = 0;
		if (!One_Test("a.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("b.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("c.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("d.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("e.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("f.txt", output)) {	//incorrect
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("g.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("h.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("i.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("j.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("k.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("l.txt", output)) {	//incorrect
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("m.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("n.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("o.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("p.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("q.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("r.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("s.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("t.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("u.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("v.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("w.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("x.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("y.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}
		if (!One_Test("z.txt", output)) {
			++failed;
		}
		else {
			++passed;
		}

		output << "\nPassed: " << passed << ". Failed: " << failed << ".\n";
		if (failed) {
			output << "Some tests failed.";
			return false;
		}
		output << "Everything works.";
		return true;
	}


	~Rubik_Cube() {}
};