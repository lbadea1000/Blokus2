//
// Created by Lucian Badea on 01.07.2024.
//

#pragma once

class Clock {
public:
    bool isOn = false;
    void startClock();
    double stopClock();
private:
    long long startTime;
    long long getNanoseconds();
};