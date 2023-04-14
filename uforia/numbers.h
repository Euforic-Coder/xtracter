#ifndef NUMBERS_H
#define NUMBERS_H

#include <iostream>
#include <chrono>
#include <climits>

namespace uforia{
    
    // Checks if an int is in a range
    bool in_range(int val, int min, int max){
        return val < max && val > min;
    }

    // Generates a random int in a range
    int truly_random(const int min = 0, const int max = INT_MAX){
        srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        int random = rand() % max;
        while(random <= min){
            random = rand() % max;
        }
        return random;
    }

    // Linear interpolation
    float linear_interpolate(int val, const int min, int max){
        if(min < max && val < max && val > min){
            max = max - min;
            val = val - min;
            return (float) val / max;
        }
    }
};

#endif //NUMBERS_H
