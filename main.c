#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_ttf.h>

#include "portaudio.h"
#include "config.h"
#include "structs.h"
#include "init.h"
#include "display.h"
#include "input.h"
#include "synthesis.h"


void handle_display(SDL_Renderer* renderer, Boxes* boxes, Wave *partials,
    Wave* lfos, State* state, Parameters *parameters, Text *text, Controls* controls) {
    clear_screen(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_boxes(renderer, boxes);
    draw_additive(renderer, boxes, partials, state, parameters);
    draw_partials(renderer, boxes, partials, state, parameters);
    draw_wavetables(renderer, boxes);
    draw_controls(renderer, boxes, controls, parameters);
    draw_textboxes(renderer, boxes, text);
    update_visual_phases(partials);
    SDL_Delay(10);
    SDL_RenderPresent(renderer);
}


void handle_input(SDL_Event* event, State* state, Parameters* parameters, Wave *partials,
    Wave* lfos, Text* text, Controls* controls, Divisions* divisions, Boxes *boxes) {

    SDL_PollEvent(event);
    if (event->type == SDL_QUIT) {
        state->running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        check_keyboard_inputs(event, state, text, parameters, partials, lfos, controls);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        check_mouse_inputs(event, state, partials, lfos, divisions, boxes);
    }
    else if (event->type == SDL_TEXTINPUT) {
        check_text_input(event, state, text);
    }
}



void handle_synthesis(void* data) {
    // cast void* and pass struct fields to function
    synthesize_audio(((Synthesis_data*)data)->stream, ((Synthesis_data*)data)->state, ((Synthesis_data*)data)->partials,
        ((Synthesis_data*)data)->lfos, ((Synthesis_data*)data)->parameters, ((Synthesis_data*)data)->wavetables);
}


int main(int argc, char* argv[]) {

    // SCREEN DIMENSIONS //
    int screen_width = 640;
    int screen_height = 480;

    // INIT PORTAUDIO //
    PaStream* stream = NULL;
    init_portaudio(&stream);


    // INIT SDL2 //
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;
    init_sdl(&renderer, &window, screen_width, screen_height);

    // INIT DIVISIONS //
    Divisions divisions;
    init_divisions(&divisions, screen_width, screen_height);

    // INIT BOXES //
    Boxes boxes;
    init_boxes(&boxes, screen_width, screen_height);

    // INIT PARTIALS //
    Wave partials[PARTIAL_COUNT];
    init_partials(&partials);

    // INIT LFOS //
    Wave lfos[LFO_COUNT];
    init_lfos(&lfos);

    // INIT STATE //
    State state;
    init_state(&state);

    // INIT PARAMETERS //
    Parameters parameters;
    init_parameters(&parameters);

    // INIT TEXT //
    Text text;
    init_text(&text, &boxes);

    // INIT CONTROLS //
    Controls controls;
    init_controls(&controls, &boxes);

    // INIT WAVETABLES //
    Wavetables* wavetables = (Wavetables*)malloc(sizeof(Wavetables));
    init_wavetables(wavetables);
   
    // INIT SYNTHESIS DATA //
    // store parameter data to pass to threaded synthesis function 
    Synthesis_data* synthesis_data = (Synthesis_data*)malloc(sizeof(Synthesis_data));
    synthesis_data->stream = stream;
    synthesis_data->lfos = lfos;
    synthesis_data->partials = partials;
    synthesis_data->state = &state;
    synthesis_data->parameters = &parameters;
    synthesis_data->wavetables = wavetables;


    // INIT AUDIO THREAD //
    SDL_Thread* synth_thread = SDL_CreateThread(handle_synthesis, "synth_thread", synthesis_data);

    while(state.running) {
        // audio handled by thread
        handle_input(&event, &state, &parameters, &partials,
            &lfos, &text, &controls, &divisions, &boxes);
        handle_display(renderer, &boxes, &partials, &lfos, &state,
            &parameters, &text, &controls);
    }

    // wait for thread to terminate
    SDL_WaitThread(synth_thread, NULL);

    // free allocated items
    free(wavetables);
    free(synthesis_data);

    // stop portaudio stream
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
   
    return 0;
}
