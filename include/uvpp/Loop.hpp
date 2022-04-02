#ifndef UVPP_LOOP_HPP
#define UVPP_LOOP_HPP

#include <cassert>
#include <type_traits>

#include "uvpp/details/CoroHelper.hpp"
#include "uvpp/details/Kernel.hpp"

namespace uvpp {

class Loop {
public:
    ~Loop( );
    Loop( );
    Loop( const Loop & ) = delete;
    Loop( Loop && ) = delete;
    Loop &operator=( const Loop & ) = delete;
    Loop &operator=( Loop && ) = delete;

    template< typename FN > void run( unsigned stackSize, FN &&fn ) noexcept {
        using Helper = details::CoroHelper< void >;

        // ensureRun< FN >( fn );

        auto &helper = Helper::singleton;

        helper.fn = std::move( fn );

        details::run( stackSize, helper.exec );
    }

    template< typename FN > void run( FN &&fn ) noexcept {
        using Helper = details::CoroHelper< void >;

        // ensureRun< FN >( fn );

        auto &helper = Helper::singleton;

        helper.fn = std::move( fn );

        details::run( 0, helper.exec );
    }

    template< typename FN, typename... ARGS > void run( unsigned stackSize, FN &&fn, ARGS... args ) noexcept {
        using Helper = details::CoroHelper< ARGS... >;

        //ensureRun< FN >( fn );

        auto &helper = Helper::singleton;

        helper.fn = std::move( fn );
        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );

        details::run( stackSize, helper.exec );
    }

    template< typename FN, typename... ARGS > void run( FN &&fn, ARGS... args ) noexcept {
        using Helper = details::CoroHelper< ARGS... >;

        //ensureRun< FN >( fn );

        auto &helper = Helper::singleton;

        helper.fn = std::move( fn );
        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );

        details::run( 0, helper.exec );
    }

private:
    template< typename FN > static inline void ensureRun( const FN &fn ) noexcept {
        if constexpr( std::is_pointer< FN >::value == true ) {
            assert( fn != nullptr );
        } else {
            assert( fn.operator bool( ) );
        }
    }
};

} // namespace uvpp.

#endif
