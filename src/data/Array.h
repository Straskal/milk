#ifndef MILK_ARRAY_H
#define MILK_ARRAY_H

#include <algorithm> 

namespace milk {
	template<class T>
	class Array {
	public:
		explicit Array(int size) {
			array_ = new T[size];
			size_ = size;
		}

		~Array() {
			if (array_ != nullptr) {
				delete[] array_;
			}
			array_ = nullptr;
		}

		int size() const {
			return size_;
		}

		T& operator[](int index) {
			return array_[index];
		}

		operator T* () {
			return array_;
		}

	private:
		T* array_;
		int size_;
	};
}

#endif