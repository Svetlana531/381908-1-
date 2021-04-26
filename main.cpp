#include "MultiStack.h"

#define PATH "C:\\out.txt"

int main() {

	Multistack a;

 	a.Put(1, 0);
	a.Put(1, 5);
	a.Put(1, 33);

	a.Put(2, 1);
	a.Put(2, 1);

	a.Put(3, 2);
	a.Put(3, 2);
	
	a.Put(4, 3);

	a.Put(10, 22);
	a.Put(10, 21);
	a.Put(10, 23);
	a.Put(10, 24);

	a.Put(14, 3);
	a.Put(14, 2);
	a.Put(14, 1);
	a.Put(14, 0);
	a.Put(14, -1);

	a.PrintMulti();
	std::cout << "Min count elements in stack: " << a.GetMinCountStack() << "\n";

	a.WriteForFile(PATH);

	std::cout << std::endl;
	Multistack b;
	b.ReadInFile(PATH);

	b.PrintMulti();

	std::cout << "Min count elements in stack: " << b.GetMinCountStack() << "\n";

	return 0;
}