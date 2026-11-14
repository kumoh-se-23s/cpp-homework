#ifndef REQUIRE_GUARD_H
#define REQUIRE_GUARD_H
#include <stdexcept>

template <typename Any>
void require_non_null(const Any* maybe_nullptr) {
    if (maybe_nullptr == nullptr) {
        throw std::invalid_argument("Unexpected nullptr received: expected a valid non-null pointer.");
    }
}

inline void require_positive(int maybe_negative) {
    if (maybe_negative < 0) {
        throw std::invalid_argument("Unexpected negative value: expected a positive number.");
    }
}
#endif //REQUIRE_GUARD_H
