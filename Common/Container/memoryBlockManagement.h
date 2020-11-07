#ifndef MEMORYBLOCKMANAGEMENT_H
#define MEMORYBLOCKMANAGEMENT_H
#include <stdint.h>

#define defaultMemorysize 1024
#define IncMemroySize   2
class memoryBlockManagement
{
public:
    memoryBlockManagement();
    memoryBlockManagement(const memoryBlockManagement& obj);
    ~memoryBlockManagement();
    void* getMemory(uint64_t size);
    memoryBlockManagement operator=(const memoryBlockManagement& obj);
    void* getNewMemory(uint64_t size);



private:
    uint64_t memorySize;
    void*    m_memoryBlock;
};

#endif // MEMORYBLOCKMANAGEMENT_H
