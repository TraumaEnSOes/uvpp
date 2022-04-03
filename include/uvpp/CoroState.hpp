#ifndef UVPP_COROSTATE_H
#define UVPP_COROSTATE_H

namespace uvpp {

enum CoroState {
    READY,
    WAITING,
    FINISHED,
    CANCELLED
};

} // namespace uvpp.

#endif
