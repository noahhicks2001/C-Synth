#include "init.h"


void init_portaudio(PaStream** stream) {

    PaStreamParameters output_parameters;
    PaError pa_error;

    pa_error = Pa_Initialize();
    if (pa_error != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(pa_error));
    }

    output_parameters.device = Pa_GetDefaultOutputDevice();
    if (output_parameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device.\n");
    }

    output_parameters.channelCount = 2;  // stereo channels
    output_parameters.sampleFormat = paFloat32;
    output_parameters.suggestedLatency = Pa_GetDeviceInfo(output_parameters.device)->defaultLowInputLatency;
    output_parameters.hostApiSpecificStreamInfo = NULL;

    pa_error = Pa_OpenStream(
        stream,
        NULL,
        &output_parameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        NULL,
        NULL,
        NULL

    );

    pa_error = Pa_StartStream(*stream);
}

void init_sdl(SDL_Renderer** renderer, SDL_Window** window, int screen_width,
    int screen_height) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("INIT ERROR\n");
    }

    *window = SDL_CreateWindow("Synth", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL);

    if (*window == NULL) {
        printf("WINDOW ERROR\n");
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (*renderer == NULL) {
        printf("RENDERER ERROR\n");
    }
}


void init_divisions(Divisions* divisions, int screen_width, int screen_height) {
    divisions->partials = (SDL_Rect){ screen_width / 2, 0, screen_width / 2, screen_height / 2 };
    divisions->textboxes = (SDL_Rect){ 3 * (screen_width / 4), screen_height / 2, screen_width / 4, screen_height / 2 };
    divisions->wavetables = (SDL_Rect){ screen_width / 4, screen_height / 2, 3 * (screen_width / 4), screen_height / 2 };
}
/*
* @brief set UI Boxes coordiantes/dimensions based on fractional values
* @brief of given screen width/screen height
*/
void init_boxes(Boxes* boxes, int screen_width, int screen_height) {

    // set additive wave border
    boxes->additive = (SDL_Rect){ 0,0, screen_width / 2, screen_height / 2 };

    // set partial borders
    boxes->partials[0] = (SDL_Rect){ screen_width / 2, 0, screen_width / 2, screen_height / 6 };
    boxes->partials[1] = (SDL_Rect){ screen_width / 2,screen_height / 6, screen_width / 2, screen_height / 6 };
    boxes->partials[2] = (SDL_Rect){ screen_width / 2,screen_height / 3, screen_width / 2, screen_height / 6 };

    // set control border
    boxes->controls = (SDL_Rect){ 0, screen_height / 2, screen_width / 4, screen_height / 2 };

    // set wavetable borders
    boxes->wavetables[0] = (SDL_Rect){ screen_width / 4, screen_height / 2, screen_width / 4, screen_height / 4 };
    boxes->wavetables[1] = (SDL_Rect){ screen_width / 2, screen_height / 2, screen_width / 4, screen_height / 4 };
    boxes->wavetables[2] = (SDL_Rect){ screen_width / 4, 3 * (screen_height / 4), screen_width / 4, screen_height / 4 };
    boxes->wavetables[3] = (SDL_Rect){ screen_width / 2, 3 * (screen_height / 4), screen_width / 4, screen_height / 4 };

    // set textbox borders
    boxes->textboxes[0] = (SDL_Rect){ 3 * (screen_width / 4), screen_height / 2, screen_width / 4, screen_height / 6 };
    boxes->textboxes[1] = (SDL_Rect){ 3 * (screen_width / 4), 2 * (screen_height / 3), screen_width / 4, screen_height / 6 };
    boxes->textboxes[2] = (SDL_Rect){ 3 * (screen_width / 4), 5 * (screen_height / 6), screen_width / 4, screen_height / 6 };
}


void init_partials(Wave* partials) {
    for (int i = 0; i < PARTIAL_COUNT; i++) {
        partials[i].type = 0;
        partials[i].frequency = 0.0;
        partials[i].visual_phase = 0.0;
        partials[i].table_increment = 0.0;
        partials[i].table_phase = 0.0;
    }
}

void init_lfos(Wave* lfos) {
    for (int i = 0; i < LFO_COUNT; i++) {
        lfos[i].type = 0;
        lfos[i].frequency = 0.0;
        lfos[i].visual_phase = 0.0;
        lfos[i].table_increment = 0.0;
        lfos[i].table_phase = 0.0;
    }
}

void init_state(State* state) {
    state->active_partials = 0;
    state->selected_partial = 0;
    state->selected_textbox = -1;
    state->selected_wavetable = 0;
    state->running = true;
}

void init_parameters(Parameters* parameters) {
    parameters->volume = 0.0;
    parameters->tremolo = 0.0;
    parameters->wobble = 0.0;
    parameters->additive_zoom = 1.0;

    for (int i = 0; i < PARTIAL_COUNT; i++) {
        parameters->partial_zoom[i] = 1.0;
    }
}

void init_text(Text* text, Boxes* boxes) {
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    text->font = TTF_OpenFont("arial.ttf", 30);
    text->color = (SDL_Color){ 255,255,255 };

    for (int i = 0; i < TEXTBOX_COUNT; i++) {
        strcpy(text->text[i], "0.0000");
        text->textboxes[i] = (SDL_Rect){ boxes->textboxes[i].x + (boxes->textboxes[i].w) * 1.0 / 6.0,
            boxes->textboxes[i].y + (boxes->textboxes->h * (1.0 / 4.0)), 0,0 };

    }
}


void init_controls(Controls* controls, Boxes* boxes) {
    for (int i = 0; i < CONTROL_COUNT; i++) {
        // start of track
        controls->tracks[i][0].x = ((i + 1) / 4.0) * boxes->controls.w;
        controls->tracks[i][0].y = (7.0 / 12.0) * (boxes->controls.y + boxes->controls.h);
        // end of track
        controls->tracks[i][1].x = ((i + 1) / 4.0) * boxes->controls.w;
        controls->tracks[i][1].y = (11.0 / 12.0) * (boxes->controls.y + boxes->controls.h);
        // knob
        controls->knobs[i] = (SDL_Rect){ (((i + 1) / 4.0) * boxes->controls.w) - 5,
            (11.0 / 12.0) * (boxes->controls.y + boxes->controls.h),
             KNOB_WIDTH, KNOB_HEIGHT
        };

    }
}

void init_wavetables(Wavetables* wavetables) {
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
        wavetables->sine[i] = (float)sin((double)i / (double)WAVETABLE_SIZE * M_PI * 2.0);
        wavetables->square[i] = (float)square_wave((double)i / (double)WAVETABLE_SIZE * M_PI * 2.0);
        wavetables->saw[i] = (float)saw_wave((double)i / (double)WAVETABLE_SIZE * M_PI * 2.0, 2 * M_PI);
        wavetables->triangle[i] = (float)triangle_wave((double)i / (double)WAVETABLE_SIZE * M_PI * 2.0, 1.0, 2 * M_PI);
    }
}
