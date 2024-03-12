#pragma once

#include <chrono>

class TimeElapse {
public:
    TimeElapse();
    ~TimeElapse() = default;
    double ComputeElapse();

private:
    std::chrono::system_clock::time_point m_TimePoint;
};
