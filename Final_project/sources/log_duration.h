#pragma once
// œ–Œ‘»À»–Œ¬Ÿ» 

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x, stream) LogDuration UNIQUE_VAR_NAME_PROFILE(x, stream) 

#include <chrono>
#include <iostream>
#include <string>

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;

    LogDuration(std::string operation, std::ostream& stream = std::cerr) :
        operation_(operation),
        stream_(stream)
    {}

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        stream_ << operation_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

    std::ostream& stream_;

private:
    std::string operation_;
    const Clock::time_point start_time_ = Clock::now();
};