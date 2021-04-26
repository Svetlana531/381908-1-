#pragma once

#include "Stack.h"
#include <fstream>
#include <string>

#define MemLim 100   // Ðàçìåð ïàìÿòè
#define StackNum 15  // Ñòåêè

class MultiRoot
{
protected:
	int* Mem;
	int DefaultStack;
public:
	MultiRoot() { Mem = new int[MemLim]; DefaultStack = 0; }

	virtual bool IsEmpty(int ns) const = 0;
	virtual bool IsFull(int ns) const = 0;
	virtual void Put(int ns, const int& Val) = 0;
	virtual int Get(int ns) = 0;


	void SetDefaultStack(int ns) { DefaultStack = ns; }
	int  IsEmpty(void) const { return IsEmpty(DefaultStack); }
	int  IsFull(void) const { return IsFull(DefaultStack); }
	void Put(const int& Val) { Put(DefaultStack, Val); }
	int Get(void) { return Get(DefaultStack); }
};

class Multistack : public MultiRoot
{
public:
	Stack* pStack[StackNum + 1];
	int FreeMemSize; 
protected:
	int* pStackMem[StackNum + 1];
	int RelocationCount;
	int StackRelocation(int nst);
	int GetFreeMemSize();
	virtual void SetStackRelocation(int* pStackMem[]);
public:
	Multistack();
	~Multistack();
	bool IsEmpty(int ns)const;
	bool IsFull(int ns)const;
	virtual void Put(int ns, const int& Val);
	virtual int Get(int ns);
	int GetRelocationCount() { return RelocationCount; }
	int GetMinCountStack();

	void PrintMulti() const;

	void WriteForFile(std::string path) const;
	void ReadInFile	 (std::string path);
};

int Multistack::StackRelocation(int nst)
{
	int is, ns, ks, k, res = 0;
	pStack[nst]->DataCount++;
	int tmp = FreeMemSize;
	FreeMemSize = GetFreeMemSize();
	if (FreeMemSize > -1)
	{
		res = 1;
		RelocationCount++;
		SetStackRelocation(pStackMem);
		for (ns = 0; ns < StackNum; ns++)
		{
			if (pStackMem[ns] < pStack[ns]->pMem)
			{
				for (k = 0; k < pStack[ns]->DataCount; k++)
				{
					pStackMem[ns][k] = pStack[ns]->pMem[k];
				}
				pStack[ns]->SetMem(pStackMem[ns], pStackMem[ns + 1] - pStackMem[ns]);
			}
			else if (pStackMem[ns] > pStack[ns]->pMem)
			{
				for (ks = ns; pStackMem[ks + 1] > pStack[ks + 1]->pMem; ks++);
				for (is = ks; is >= ns; is--)
				{
					for (k = pStack[is]->DataCount - 1; k >= 0; k--)
					{
						pStackMem[is][k] = pStack[is]->pMem[k];
					}
					pStack[is]->SetMem(pStackMem[is], pStackMem[is + 1] - pStackMem[is]);
				}
			}
			else
			{
				pStack[ns]->SetMem(pStackMem[ns], pStackMem[ns + 1] - pStackMem[ns]);
			}
		}
	}
	pStack[nst]->DataCount--;
	FreeMemSize++;
	return res;
}

int Multistack::GetFreeMemSize()
{
	FreeMemSize = 0;
	for (int i = 0; i < StackNum; i++)
	{
		FreeMemSize += pStack[i]->MemSize - pStack[i]->DataCount;
	}
	return FreeMemSize;
}

void Multistack::SetStackRelocation(int* pStackMem[])
{
	pStackMem[0] = &Mem[0];
	for (int i = 1; i <= StackNum; i++)
	{
		pStackMem[i] = pStackMem[i - 1] + pStack[i - 1]->DataCount + FreeMemSize / StackNum;
	}
	pStackMem[StackNum] = pStack[StackNum]->pMem;
}

Multistack::Multistack()
{
	int Size = MemLim / StackNum;
	for (int i = 0, pos = 0; i <= StackNum; i++, pos += Size)
	{
		pStack[i] = new Stack(0);
		if (i == StackNum - 1)
		{
			pStack[i]->SetMem(&Mem[pos], MemLim - pos);
		}
		if (i == StackNum)
		{
			pStack[i]->SetMem(&Mem[MemLim], 0);
		}
		else
		{
		pStack[i]->SetMem(&Mem[pos], Size);
		}
	}
	RelocationCount = 0;
	FreeMemSize = MemLim;
}

Multistack::~Multistack()
{
	for (int i = 0; i < StackNum; i++)
	{
		delete pStack[i];
	}
}

bool Multistack::IsEmpty(int ns) const { return pStack[ns]->IsEmpty(); }

bool Multistack::IsFull(int ns) const { return pStack[ns]->IsFull(); }

void Multistack::Put(int ns, const int& Val)
{
	if (pStack[ns]->IsFull())
	{
		if (!StackRelocation(ns))
		{
			throw std::out_of_range("Input error: No free memory");
		}
	}
	pStack[ns]->Put(Val);
	FreeMemSize--;
}

int Multistack::Get(int ns)
{
	int tmp = pStack[ns]->Get();
	FreeMemSize++;
	return tmp;
}

inline int Multistack::GetMinCountStack()
{
	int min = 100;
	int num = 0;
	for (int i = 0; i < StackNum; i++)
	{
		int count = pStack[i]->DataCount;
		if (min > count && count != 0)
		{
			min = pStack[i]->DataCount;
			num = i;
		}
	}
	return num;
}

inline void Multistack::PrintMulti() const
{
	for (int i = 0; i < StackNum; i++)
	{
		if (pStack[i]->DataCount == 0) { continue; }
		std::cout << i << ") ";
		pStack[i]->print();
		std::cout << "\n";
	}
}

inline void Multistack::WriteForFile(std::string path) const
{
	std::ofstream file(path, std::ios::trunc);
	if (file.is_open())
	{
		for (int i = 0; i < StackNum; i++)
		{
			if (pStack[i]->DataCount == 0) { continue; }
			file << i << ") ";
			file << pStack[i]->GetElements();
			file << "\n";
		}
	}
	else
	{
		std::cout << "Files is not open!\n";
	}
	file.close();
}

inline void Multistack::ReadInFile(std::string path)
{
	std::ifstream file(path, std::ios::in);
	if (file.is_open())
	{
		std::string buf;

		int val[250];
		int i_val = 0;
		int pos;
		std::string str;
		while (std::getline(file, buf))
		{
			for (int i = 0; i < buf.size(); i++)
			{
				if (buf[i] != ')')
				{
					str += buf[i];
				}
				else { break; }
			}
			pos = std::atoi(str.c_str());
			int i = str.size() + 2;
			str.clear();
			for (i; i < buf.size(); i++)
			{ 
				if(buf[i] == ' ') 
				{ 
					val[i_val] = std::atoi(str.c_str());
					Put(pos, val[i_val]);
					i_val++;
					str.clear();
					continue;
				}
				str += buf[i];
			}
		}
	}
	else
	{
		std::cout << "Files is not open!\n";
	}
	file.close();
}





