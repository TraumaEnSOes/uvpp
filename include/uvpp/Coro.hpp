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
    friend class CoroPrivate;

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

        m_private = details::createCoro( stackSize, this, Helper::exec );
    }
    template< typename FN > Coro( FN &&fn ) {
        using Helper = details::CoroHelper< void >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.fn = std::move( fn );

        m_private = details::createCoro( 0, this, Helper::exec );
    }
    template< typename FN, typename... ARGS > Coro( size_t stackSize, FN &&fn, ARGS... args ) {
        using Helper = details::CoroHelper< ARGS... >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );
        helper.fn = std::move( fn );

        m_private = details::createCoro( stackSize, this, Helper::exec );
    }
    template< typename FN, typename... ARGS > Coro( FN &&fn, ARGS... args ) {
        using Helper = details::CoroHelper< ARGS... >;
        auto &helper = Helper::singleton;

        UVPP_MT_LOCKGUARD( helper.mutex );

        helper.tuple = std::make_tuple( std::forward< ARGS >( args )... );
        helper.fn = std::move( fn );

        m_private = details::createCoro( 0, this, Helper::exec );
    }
    Coro( CoroPrivate *priv = nullptr ) :
        m_private( priv )
    {
    }
    Coro( const Coro &other ) = delete;
    Coro( Coro &&other ) :
        m_private( other.m_private )
    {
        if( m_private ) {
            other.m_private = nullptr;
            details::setCoro( m_private, this );
        }
    }

    Coro &operator=( const Coro &other ) = delete;
    Coro &operator=( Coro &&other ) {
        m_private = other.m_private;

        if( m_private ) {
            other.m_private = nullptr;
            details::setCoro( m_private, this );
        }

        return *this;
    }

    operator bool( ) const noexcept { return m_private != nullptr; }
    bool operator!( ) const noexcept { return m_private == nullptr; }

    bool operator==( const Coro &other ) const noexcept { return m_private == other.m_private; }
    bool operator!=( const Coro &other ) const noexcept { return m_private != other.m_private; }

    [[nodiscard]] bool joinable( ) const noexcept {
        return m_private ?
            details::joinable( m_private ) :
            false;
    }

    void requestStop( ) noexcept {
        if( m_private ) {
            details::requestStop( m_private );
        }
    }

    [[nodiscard]] bool stopRequested( ) const noexcept {
        assert( m_private );

        return details::stopRequested( m_private );
    }

    void cancel( ) {
        if( m_private ) {
            details::cancel( m_private );
        }
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

    Coro &detach( ) noexcept {
        assert( m_private != nullptr );

        details::detach( m_private );

        return *this;
    }

    std::exception_ptr join( ) noexcept {
        assert( m_private != nullptr );

        return details::join( m_private );
    }

    static Coro thisCoro( ) noexcept { return Coro( details::thisCoro( ) ); }

private:
    CoroPrivate *m_private = nullptr;
};

} // namespace uvpp

#endif
