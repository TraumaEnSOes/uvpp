#ifndef UVPP_DETAILS_KERNEL_HPP
#define UVPP_DETAILS_KERNEL_HPP

#include "uvpp/CoroState.hpp"

#include <exception>

namespace uvpp {

struct CoroPrivate;
struct Coro;

void yield( ) noexcept;
void exit( std::exception_ptr err ) noexcept;
inline void exit( ) { exit( std::exception_ptr( ) ); }

namespace details {

void firstYield( ) noexcept;
void run( unsigned stackSize, void (*fn)( ) );
CoroPrivate *createCoro( unsigned stackSize, Coro *origin, void (*fn)( ) );
CoroPrivate *thisCoro( ) noexcept;

bool setCoro( CoroPrivate *coro, Coro *self ) noexcept;
bool joinable( const CoroPrivate *coro ) noexcept;
void requestStop( CoroPrivate *coro ) noexcept;
bool stopRequested( const CoroPrivate *coro ) noexcept;
void cancel( CoroPrivate *coro ) noexcept;
const std::string &name( const CoroPrivate *coro ) noexcept;
void setName( CoroPrivate *coro ) noexcept;
void setName( CoroPrivate *coro, std::string n ) noexcept;
void detach( CoroPrivate *coro ) noexcept;
std::exception_ptr join( CoroPrivate *m_private );

}

} // namespace uvpp.

#endif
