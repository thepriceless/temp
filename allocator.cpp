#pragma once

template<typename T>
class Allocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = size_t;

	template <class U>
	friend class Allocator;

	template<class U>
	struct rebind {
		using other = Allocator<U>;
	};

	Allocator() {
		size_t chunk_sizes[] = { 8192, 4096, 1024, 256, 64, 16, 8, 1, 0 };
		size_t chunk_amounts[] = { 1, 2, 4, 8, 16, 64, 256, 1024, 1 };

		Chunk_info = new Block[9];

		size_t total_capacity = 0;
		for (int i = 0; i < 9; ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = static_cast<char*>(malloc(total_capacity));
	}

	Allocator(size_t* chunk_sizes, size_t* chunk_amounts, size_t blocks_amount) {
		chunk_sizes[blocks_amount] = 0;
		chunk_amounts[blocks_amount] = 1;

		Chunk_info = new Block[blocks_amount + 1];

		size_t total_capacity = 0;
		for (int i = 0; i < blocks_amount; ++i) {
			Chunk_info[i].amount = chunk_amounts[i];
			Chunk_info[i].size = chunk_sizes[i];
			Chunk_info[i].used = new bool[chunk_amounts[i]];
			for (int j = 0; j < chunk_amounts[i]; ++j) {
				Chunk_info[i].used[j] = false;
			}

			total_capacity += chunk_sizes[i] * chunk_amounts[i];
		}
		base = static_cast<char*>(malloc(total_capacity));
	}

	template<class U>
	Allocator(const Allocator<U>& object) : base(object.base), Chunk_info(object.Chunk_info) {}

	pointer allocate(size_t n) {
		int i = 0;
		while (n * sizeof(value_type) <= Chunk_info[i].size) {
			++i;
		}
		--i;

		if (i == -1) {
			throw std::bad_alloc();
		};

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
			return (T*)(base.ptr + shift);
		}

		throw std::bad_alloc();
	}

	void deallocate(value_type* pointer, size_t n) {
		ptrdiff_t shift = ((char*)pointer - base.ptr);
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
		if (*(base.usage_cnt) == 1) {
			std::free(base.ptr);
		}
		if (*(Chunk_info.usage_cnt) == 1) {
			delete[] Chunk_info.ptr;
		}
	}

private:
	My_sharedptr<char> base;
	My_sharedptr<Block> Chunk_info;

	int Shift_Counter(size_t chunk_group) const {
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
