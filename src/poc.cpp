#include "uvpp/Coro.hpp"
#include "uvpp/Loop.hpp"

#include <iostream>

void worker( int number ) {
    std::cout << "Starting Worker " << number << std::endl;

    uvpp::Coro::thisCoro( ).setName( std::string( "Worker " ) + std::to_string( number ) );

    for( int x = 0; x < 5; ++x ) {
        uvpp::yield( );
        std::cout << "Worker " << number << ", x = " << x << std::endl;
    }

    std::cout << "Worker " << number << " finished" << std::endl;
}

void failWorker( int number ) {
    std::cout << "Starting Worker " << number << std::endl;
    uvpp::Coro::thisCoro( ).setName( "FailWorker" );

    throw std::runtime_error( "Manually raised 'runtime_error'" );
}

void master( ) {
    std::cout << "Starting Master" << std::endl;

    uvpp::Coro::thisCoro( ).setName( "Master" );

    int idx;
    for( idx = 0; idx < 3; ++idx ) {
        std::cout << "Creating Worker " << idx << std::endl;
        uvpp::Coro( worker, idx ).dtach( );
    }

    std::cout << "Creating final Worker" << std::endl;
    auto error = uvpp::Coro( failWorker, idx ).join( );

    if( error ) {
        std::string what;

        try {
            std::rethrow_exception( error );
        } catch( const std::exception &exc ) {
            what = exc.what( );
        }

        std::cout << "FailWorker throws\n";
        std::cout << what << std::endl;
    }

    std::cout << "Master finished" << std::endl;
}

int main( ) {
    uvpp::Loop loop;
    loop.run( 32768U, master );

    return 0;
}
