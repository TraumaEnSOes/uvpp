#ifndef UVPP_DETAILS_KERNEL_HPP
#define UVPP_DETAILS_KERNEL_HPP

#include "uvpp/CoroState.hpp"

#include <exception>

namespace uvpp {

struct CoroPrivate;

void yield( ) noexcept;
void exit( std::exception_ptr err ) noexcept;
inline void exit( ) { exit( std::exception_ptr( ) ); }

namespace details {

void firstYield( ) noexcept;
void run( unsigned stackSize, void (*fn)( ) );
CoroPrivate *createCoro( unsigned stackSize, void (*fn)( ) );
CoroPrivate *thisCoro( ) noexcept;

CoroState state( const CoroPrivate *coro ) noexcept;
const std::string &name( const CoroPrivate *coro ) noexcept;
void setName( CoroPrivate *coro ) noexcept;
void setName( CoroPrivate *coro, std::string n ) noexcept;
void dtach( CoroPrivate *coro ) noexcept;

}

} // namespace uvpp.

#endif
