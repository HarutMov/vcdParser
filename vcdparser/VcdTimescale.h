#ifndef VCD_TIMESCALE_H_
#define VCD_TIMESCALE_H_

struct VcdTimescale
{
    std::string time_number;
    std::string time_unit;

    VcdTimescale() : time_number("10"), time_unit("ns")
    {
    }
};

#endif //VCD_TIMESCALE_H_
