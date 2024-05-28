#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "portaudio.h"
#include "config.h"
#include "structs.h"
#include "wave_math.h"

void init_portaudio(PaStream** stream);
void init_sdl(SDL_Renderer** renderer, SDL_Window** window, int screen_width,
    int screen_height);
void init_divisions(Divisions* divisions, int screen_width, int screen_height);
void init_boxes(Boxes* boxes, int screen_width, int screen_height);
void init_partials(Wave* partials);
void init_lfos(Wave* lfos);
void init_state(State* state);
void init_parameters(Parameters* parameters);
void init_text(Text* text, Boxes* boxes);
void init_controls(Controls* controls, Boxes* boxes);
void init_wavetables(Wavetables* wavetables);

#endif
