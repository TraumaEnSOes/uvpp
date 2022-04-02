#ifndef LOOPPRIVATE_HPP
#define LOOPPRIVATE_HPP

#include "libco/libco.h"

namespace uvpp {

struct CoroPrivate;

} // namespace uvpp.

struct LoopPrivate {
    uvpp::CoroPrivate *CurrentCoro = nullptr;
    cothread_t SchedulerCoro = nullptr;
    int ActiveCoros = 0;
    char errorBuffer[1024];
};

extern thread_local LoopPrivate *LoopData;

#endif
