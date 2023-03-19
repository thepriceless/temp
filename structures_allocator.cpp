#pragma once

struct Block {
	size_t size;
	size_t amount;
	bool* used;
	size_t easy_shift;

	Block() {
		this->size = 0;
		this->amount = 0;
		easy_shift = 0;
		used = nullptr;
	}

	Block(const Block& object) : size(object.size), amount(object.amount), used(object.used), easy_shift(object.easy_shift) {};

	~Block() {
		delete[] used;
	}
};


template<class T>
struct My_sharedptr {
	T* ptr;
	int* usage_cnt;

	My_sharedptr() {
		usage_cnt = new int(1);
	}

	My_sharedptr(const My_sharedptr& object) {
		++(*object.usage_cnt);
		ptr = object.ptr;
		usage_cnt = object.usage_cnt;
	}

	T& operator[] (size_t ind) const {
		return ((T*)ptr)[ind];
	}

	template <class U>
	My_sharedptr& operator = (const My_sharedptr<U>& object) {
		ptr = object.ptr;
		usage_cnt = object.usage_cnt;
		return *this;
	}

	My_sharedptr& operator= (T* pointer) {
		ptr = pointer;
		return *this;
	}

	bool operator== (const My_sharedptr& object) const {
		return ptr == object.ptr;
	}

	~My_sharedptr() {
		if (*usage_cnt == 1) {
			delete usage_cnt;
		}
		else {
			--(*usage_cnt);
		}
	}
};
