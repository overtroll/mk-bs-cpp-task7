#include <iostream>

#include "intrusive_ptr.h"

class TDoc : public TRefCounter<TDoc> {
};

int main() {
	TIntrusivePtr<TDoc> ptr = nullptr;

	ptr = MakeIntrusive<TDoc>();
	std::cout << ptr->RefCount() << "\n";

	TIntrusivePtr<TDoc> ptr2 = ptr;
	std::cout << ptr->RefCount() << " " << ptr2->RefCount() << "\n";

	TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
	ptr3.Reset(ptr2);
	std::cout << ptr->RefCount() << " " << ptr3->RefCount() << "\n";

	ptr3.Reset();
	// It seems highly unlikely that ptr3->RefCount() is supposed to be 2 as specified
	// in the task and therefore I assume that ptr2 was intended.
	std::cout << ptr->RefCount() << " " << ptr2->RefCount() << "\n";

	ptr3.Reset(std::move(ptr2));
	std::cout << ptr->RefCount() << " " << ptr3->RefCount() << "\n";

	static_assert(sizeof(TDoc*) == sizeof(ptr), "wee");
}