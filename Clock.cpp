//
// Created by Lucian Badea on 01.07.2024.
//
#include "Clock.h"
#include <ctime>
#include <cstdio>
#include <chrono>

long long Clock::getNanoseconds() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now);
    return now_ns.count();
}

void Clock::startClock() {
    isOn = true;
    startTime = getNanoseconds();
}

double Clock::stopClock() {
    return (double) (getNanoseconds() - startTime) * 1e-9;
}