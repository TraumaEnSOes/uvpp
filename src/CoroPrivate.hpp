#ifndef COROPRIVATE_HPP
#define COROPRIVATE_HPP

#include "libco/libco.h"

#include "uvpp/CoroState.hpp"

#include <string>

struct LoopPrivate;

namespace uvpp {

struct CoroPrivate {
    CoroPrivate( LoopPrivate *l ) :
        loop( l )
    {
    }

    LoopPrivate *loop;
    CoroState state = CoroState::READY;
    bool dtached = false;
    CoroPrivate *waitedBy = nullptr;
    CoroPrivate *waitTo = nullptr;

    CoroPrivate *prev;
    CoroPrivate *next;

    std::exception_ptr exception;
    std::string name;

    cothread_t cothread;
};

} // namespace uvpp.

#endif

