#pragma once

template <typename Type>
class Circular_Buffer {
private:
	Type* memory;
	size_t size;
	size_t capacity;
	size_t front_shift;
	size_t back_shift;

	int Shift_Limitation(int current_shift) {
		if (current_shift < 0) { current_shift += capacity; }
		if (capacity) { return current_shift % capacity; }
		return current_shift;
	}

public:
	Circular_Buffer(size_t size = 0) : memory(new Type[size]), capacity(size) {
		size = 0;
		front_shift = 0;
		back_shift = 1;
	}

	int Get_Size() { return size; }

	size_t Get_Capacity() { return capacity; }

	Type* Get_Memory() { return memory; }

	class Iterator {
	private:
		Type* pointer;
		Circular_Buffer<Type>* owner;

	public:
		Iterator(Circular_Buffer<Type>* own = nullptr, Type* ptr = nullptr) : owner(own), pointer(ptr) {}

		Iterator(const Iterator& other) : pointer(other.pointer), owner(other.owner) {}

		Iterator& operator=(const Iterator& other) {
			if (other == *this) { return *this; }
			this->pointer = other.pointer;
			this->owner = other.owner;
			return *this;
		}

		Type& operator*() { return *this->pointer; }

		Type& operator[](int index) { return *(this->operator+(index)); }

		int operator- (const Iterator& other) const {
			if (*this >= other) {
				if (this->pointer - other.pointer >= 0) { return this->pointer - other.pointer; }
				return this->pointer - other.pointer + this->owner->Get_Capacity();
			}
			else {
				if (this->pointer - other.pointer >= 0) { return this->pointer - other.pointer - this->owner->Get_Capacity(); }
				return this->pointer - other.pointer;
			}
		}

		bool operator != (const Iterator& other) const { return this->pointer != other.pointer; }

		bool operator == (const Iterator& other) const { return !(this->operator!=(other)); }

		bool operator > (const Iterator& other) const {
			Iterator begin = this->owner->Begin();
			Iterator end = this->owner->End();
			if (begin.pointer <= end.pointer) { return this->pointer > other.pointer; }
			if ((this->pointer >= begin.pointer && other.pointer >= begin.pointer) || (this->pointer <= end.pointer && other.pointer <= end.pointer)) { return this->pointer > other.pointer; }
			if (this->pointer >= begin.pointer) { return false; }
			return true;
		}

		bool operator < (const Iterator& other) const { return !(this->operator>(other) || this->operator==(other)); }

		bool operator >= (const Iterator& other) const { return !(this->operator<(other)); }

		bool operator <= (const Iterator& other) const { return !(this->operator>(other)); }

		Iterator operator+ (int add) {
			Iterator result(*this);
			return result.operator+=(add);
		}

		Iterator operator- (int sub) {
			Iterator result(*this);
			return result.operator-=(sub);
		}

		Iterator& operator+= (int add) {
			Type* mem_begin = this->owner->Get_Memory();
			size_t buf_cap = this->owner->Get_Capacity();
			if (add >= buf_cap) {
				this->pointer = nullptr;
				return *this;
			}
			if (this->pointer + add > mem_begin + buf_cap - 1) { return this->operator-=(buf_cap - add); }
			Iterator begin = this->owner->Begin();
			Iterator end = this->owner->End();
			Iterator result(this->owner, this->pointer + add);
			if (!(begin <= result && result <= end)) { this->pointer = nullptr; }
			else { this->pointer += add; }
			return *this;
		}

		Iterator& operator-= (int sub) {
			Type* mem_begin = this->owner->Get_Memory();
			size_t buf_cap = this->owner->Get_Capacity();
			if (sub >= buf_cap) {
				this->pointer = nullptr;
				return *this;
			}

			if (this->pointer - sub < mem_begin) { return this->operator+=(buf_cap - sub); }
			Iterator begin = this->owner->Begin();
			Iterator end = this->owner->End();
			Iterator result(this->owner, this->pointer - sub);
			if (!(begin <= result && result <= end)) { this->pointer = nullptr; }
			else { this->pointer -= sub; }
			return *this;
		}

		friend Iterator operator+ (int add, Iterator& object) {
			return object.operator+(add);
		}

		Iterator& operator++() {
			Iterator end = this->owner->End();
			if (*this == end) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				size_t buf_cap = this->owner->Get_Capacity();
				Type* mem_end = mem_begin + buf_cap - 1;
				if (this->pointer != mem_end) { ++this->pointer; }
				else { this->pointer = mem_begin; }
			}
			return *this;
		}

		Iterator operator++(int) {
			Iterator old(*this);
			Iterator end = this->owner->End();
			if (*this == end) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				size_t buf_cap = this->owner->Get_Capacity();
				Type* mem_end = mem_begin + buf_cap - 1;
				if (this->pointer != mem_end) { ++this->pointer; }
				else { this->pointer = mem_begin; }
			}
			return old;
		}

		Iterator& operator--() {
			Iterator begin = this->owner->Begin();
			if (*this == begin) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				if (this->pointer != mem_begin) { --this->pointer; }
				else { this->pointer = mem_begin + this->owner->Get_Capacity() - 1; }
			}
			return *this;
		}

		Iterator operator--(int) {
			Iterator old(*this);
			Iterator begin = this->owner->Begin();
			if (*this == begin) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				if (this->pointer != mem_begin) { --this->pointer; }
				else { this->pointer = mem_begin + this->owner->Get_Capacity() - 1; }
			}
			return old;
		}
	};

	class Const_Iterator {
	private:
		Type* pointer;
		Circular_Buffer<Type>* owner;

	public:
		Const_Iterator(Circular_Buffer<Type>* own = nullptr, Type* ptr = nullptr) : owner(own), pointer(ptr) {}

		Const_Iterator(const Const_Iterator& other) : pointer(other.pointer), owner(other.owner) {}

		Const_Iterator& operator=(const Const_Iterator& other) {
			if (other == *this) { return *this; }
			this->pointer = other.pointer;
			this->owner = other.owner;
			return *this;
		}

		const Type& operator*() { return *this->pointer; }

		const Type& operator[](int index) { return *(this->operator+(index)); }

		int operator- (const Iterator& other) const {
			if (*this >= other) {
				if (this->pointer - other.pointer >= 0) { return this->pointer - other.pointer; }
				return this->pointer - other.pointer + this->owner->Get_Capacity();
			}
			else {
				if (this->pointer - other.pointer >= 0) { return this->pointer - other.pointer - this->owner->Get_Capacity(); }
				return this->pointer - other.pointer;
			}
		}

		bool operator != (const Const_Iterator& other) const { return this->pointer != other.pointer; }

		bool operator == (const Const_Iterator& other) const { return !(this->operator!=(other)); }

		bool operator > (const Const_Iterator& other) const {
			Iterator begin = this->owner->Begin();
			Iterator end = this->owner->End();
			if (begin.pointer <= end.pointer) { return this->pointer > other.pointer; }
			if ((this->pointer >= begin.pointer && other.pointer >= begin.pointer) || (this->pointer <= end.pointer && other.pointer <= end.pointer)) { return this->pointer > other.pointer; }
			if (this->pointer >= begin.pointer) { return false; }
			return true;
		}

		bool operator < (const Const_Iterator& other) const { return !(this->operator>(other) || this->operator==(other)); }

		bool operator >= (const Const_Iterator& other) const { return !(this->operator<(other)); }

		bool operator <= (const Const_Iterator& other) const { return !(this->operator>(other)); }

		Const_Iterator operator+ (int add) {
			Const_Iterator result(*this);
			return result.operator+=(add);
		}

		Const_Iterator operator- (int sub) {
			Const_Iterator result(*this);
			return result.operator-=(sub);
		}

		Const_Iterator& operator+= (int add) {
			Type* mem_begin = this->owner->Get_Memory();
			size_t buf_cap = this->owner->Get_Capacity();
			if (add >= buf_cap) {
				this->pointer = nullptr;
				return *this;
			}
			if (this->pointer + add > mem_begin + buf_cap - 1) { return this->operator-=(buf_cap - add); }
			Const_Iterator begin = this->owner->Begin();
			Const_Iterator end = this->owner->End();
			Const_Iterator result(this->owner, this->pointer + add);
			if (!(begin <= result && result <= end)) { this->pointer = nullptr; }
			else { this->pointer += add; }
			return *this;
		}

		Const_Iterator& operator-= (int sub) {
			Type* mem_begin = this->owner->Get_Memory();
			size_t buf_cap = this->owner->Get_Capacity();
			if (sub >= buf_cap) {
				this->pointer = nullptr;
				return *this;
			}

			if (this->pointer - sub < mem_begin) { return this->operator+=(buf_cap - sub); }
			Const_Iterator begin = this->owner->Begin();
			Const_Iterator end = this->owner->End();
			Const_Iterator result(this->owner, this->pointer - sub);
			if (!(begin <= result && result <= end)) { this->pointer = nullptr; }
			else { this->pointer -= sub; }
			return *this;
		}

		friend Const_Iterator operator+ (int add, Const_Iterator& object) {
			return object.operator+(add);
		}

		Const_Iterator& operator++() {
			Const_Iterator end = this->owner->End();
			if (*this == end) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				size_t buf_cap = this->owner->Get_Capacity();
				Type* mem_end = mem_begin + buf_cap - 1;
				if (this->pointer != mem_end) { ++this->pointer; }
				else { this->pointer = mem_begin; }
			}
			return *this;
		}

		Const_Iterator operator++(int) {
			Const_Iterator old(*this);
			Const_Iterator end = this->owner->End();
			if (*this == end) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				size_t buf_cap = this->owner->Get_Capacity();
				Type* mem_end = mem_begin + buf_cap - 1;
				if (this->pointer != mem_end) { ++this->pointer; }
				else { this->pointer = mem_begin; }
			}
			return old;
		}

		Const_Iterator& operator--() {
			Const_Iterator begin = this->owner->Begin();
			if (*this == begin) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				if (this->pointer != mem_begin) { --this->pointer; }
				else { this->pointer = mem_begin + this->owner->Get_Capacity() - 1; }
			}
			return *this;
		}

		Const_Iterator operator--(int) {
			Const_Iterator old(*this);
			Const_Iterator begin = this->owner->Begin();
			if (*this == begin) { this->pointer = nullptr; }
			else {
				Type* mem_begin = this->owner->Get_Memory();
				if (this->pointer != mem_begin) { --this->pointer; }
				else { this->pointer = mem_begin + this->owner->Get_Capacity() - 1; }
			}
			return old;
		}
	};

	Type& operator[](int index) {
		Iterator begin = Begin();
		return begin[index];
	}

	Iterator Begin(bool for_insert = false) {
		if (!for_insert) { return Iterator(this, memory + front_shift); }
		else {
			if (front_shift) { return Iterator(this, memory + front_shift - 1); }
			else { return Iterator(this, memory + capacity - 1); }
		}
	}

	Iterator End() {
		Iterator end(this, memory + back_shift);
		if (!size) { --end; }
		return end;
	}

	Const_Iterator Begin(bool for_insert = false) const {
		if (!for_insert) { return Const_Iterator(this, memory + front_shift); }
		else {
			if (front_shift) { return Const_Iterator(this, memory + front_shift - 1); }
			else { return Const_Iterator(this, memory + capacity - 1); }
		}
	}

	Const_Iterator End() const {
		Const_Iterator end(this, memory + back_shift);
		if (!size) { --end; }
		return end;
	}

	Const_Iterator cBegin(bool for_insert = false) const {
		if (!for_insert) { return Const_Iterator(this, memory + front_shift); }
		else {
			if (front_shift) { return Const_Iterator(this, memory + front_shift - 1); }
			else { return Const_Iterator(this, memory + capacity - 1); }
		}
	}

	Const_Iterator cEnd() const {
		Const_Iterator end(this, memory + back_shift);
		if (!size) { --end; }
		return end;
	}

	void Insert(const Type& element, Iterator place) {
		Iterator default_it;
		if (place == default_it) { *memory = element; }
		else { *place = element; }
		++size;
	}

	void Push_Front(const Type& element) {
		if (!capacity) { Resize(2); }
		Iterator place;
		if (size) { place = Begin(true); }
		Insert(element, place);
		if (size > 1) { front_shift = Shift_Limitation(front_shift - 1); }
		if (size == capacity) { Resize(capacity * 2); }
	}

	void Push_Back(const Type& element) {
		if (!capacity) { Resize(2); }
		Iterator place(this);
		if (size) { place = End(); }
		Insert(element, place);
		if (size > 1) { back_shift = Shift_Limitation(back_shift + 1); }
		if (size == capacity) { Resize(capacity * 2); }
	}

	const Type& Pop_Front() {
		Iterator begin = Begin();
		size--;
		if (size) { front_shift = Shift_Limitation(front_shift + 1); }
		else {
			front_shift = 0;
			back_shift = 1;
		}
		return *begin;
	}

	const Type& Pop_Back() {
		Iterator end = End();
		size--;
		if (size) { back_shift = Shift_Limitation(back_shift - 1); }
		else {
			front_shift = 0;
			back_shift = 1;
		}
		return *(--end);
	}

	void Resize(size_t new_size) {
		Type* new_memory = new Type[new_size];
		Iterator begin = Begin();
		back_shift = Shift_Limitation(back_shift - 1);
		for (int i = 0; i < size; ++begin, ++i) { new_memory[i] = *begin; }

		delete[] memory;
		memory = new_memory;
		front_shift = 0;
		back_shift = std::max(size, (size_t)1);
		capacity = new_size;
	}

	void Print() {
		Iterator begin = Begin();
		Iterator end = End();
		for (; begin < end; ++begin) { std::cout << *begin << std::endl; }
		std::cout << std::endl;
	}


	~Circular_Buffer() {
		delete[] memory;
	}
};
