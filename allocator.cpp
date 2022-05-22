#include <iostream>
#include <vector>
#include <cstddef>

struct Block {
	int size;
	int amount;
	bool* used;

	Block() {
		this->size = 0;
		this->amount = 0;
		used = nullptr;
	}

	Block(const Block& object) : size(object.size), amount(object.amount), used(object.used) {};
};


template<typename T>
class Allocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = size_t;

	value_type* base; //hide into private
	Block* Chunk_info;

	template<class U>
	struct rebind {
		using other = Allocator<U>;
	};

	Allocator() {
		int chunk_sizes[] = { 4096, 1024, 256, 64, 16, 8, 4, 1 };
		int chunk_amounts[] = { 1, 2, 8, 16, 64, 256, 256, 1024 };

		Chunk_info = new Block[8];

		int total_capacity = 0;
		for (int i = 0; i < 8; ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = (value_type*)malloc(total_capacity);
	}

	Allocator(int* chunk_sizes, int* chunk_amounts, int blocks_amount) {
		Chunk_info = new Block[blocks_amount];

		int total_capacity = 0;
		for (int i = 0; i < blocks_amount; ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = (value_type*)malloc(total_capacity);
	}
	
	template<class U>
	Allocator(const Allocator<U>& object) {
		base = object.base;
		Chunk_info = object.Chunk_info;
	}

	pointer allocate(size_t n) {
		int i = 0;
		while (n * sizeof(value_type) <= Chunk_info[i].size) {
			++i;
		}
		--i;
		
		if (i == -1) { /*exception*/ };

		int shift = Shift_Counter(i);
		while (shift == -1 && i > 0) {
			--i;
			shift = Shift_Counter(i);
		}

		if (shift != -1) {
			Chunk_info[i].used[shift / Chunk_info[i].size] = true;
			for (int chunk = 0; chunk < i; ++chunk) {
				shift += Chunk_info[chunk].size * Chunk_info[chunk].amount;
			}
			return static_cast<pointer>(base + shift / sizeof(value_type));
		}

		/*exception*/
	}

	void deallocate(value_type* pointer, size_t n) {
		ptrdiff_t shift = (pointer - base) * sizeof(value_type);
		int chunk_group = 0;
		while (shift >= Chunk_info[chunk_group].size * Chunk_info[chunk_group].amount) {
			shift -= Chunk_info[chunk_group].size * Chunk_info[chunk_group].amount;
			++chunk_group;
		}

		int subblock_in_chunk = Shift_DeCounter(shift, chunk_group);
		Chunk_info[chunk_group].used[subblock_in_chunk] = false;
	}

	Allocator& operator = (const Allocator& object) {
		if (this == &object) { return *this; }
		this->base = object.base;
		this->Chunk_info = object.Chunk_info;
		return *this;
	}

	bool operator == (const Allocator& object) const {
		return (this->base == object.base);
	}

	bool operator != (const Allocator& object) const {
		return (!(this->base == object.base));
	}

	~Allocator() {
		//delete base;
	}

private:
	int Shift_Counter(int chunk_group) const {
		int shift = 0;
		for (int j = 0; j < Chunk_info[chunk_group].amount; ++j) {
			if (!Chunk_info[chunk_group].used[j]) {
				return shift;
			}
			shift += Chunk_info[chunk_group].size;
		}
		return -1;
	}

	int Shift_DeCounter(ptrdiff_t shift, int chunk_group) {
		int block_in_chunk = 0;
		while (shift > 0) {
			shift -= Chunk_info[chunk_group].size;
			++block_in_chunk;
		}

		return block_in_chunk;
	}

};


template<class T>
class TrackingAllocator
{
public:
	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;

	using size_type = size_t;

	value_type* base; //hide into private
	Block* Chunk_info;

	TrackingAllocator() {
		std::vector<int> chunk_sizes = { 4096, 1024, 256, 64, 16, 8, 4, 1 };
		std::vector<int> chunk_amounts = { 1, 2, 8, 16, 64, 256, 256, 1024 };

		Chunk_info = new Block[chunk_sizes.size()];

		int total_capacity = 0;
		for (int i = 0; i < chunk_sizes.size(); ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = (value_type*)malloc(total_capacity);
	}

	TrackingAllocator(int* chunk_sizes, int* chunk_amounts, int blocks_amount) {
		Chunk_info = new Block[blocks_amount];

		int total_capacity = 0;
		for (int i = 0; i < blocks_amount; ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = (value_type*)malloc(total_capacity);
	}

	TrackingAllocator(const TrackingAllocator& object) = default;

	~TrackingAllocator() = default;

	pointer allocate(size_type numObjects)
	{
		std::cout << "allocate";
		return static_cast<pointer>(operator new(sizeof(T) * numObjects));
	}

	void deallocate(pointer p, size_type numObjects)
	{
		operator delete(p);
	}


private:
	int Shift_Counter(int chunk_group) const {
		int shift = 0;
		for (int j = 0; j < Chunk_info[chunk_group].amount; ++j) {
			if (!Chunk_info[chunk_group].used[j]) {
				return shift;
			}
			shift += Chunk_info[chunk_group].size;
		}
		return -1;
	}

	int Shift_DeCounter(ptrdiff_t shift, int chunk_group) {
		int block_in_chunk = 0;
		while (shift > 0) {
			shift -= Chunk_info[chunk_group].size;
			++block_in_chunk;
		}

		return block_in_chunk;
	}
};




int main() {
	int* sizez = new int[3];
	sizez[0] = 2000;
	sizez[1] = 1000;
	sizez[2] = 10;
	int* amountz = new int[3];
	amountz[0] = 5;
	amountz[1] = 15;
	amountz[2] = 1000;
	Allocator<int> all(sizez, amountz, 3);
	
	std::vector<int, std::allocator<int>> v(5);
	std::vector<int, Allocator<int>> v1;
	
	/*all.allocate(10);
	all.allocate(11);
	all.allocate(16);
	int* last = all.allocate(251);
	int* p = all.allocate(250);
	int* pp = all.allocate(250);
	*pp = 239;
	*last = 41;
	std::cout << *pp << ' ' << *last;
	*last += 1;
	std::cout << *pp << ' ' << *last;
	Allocator<int> all2(all);
	all2.deallocate(last, 251);
	all2.deallocate(pp, 250);
	for (int i = 0; i < 1; i++) {
		std::cout << all2.Chunk_info[0].used[i];
	}
	
	Allocator<int> all3 = all;
	all3.deallocate(p, 250);
	for (int i = 0; i < 1; i++) {
		std::cout << all.Chunk_info[0].used[i];
	}
	for (int i = 0; i < 2; i++) {
		std::cout << all.Chunk_info[1].used[i];
	}*/
}
