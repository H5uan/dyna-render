#include "Time.h"

TimeElapse::TimeElapse() {
    this->m_TimePoint = std::chrono::system_clock::now();
}

double TimeElapse::ComputeElapse() {
    const auto end = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_TimePoint);
    return static_cast<double>(duration.count()) / 1000.0;
}
