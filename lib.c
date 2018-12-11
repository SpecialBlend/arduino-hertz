#pragma once

float vector(float x, float y, float z) {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

float avg(float high, float low) {
    return (high + low) / 2;
}