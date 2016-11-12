#pragma once

#include <sys/time.h>
#include <chrono>

namespace RJ {

/// type for storing time in microseconds
typedef std::chrono::steady_clock::time_point Time;
typedef uint64_t Timestamp;
/** returns the local system timestamp in microseconds */


template <class Duration>
auto numMicroseconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

inline Time now() {
    auto time_point = std::chrono::steady_clock::now();
    //struct timeval time;
    //gettimeofday(&time, nullptr);
    return time_point;
    //return (Time)time.tv_sec * 1000000 + (Time)time.tv_usec;
}

inline Timestamp timestamp(Time time) {
    return numMicroseconds(time.time_since_epoch());
}

inline Timestamp timestamp() {
    return timestamp(now());
}

/// Converts a decimal number of seconds to an integer timestamp in microseconds
inline RJ::Timestamp SecsToTimestamp(double secs) {
    return secs * 1000000.0f;
}

/// Converts an integer timestamp in microseconds to a decimal number of seconds
inline float TimestampToSecs(RJ::Timestamp timestamp) {
    return (float)(timestamp / 1000000.0f);
}

}  // namespace RJ
