#ifndef UVPP_DETAILS_COROHELPER_HPP
#define UVPP_DETAILS_COROHELPER_HPP

#include <functional>
#include <mutex>
#include <tuple>

#include "uvpp/details/Kernel.hpp"

namespace uvpp {

namespace details {

template< typename... ARGS > struct CoroHelper {
    std::function< void( ARGS... ) > fn;
    std::tuple< ARGS... > tuple;
    std::mutex mutex;

    static void exec( ) {
        std::function< void( ARGS... ) > target;
        std::tuple< ARGS... > arguments;
        std::exception_ptr err;

        target = std::move( singleton.fn );
        arguments = std::move( singleton.tuple );

        firstYield( );

        try {
            std::apply( target, arguments );
        } catch( ... ) {
            err = std::current_exception( );
        }

        exit( std::move( err ) );
    }

    static CoroHelper< ARGS... > singleton;
};
template< typename... ARGS > CoroHelper< ARGS... > CoroHelper< ARGS... >::singleton;

template< > struct CoroHelper< void > {
    std::function< void( ) > fn;
    std::mutex mutex;

    static void exec( ) {
        std::function< void( ) > target;
        std::exception_ptr err;

        target = std::move( singleton.fn );

        firstYield( );

        try {
            target( );
        } catch( ... ) {
            err = std::current_exception( );
        }

        exit( std::move( err ) );
    }

    static CoroHelper< void > singleton;
};
inline CoroHelper< void > CoroHelper< void >::singleton;

} // namespace details.

} // namespace uvpp.

#endif
