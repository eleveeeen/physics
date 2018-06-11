#ifndef _HeapManager_h_
#define _HeapManager_h_

#include"Common.h"

//プールされたメモリを管理するスタックサイズ
#define HEAP_STACK_SIZE 64

#define MIN_ALLOC_SIZE 16


#define ALLOC_BYTES_ALIGN16(bytes) max(16,BYTES_ALIGN16(bytes))
#define ALLOC_BYTES_ALIGN128(bytes) max(128,BYTES_ALIGN128(bytes))


class HeapManager
{
private:
	unsigned char* heap;
	unsigned char* poolStack[HEAP_STACK_SIZE];
	int heapBytes;
	int curStack;
	int rest;

public:
	enum
	{
		ALIGN16=16,
		ALIGN128=128
	};

	HeapManager()
	{
		heap = nullptr;
		heapBytes = 0;
	}
	HeapManager(unsigned char* buf, int bytes)
	{
		heap = buf;
		heapBytes = bytes;
		Clear();
	}

	~HeapManager()
	{

	}
	void Init(unsigned char* buf,int bytes)
	{
		heap = buf;
		heapBytes = bytes;
		Clear();
	}


	int GetAllocated()
	{
		return (int)(poolStack[curStack] - heap);
	}

	int GetRest()
	{
		return heapBytes - GetAllocated();
	}

	void* Allocate(size_t bytes, int alignment = ALIGN16)
	{
		bytes = max(bytes, MIN_ALLOC_SIZE);
		uintptr_t p = (uintptr_t)poolStack[curStack];

		if (alignment == ALIGN128)
		{
			p = (p + 127) & ~127;
			bytes = (bytes + 127) & ~127;
		}
		else
		{
			p = (p + 15) & ~15;
			bytes = (bytes + 15) & ~15;
		}

		poolStack[++curStack] = (unsigned char*)(p + bytes);
		
		rest = GetRest();

		return (void*)p;

	}

	void Deallocate(void* p)
	{

		(void)p;
		--curStack;
	}

	void Clear()
	{
		poolStack[0] = heap;
		curStack = 0;
		rest = 0;
	}

};



#endif