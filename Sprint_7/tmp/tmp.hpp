#include "scoped_ptr.hpp"
#include <string>
#include <vector>
#include <cassert>

using namespace std;

int tmp() {
	{
		std::vector<int> a = {1, 2, 3, 4, 5};
		a.insert(a.begin() + 5, 6);
	}

	{
		std::vector<int> a = { 1, 2, 3, 4, 5 };
		int i = 4;
		if (i < a.size()) {
			a[i] = 0;
		}
	}

	{
		ScopedPtr<float> smart_ptr_1(4);
	}

	{
		string* tmp = nullptr;

		string* raw_ptr = new string("hello");
		ScopedPtr<string> smart_ptr;
		smart_ptr.SetRawPtr(raw_ptr);
		tmp = smart_ptr.GetRawPtr();

		assert(tmp == raw_ptr);
		assert(&*smart_ptr == raw_ptr);
		assert(*tmp == *raw_ptr);

		tmp = smart_ptr.Release();

		assert(tmp == raw_ptr);
		assert(smart_ptr.GetRawPtr() == nullptr);
		assert(*tmp == *raw_ptr);

		string* new_raw_ptr = new string("goodbay");
		smart_ptr.SetRawPtr(new_raw_ptr);

		assert(tmp == raw_ptr);
		assert(smart_ptr.GetRawPtr() == new_raw_ptr);
		assert(*tmp == *raw_ptr);
	}

	return 0;
}