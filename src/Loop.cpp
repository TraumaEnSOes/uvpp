#include "uvpp/Loop.hpp"

#include "LoopPrivate.hpp"

#include <exception>

thread_local LoopPrivate *LoopData;

namespace uvpp {

Loop::~Loop( ) {
    if( LoopData->ActiveCoros != 0 ) {
        std::terminate( );
    }

    delete LoopData;
    LoopData = nullptr;
}

Loop::Loop( ) {
    LoopData = new LoopPrivate;
}

} // namespace uvpp.
