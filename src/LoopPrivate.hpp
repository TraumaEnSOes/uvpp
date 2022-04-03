#ifndef LOOPPRIVATE_HPP
#define LOOPPRIVATE_HPP

#include "CoroPrivate.hpp"

#include "libco/libco.h"

#include <cassert>

struct LoopPrivate {
    uvpp::CoroPrivate *CurrentCoro = nullptr;
    cothread_t SchedulerCoro = nullptr;
    int ActiveCoros = 0;
    char errorBuffer[1024];

    void insertBefore( uvpp::CoroPrivate *coro ) noexcept {
        insertBefore( coro );
        ++ActiveCoros;
    }

    void reinsertBefore( uvpp::CoroPrivate *coro ) noexcept {
        assert( coro );
        assert( CurrentCoro != coro );

        if( CurrentCoro ) {
            CurrentCoro->insertBefore( coro );
        } else {
            coro->reset( );
            CurrentCoro = coro;
        }
    }

    void unlink( uvpp::CoroPrivate *coro ) noexcept {
        if( ( CurrentCoro == coro ) && ( CurrentCoro->isUnique( ) ) ) {
            CurrentCoro = nullptr;
        } else {
            coro->unlink( );
        }
    }

    void advanceCoro( ) noexcept {
        if( CurrentCoro ) {
            CurrentCoro = CurrentCoro->next( );
        }
    }
};

extern thread_local LoopPrivate *LoopData;

#endif
