#ifndef __VPMUTEX_H__
#define __VPMUTEX_H__

class VPMutex
{
public:
    VPMutex();
    virtual ~VPMutex();

    void lock();
    void unlock();

private:
    void* c_ctrl;

};

#endif  //__VPMUTEX_H__
