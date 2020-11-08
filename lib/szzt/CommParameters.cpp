#include "CommParameters.h"

CommParameters::CommParameters()
    : porttype(COM)
    , portno(1)
    , m_baudrate(BAUD_RATE_115200)
    , m_parity(MY_PARITY_NONE)
    , m_flowcontrol(FLOWCONTROL_HARDWARE)
{
}

CommParameters::~CommParameters()
{
}
