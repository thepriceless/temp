#pragma once
#include "Cell.h"

class Side {
private:
	std::vector<std::vector<Cell>> face;
	int size;
	std::string name;

	void Resize(int side_size) {
		face.resize(side_size);
		for (int i = 0; i < side_size; ++i) {
			face[i].resize(side_size);
		}
	}

public:
	Side(std::string side_name, int side_size = 3) : name(side_name), size(side_size) {
		Resize(side_size);
	}

	Side(const Side& other) : size(other.size), face(other.face), name(other.name) {}

	std::vector<Cell>& operator [] (int index) {
		return face[index];
	}

	const std::vector<Cell>& operator [] (int index) const {
		return face[index];
	}

	Side& operator = (const Side& object) {
		this->face = object.face;
		return *this;
	}

	void Rotate_Clockwise() {
		Cell temp(face[0][0]);
		face[0][0] = face[2][0];
		face[2][0] = face[2][2];
		face[2][2] = face[0][2];
		face[0][2] = temp;
		temp = face[0][1];
		face[0][1] = face[1][0];
		face[1][0] = face[2][1];
		face[2][1] = face[1][2];
		face[1][2] = temp;
	}

	void Rotate_CounterClockwise() {
		Cell temp(face[0][0]);
		face[0][0] = face[0][2];
		face[0][2] = face[2][2];
		face[2][2] = face[2][0];
		face[2][0] = temp;
		temp = face[0][1];
		face[0][1] = face[1][2];
		face[1][2] = face[2][1];
		face[2][1] = face[1][0];
		face[1][0] = temp;
	}

	bool Compare_Cells() {
		return (face[0][1] == face[1][1]);
	}

	void Print_Side(std::ostream& output) const {
		output << name << '\n';
		for (int row = 0; row < size; ++row) {
			for (int col = 0; col < size - 1; ++col) {
				output << face[row][col] << ' ';
			}
			if (row == size - 1 && name == "top") {
				output << face[row][size - 1];
			}
			else {
				output << face[row][size - 1] << '\n';
			}
		}
	}

	~Side() {
		face.clear();
	}
};