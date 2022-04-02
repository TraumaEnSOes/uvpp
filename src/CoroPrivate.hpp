#ifndef COROPRIVATE_HPP
#define COROPRIVATE_HPP

#include "libco/libco.h"

#include "CListNode.hpp"

#include "uvpp/CoroState.hpp"

#include <string>

struct LoopPrivate;

namespace uvpp {

struct CoroPrivate : public CListNode {
    CoroPrivate( LoopPrivate *l ) :
        CListNode( ),
        loop( l )
    {
    }

    LoopPrivate *loop;
    CoroState state = CoroState::READY;
    bool dtached = false;
    CoroPrivate *waitedBy = nullptr;
    CoroPrivate *waitTo = nullptr;

    std::exception_ptr exception;
    std::string name;

    cothread_t cothread;
};

} // namespace uvpp.

#endif

