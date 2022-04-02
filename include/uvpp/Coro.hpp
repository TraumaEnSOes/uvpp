#ifndef UVPP_CORO_HPP
#define UVPP_CORO_HPP

#include <cassert>
#include <exception>
#include <ostream>
#include <string>

#ifdef UVPP_NOMT
#define UVPP_MT_MUTEX
#define UVPP_MT_LOCKGUARD( M )
#else
#include <mutex>
#define UVPP_MT_MUTEX( M ) std::mutex M
#define UVPP_MT_LOCKGUARD( M ) std::lock_guard< std::mutex > lockGuard( M )
#endif

#include "uvpp/CoroState.hpp"
#include "uvpp/details/CoroHelper.hpp"
#include "uvpp/details/Kernel.hpp"

namespace uvpp {

struct CoroPrivate;

class Coro {
    friend inline std::ostream &operator<<( std::ostream &os, const Coro &coro ) {
        return os << "Coro<" << coro.m_private << '>';
    }

public:
    template< typename FN > Coro( unsigned stackSize, FN &&fn )
    {
        using Helper = details::CoroHelper< void >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.fn = std::move( fn );

        m_private = details::createCoro( stackSize, Helper::exec );
    }
    template< typename FN > Coro( FN &&fn ) {
        using Helper = details::CoroHelper< void >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.fn = std::move( fn );

        m_private = details::createCoro( 0, Helper::exec );
    }
    template< typename FN, typename... ARGS > Coro( size_t stackSize, FN &&fn, ARGS... args ) {
        using Helper = details::CoroHelper< ARGS... >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );
        helper.fn = std::move( fn );

        m_private = details::createCoro( stackSize, Helper::exec );
    }
    template< typename FN, typename... ARGS > Coro( FN &&fn, ARGS... args ) {
        using Helper = details::CoroHelper< ARGS... >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );
        helper.fn = std::move( fn );

        m_private = details::createCoro( 0, Helper::exec );
    }
    Coro( CoroPrivate *priv = nullptr ) :
        m_private( priv )
    {
    }
    Coro( const Coro &other ) = default;
    Coro( Coro &&other ) = default;

    Coro &operator=( CoroPrivate *priv ) {
        m_private = priv;
        return *this;
    }
    Coro &operator=( const Coro &other ) = default;
    Coro &operator=( Coro &&other ) = default;

    operator bool( ) const noexcept { return m_private != nullptr; }
    bool operator!( ) const noexcept { return m_private == nullptr; }

    bool operator==( const Coro &other ) const noexcept { return m_private == other.m_private; }
    bool operator==( CoroPrivate *other ) const noexcept { return m_private == other; }
    bool operator!=( const Coro &other ) const noexcept { return m_private != other.m_private; }
    bool operator!=( CoroPrivate *other ) const noexcept { return m_private != other; }

    CoroState state( ) const noexcept {
        assert( m_private != nullptr );

        return details::state( m_private );
    }

    const std::string &name( ) const noexcept {
        assert( m_private != nullptr );

        return details::name( m_private );
    }

    Coro &setName( ) noexcept {
        assert( m_private != nullptr );

        details::setName( m_private );

        return *this;
    }

    Coro &setName( std::string n ) noexcept {
        assert( m_private != nullptr );

        details::setName( m_private, std::move( n ) );

        return *this;
    }

    Coro &dtach( ) noexcept {
        assert( m_private != nullptr );

        details::dtach( m_private );

        return *this;
    }

    std::exception_ptr join( ) {
        assert( m_private != nullptr );

        return details::join( m_private );
    }

    static Coro thisCoro( ) noexcept { return Coro( details::thisCoro( ) ); }

private:
    CoroPrivate *m_private = nullptr;
};

} // namespace uvpp

#endif
