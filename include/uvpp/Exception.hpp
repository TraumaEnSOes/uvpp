#ifndef UVPP_EXCEPTION_HPP
#define UVPP_EXCEPTION_HPP

#include <stdexcept>

namespace uvpp {

class Exception : public std::logic_error {
public:
    Exception( const char *msg ) : std::logic_error( msg ) { }
    Exception( std::string msg ) : std::logic_error( std::move( msg ) ) { }
};

} // namespace uvpp.

#endif
