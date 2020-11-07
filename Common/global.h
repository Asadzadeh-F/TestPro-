#ifndef GLOBAL
#define GLOBAL

#include <cstdint>
#include <string>

//#if !defined(nullptr)
#define nullptr 0
//#endif

#define SafeDelete(x) \
	if (x != nullptr) \
	{                 \
		delete x;     \
		x = nullptr;  \
	}

#define SafeDeleteArray(x) \
	if (x != nullptr)      \
	{                      \
		delete[] x;        \
		x = nullptr;       \
	}

#endif // GLOBAL
