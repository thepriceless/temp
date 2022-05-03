#pragma once

// white - 0
// blue - 1
// red - 2
// orange - 3
// green - 4
// yellow - 5

class Cell {
private:
	int color;
public:
	Cell(int cell_color = 0) : color(cell_color) {}

	Cell(const Cell& other) : color(other.color) {}
	
	Cell& operator = (const Cell &object) {
		if (&object == this) { return *this; }
		this->color = object.color;
		return *this;
	}

	Cell& operator = (int color) {
		this->color = color;
		return *this;
	}

	int Get_Color() const {
		return this->color;
	}

	bool operator == (const Cell& object) const {
		return this->color == object.color;
	}

	bool operator == (int color) const {
		return this->color == color;
	}

	bool operator != (int color) const {
		return this->color != color;
	}

	friend std::ostream& operator << (std::ostream& out, const Cell& object) {
		out << object.color;
		return out;
	}

	~Cell() {}
};