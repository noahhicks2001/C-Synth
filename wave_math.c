#include "wave_math.h"

double sgn(double x) {
    if (x < 0.0) {
        return -1.0;
    }
    else if (x == 0.0) {
        return 0.0;
    }
    else {
        return 1.0;
    }
}

double modulo(double x, double y) {
    return x - floor(x / y) * y;
}
double square_wave(double x) {
    return sgn(sin(x));
}

double saw_wave(double x, double period) {
    return 2.0 * ((x / period) - floor((1.0 / 2.0) + (x / period)));
}

double triangle_wave(double x, double amp, double period) {
    return ((4.0 * amp) / period) * fabs(modulo((x - (period / 4.0)), period) - (period / 2.0)) - amp;
}

double table_increment(double frequency) {
    return WAVETABLE_SIZE * frequency * 1.0 / (double)SAMPLE_RATE;
}


bool jump_discontinuity(int y1, int y2) {
    if (y1 == 0 || y2 == 0) {
        return false;
    }
    return fabs(y1 - y2) > 1.0;
}
