#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include "config.h"
#include "structs.h"
#include "wave_math.h"

void select_wavetable(SDL_Point* position, State* state, Boxes* boxes);
void select_partial(SDL_Point* position, State* state, Boxes* boxes);
void select_textbox(SDL_Point* position, State* state, Boxes* boxes);
void check_mouse_inputs(SDL_Event* event, State* state, Wave* partials,
    Wave* lfos, Divisions* divisions, Boxes* boxes);
void check_keyboard_inputs(SDL_Event* event, State* state, Text* text,
    Parameters* parameters, Wave* partials, Wave* lfos, Controls* controls);
void check_text_input(SDL_Event* event, State* state, Text* text);
#endif
