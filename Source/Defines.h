#pragma once

#define MAX_FLOAT 150000.0f

#define BACK_WIDTH 825
#define BACK_HEIGHT 300

#define OSC_WIDTH 627
#define OSC_HEIGHT 269
#define OSC_CENTER (OSC_HEIGHT / 2)

inline float clip(float x, float max = 1.0f)
{
    if (x > max) {
        return max;
    } else if (x < -max) {
        return -max;
    } else {
        return x;
    }
}
