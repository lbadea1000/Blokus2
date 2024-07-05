//
// Created by Lucian Badea on 03.07.2024.
//

#pragma once
#include <chrono>
#include <random>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int randomNr(int a, int b) {
    return uniform_int_distribution<int>(a, b)(rng);
}