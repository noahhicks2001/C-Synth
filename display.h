#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config.h"
#include "structs.h"
#include "wave_math.h"

void clear_screen(SDL_Renderer* renderer);
void draw_boxes(SDL_Renderer* renderer, Boxes* boxes);
void update_visual_phases(Wave* partials);
double position_wave_x(double x, SDL_Rect* box);
double position_wave_y(double y, SDL_Rect* box);
void draw_partials(SDL_Renderer* renderer, Boxes* boxes, Wave* partials,
    State* state, Parameters* parameters);
void draw_additive(SDL_Renderer* renderer, Boxes* boxes, Wave* partials,
    State* state, Parameters* parameters);
void draw_wavetables(SDL_Renderer* renderer, Boxes* boxes);
void draw_textboxes(SDL_Renderer* renderer, Boxes* boxes, Text* text);
void draw_controls(SDL_Renderer* renderer, Boxes* boxes, Controls* controls,
    Parameters* parameters);
#endif
