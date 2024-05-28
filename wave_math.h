#ifndef WAVE_MATH_H
#define WAVE_MATH_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "config.h"

double sgn(double x);
double modulo(double x, double y);
double square_wave(double x);
double saw_wave(double x, double period);
double triangle_wave(double x, double amp, double period);
double table_increment(double frequency);
bool jump_discontinuity(int y1, int y2);

#endif
