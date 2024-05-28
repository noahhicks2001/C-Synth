#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config.h"
#include "portaudio.h"

typedef struct Divisions {
    SDL_Rect partials;
    SDL_Rect wavetables;
    SDL_Rect textboxes;
} Divisions;

typedef struct Boxes {
    SDL_Rect additive;
    SDL_Rect partials[PARTIAL_COUNT];
    SDL_Rect controls;
    SDL_Rect wavetables[WAVETABLE_COUNT];
    SDL_Rect textboxes[TEXTBOX_COUNT];
} Boxes;

typedef struct Wave {
    int type;
    double frequency;
    double visual_phase;
    double table_increment;
    double table_phase;
} Wave;

typedef struct Parameters {
    double volume;
    double tremolo;
    double wobble;
    double additive_zoom;
    double partial_zoom[PARTIAL_COUNT];
} Parameters;

typedef struct State {
    int active_partials;
    int selected_wavetable;
    int selected_partial;
    int selected_textbox;
    bool running;
} State;

typedef struct Text {
    char text[TEXTBOX_COUNT][7];
    TTF_Font* font;
    SDL_Color color;
    SDL_Rect  textboxes[TEXTBOX_COUNT];
} Text;


typedef struct Controls {
    SDL_Point tracks[CONTROL_COUNT][2];
    SDL_Rect  knobs[CONTROL_COUNT];
} Controls;


typedef struct Wavetables {
    float sine[WAVETABLE_SIZE];
    float square[WAVETABLE_SIZE];
    float saw[WAVETABLE_SIZE];
    float triangle[WAVETABLE_SIZE];
} Wavetables;


typedef struct Synthesis_data {
    PaStream* stream;
    Wave* partials;
    Wave* lfos;
    State* state;
    Parameters* parameters;
    Wavetables* wavetables;
} Synthesis_data;
#endif
