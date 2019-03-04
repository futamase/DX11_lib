#pragma once

#include <malloc.h>
#include <exception>


// operator newとdeleteを特別にアライメントしたメモリを確保したいクラスに継承する
//使い方：
//   __declspec(align(16)) struct MyAlignedType : public AlignedNew<MyAlignedType>

template<typename TDerived>
struct AlignedNew
{
	static void* operator new (size_t size)
	{
		const size_t alignment = __alignof(TDerived);

		static_assert(alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment. Did you forget a __declspec(align) on TDerived?");

		void* ptr = _aligned_malloc(size, alignment);

		if (!ptr)
			throw std::bad_alloc();

		return ptr;
	}

	static void operator delete (void* ptr)
	{
		_aligned_free(ptr);
	}

	static void* operator new[](size_t size)
	{
		return operator new(size);
	}

	static void operator delete[](void* ptr)
	{
		operator delete(ptr);
	}
};
