#include <iostream>
#include <string>

#define DefMemSize 25

enum TMemType
{
	MEM_HOLDER, MEM_RENTER
};

class Stack
{
protected:
	int* pMem;
	int MemSize;
	int DataCount;
	TMemType MemType;
	int Hi;
protected:
	void SetMem(int* _pMem, int Size);
	int GetNextIndex(int index) { return ++index; };
public:
	~Stack();
	Stack(int Size = DefMemSize);
	int IsEmpty()const { return DataCount == 0; };
	int IsFull()const { return DataCount == MemSize; };
	void Put(const int& Val);
	int Get();
	void swap(Stack& first, Stack& second);
	void print();
	std::string GetElements();
	friend class Multistack;
	friend class MultiRoot;
};

void Stack::SetMem(int* _pMem, int Size)
{
	if (MemType == MEM_HOLDER) { delete[] pMem; }
	pMem = _pMem;
	MemType = MEM_RENTER;
	MemSize = Size;
}

Stack::~Stack()
{
	if (MemType == MEM_HOLDER)
	{
		delete[] pMem;
	}
	pMem = nullptr;
}

Stack::Stack(int Size)
{
	Hi = -1;
	DataCount = 0;
	MemSize = Size;
	if (Size == 0)
	{
		pMem = nullptr;
		MemType = MEM_RENTER;
	}
	else
	{
		pMem = new int[MemSize];
		MemType = MEM_HOLDER;
	}
}

void Stack::Put(const int& Val)
{
	if (pMem == nullptr)throw "Память не выделена!";
	else if (IsFull())  throw "Стек заполнен!";
	else
	{
		Hi = GetNextIndex(Hi);
		pMem[Hi] = Val;
		DataCount++;
	}
}

int Stack::Get()
{
	int temp = -1;
	if (pMem == nullptr)throw "Память не выделена!";
	else if (IsEmpty()) throw "Стек пуст!";
	else
	{
		temp = pMem[Hi--];
		DataCount--;
	}
	return temp;
}

void Stack::swap(Stack& first, Stack& second)
{
	if (first.MemSize != second.MemSize)
	{
		throw "Разное количество памяти у стеков";
	}
	Stack tmpF;
	Stack tmpS;
	for (int i = 0; i < first.MemSize; i++)		// Zapis first
	{
		if (first.IsEmpty()) { break; }
		tmpF.Put(first.Get());
	}
	for (int i = 0; i < second.MemSize; i++)	// Zapis second 
	{
		if (second.IsEmpty()) { break; }
		tmpS.Put(second.Get());
	}

	for (int i = 0; i < tmpS.MemSize; i++)		// Swap first <- second
	{
		if (tmpS.IsEmpty()) { break; }
		first.Put(tmpS.Get());
	}
	for (int i = 0; i < tmpF.MemSize; i++)      // Swap first -> second
	{
		if (tmpF.IsEmpty()) { break; }
		second.Put(tmpF.Get());
	}
}

void Stack::print()
{
	for (int i = 0; i < DataCount; i++)
	{
		std::cout << pMem[i] << " ";
	}
}

inline std::string Stack::GetElements()
{
	std::string res;
	for (int i = 0; i < DataCount; i++)
	{
		res += std::to_string(pMem[i]) + " ";
	}
	return res;
}
