#pragma once

#include <sys/time.h>
#include <chrono>
#include <string>

namespace RJ {

/// type for storing time in microseconds
typedef std::chrono::system_clock::time_point Time;
typedef uint64_t Timestamp;
typedef std::chrono::duration<double> Seconds;
/** returns the local system timestamp in microseconds */


template <class Duration>
constexpr auto numMicroseconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

inline Time now() {
    return std::chrono::system_clock::now();
    //struct timeval time;
    //gettimeofday(&time, nullptr);
    //return (Time)time.tv_sec * 1000000 + (Time)time.tv_usec;
}

constexpr Timestamp timestamp(Time time) {
    return numMicroseconds(time.time_since_epoch());
}

inline Timestamp timestamp() {
    return timestamp(now());
}

/// Converts a decimal number of seconds to an integer timestamp in microseconds
constexpr  RJ::Timestamp SecsToTimestamp(double secs) {
    return secs * 1000000.0f;
}

template <class Duration>
constexpr auto numSeconds(Duration d) {
    std::chrono::duration<double> seconds = d;
    return seconds.count();
}

/// Converts an integer timestamp in microseconds to a decimal number of seconds
constexpr float TimestampToSecs(RJ::Timestamp timestamp) {
    return (float)(timestamp / 1000000.0f);
}

}  // namespace RJ

inline RJ::Time operator+(const RJ::Time &time, const RJ::Seconds& sec) {
    return time + std::chrono::duration_cast<RJ::Time::duration>(sec);
}

inline RJ::Time operator-(const RJ::Time &time, const RJ::Seconds& sec) {
    return time - std::chrono::duration_cast<RJ::Time::duration>(sec);
}

inline std::string to_string(RJ::Seconds seconds) {
    return std::to_string(seconds.count()) + "(Seconds)";
}
