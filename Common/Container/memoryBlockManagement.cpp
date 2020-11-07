#include "memoryBlockManagement.h"
#include "QObject"
#include "global.h"
#include "string"
#include <stdio.h>

memoryBlockManagement::memoryBlockManagement()
{
	memorySize	  = defaultMemorysize;
	m_memoryBlock = new uint8_t(memorySize);
}

memoryBlockManagement::memoryBlockManagement(const memoryBlockManagement& obj)
{
	if (m_memoryBlock != obj.m_memoryBlock)
	{
		memorySize	  = defaultMemorysize;
		m_memoryBlock = new uint8_t(memorySize);
		memcpy(m_memoryBlock, obj.m_memoryBlock, memorySize);
	}
}

memoryBlockManagement::~memoryBlockManagement()
{
	SafeDeleteArray(m_memoryBlock);
	memorySize = 0;
}

void* memoryBlockManagement::getMemory(uint64_t size)
{
	if (memorySize < size)
	{
		uint64_t* memtemp = new uint64_t(memorySize);
		SafeDeleteArray(memtemp);
		memorySize	  = IncMemroySize * memorySize;
		m_memoryBlock = new uint8_t(memorySize);
	}

	return m_memoryBlock;
}

memoryBlockManagement memoryBlockManagement::operator=(const memoryBlockManagement& obj)
{
	if (m_memoryBlock != obj.m_memoryBlock)
	{
		memorySize	  = obj.memorySize;
		m_memoryBlock = new uint8_t(memorySize);
		memcpy(m_memoryBlock, obj.m_memoryBlock, memorySize);
		return *this;
	}
}

void* memoryBlockManagement::getNewMemory(uint64_t size)
{
	uint8_t* memtemp = new uint8_t[size];
	return memtemp;
}
