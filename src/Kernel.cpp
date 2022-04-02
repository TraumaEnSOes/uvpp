#include "CoroPrivate.hpp"
#include "ErrorMessage.hpp"
#include "LoopPrivate.hpp"
#include "uvpp/Coro.hpp"
#include "uvpp/Exception.hpp"
#include "uvpp/details/Kernel.hpp"

static constexpr unsigned DEFAULT_STACK_SIZE = 131072U;
static constexpr unsigned DEFAULT_RUN_STACK_SIZE = 65536U;

namespace uvpp {

void yield( ) noexcept {
    co_switch( LoopData->SchedulerCoro );
}

void exit( std::exception_ptr err ) noexcept {
    auto &coro = *( LoopData->CurrentCoro );

    if( err ) {
        coro.exception = std::move( err );
    }

    coro.state = CoroState::FINISHED;

    co_switch( LoopData->SchedulerCoro );
}

namespace details {

void firstYield( ) noexcept {
    if( LoopData->CurrentCoro->prev( ) != LoopData->CurrentCoro ) {
        LoopData->CurrentCoro = LoopData->CurrentCoro->prev( );
    }

    co_switch( LoopData->SchedulerCoro );
}

void run( unsigned stackSize, void (*fn)( ) ) {
    if( LoopData->ActiveCoros != 0 ) {
        throw Exception( "'uvpp::run( )' called twice" );
    }

    if( stackSize == 0 ) {
        stackSize = DEFAULT_RUN_STACK_SIZE;
    }

    LoopData->SchedulerCoro = co_active( );

    auto firstCoro = new CoroPrivate( LoopData );
    firstCoro->cothread = co_create( stackSize, fn );

    if( firstCoro->cothread == nullptr ) { throw std::bad_alloc( ); }

    firstCoro->dtached = true;

    LoopData->CurrentCoro = firstCoro;
    LoopData->ActiveCoros = 1;

    // Main loop.
    while( LoopData->ActiveCoros != 0 ) {
        co_switch( LoopData->CurrentCoro->cothread );

        auto outgoingCoro = LoopData->CurrentCoro;
        LoopData->CurrentCoro = LoopData->CurrentCoro->next( );

        if( outgoingCoro->state != CoroState::READY ) {
            --( LoopData->ActiveCoros );

            if( outgoingCoro->state == CoroState::FINISHED && ( outgoingCoro->dtached == false ) ) {
                 throw Exception( "Non-detached coro as finished without join" );
                 std::terminate( );
            }

            if( LoopData->ActiveCoros != 0 ) {
                outgoingCoro->unlink( );
            }

            delete outgoingCoro;
        }
    }
}

CoroPrivate *createCoro( unsigned stackSize, void (*fn)( ) ) {
    if( stackSize == 0 ) {
        stackSize = DEFAULT_STACK_SIZE;
    }

    auto newPrivate = new CoroPrivate( LoopData );
    newPrivate->cothread = co_create( stackSize, fn );
    if( newPrivate->cothread == nullptr ) { throw std::bad_alloc( ); }

    ( LoopData->ActiveCoros ) += 1;

    LoopData->CurrentCoro->insertBefore( newPrivate );

    // To fast free locks, and allow creator to dtach
    // LoopData->CurrentCoro = LoopData->CurrentCoro->prev;
    co_switch( newPrivate->cothread );

    return newPrivate;
}

CoroPrivate *thisCoro( ) noexcept {
    return LoopData->CurrentCoro;
}

CoroState state( const CoroPrivate *coro ) noexcept {
    return coro->state;
}

const std::string &name( const CoroPrivate *coro ) noexcept {
    return coro->name;
}

void setName( CoroPrivate *coro ) noexcept {
    coro->name.clear( );
}

void setName( CoroPrivate *coro, std::string n ) noexcept {
    coro->name = std::move( n );
}

void dtach( CoroPrivate *coro ) noexcept {
    coro->dtached = true;
}

} // namespace details.

} // namespace uvpp.
