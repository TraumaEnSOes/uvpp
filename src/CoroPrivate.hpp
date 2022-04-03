#ifndef COROPRIVATE_HPP
#define COROPRIVATE_HPP

#include "libco/libco.h"

#include "CListNode.hpp"

#include "uvpp/Coro.hpp"
#include "uvpp/CoroState.hpp"

#include <string>

struct LoopPrivate;

namespace uvpp {

struct CoroPrivate : public CListNode< CoroPrivate > {
    ~CoroPrivate( ) {
        if( coro ) {
            coro->m_private = nullptr;
        }

        co_delete( cothread );
    }

    CoroPrivate( LoopPrivate *l, Coro *c ) :
        CListNode( ),
        loop( l ),
        coro( c )
    {
    }

    LoopPrivate *loop;
    Coro *coro;

    bool stopRequested = false;
    CoroState state = CoroState::READY;
    bool detached = false;
    CoroPrivate *waitedBy = nullptr;
    CoroPrivate *waitTo = nullptr;

    std::exception_ptr exception;
    std::string name;

    cothread_t cothread;
};

} // namespace uvpp.

#endif

