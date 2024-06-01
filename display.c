#include "display.h"


void clear_screen(SDL_Renderer* renderer) {
    // set background to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/*
* @brief draw component boxes to screen
*/
void draw_boxes(SDL_Renderer* renderer, Boxes* boxes) {

    // draw additive box
    SDL_RenderDrawRect(renderer, &boxes->additive);

    // draw partial boxes
    for (int i = 0; i < PARTIAL_COUNT; i++) {
        SDL_RenderDrawRect(renderer, &boxes->partials[i]);
    }
    // draw control box
    SDL_RenderDrawRect(renderer, &boxes->controls);

    // draw wavetable boxes
    for (int i = 0; i < WAVETABLE_COUNT; i++) {
        SDL_RenderDrawRect(renderer, &boxes->wavetables[i]);
    }

    // draw textboxes boxes
    for (int i = 0; i < TEXTBOX_COUNT; i++) {
        SDL_RenderDrawRect(renderer, &boxes->textboxes[i]);
    }

}

void update_visual_phases(Wave* partials) {
    for (int i = 0; i < PARTIAL_COUNT; i++) {
        partials[i].visual_phase += 0.10;
        if (partials[i].visual_phase >= 2 * M_PI) {
            partials[i].visual_phase = 0.0;
        }
    }
}

double position_wave_x(double x, SDL_Rect* box) {
    return x + box->x;
}
double position_wave_y(double y, SDL_Rect* box) {
    return (y * WAVE_HEIGHT_SCALE) + (box->y + (box->h / 2.0));
}

void draw_partials(SDL_Renderer* renderer, Boxes* boxes, Wave* partials,
    State* state, Parameters* parameters) {

    // screen x,y
    double x = 0.0;
    double y = 0.0;

    // store previous y values
    double prev[PARTIAL_COUNT] = { 0,0,0 };

    // iterate over span of partial box width
    for (double i = 0.0; i <= boxes->partials->w; i += PARTIAL_DRAW_INCREMENT) {
        for (int j = 0; j < state->active_partials; j++) {
            // select wave type to draw to screen
            if (partials[j].frequency >= 1.0) {
                switch (partials[j].type) {
                case SINE:
                    y = sin(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->partial_zoom[j]) + partials[j].visual_phase);
                    break;
                case SQUARE:
                    y = square_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->partial_zoom[j]) + partials[j].visual_phase);
                    break;
                case SAW:
                    y = saw_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->partial_zoom[j]) + partials[j].visual_phase, 2 * M_PI);
                    break;
                case TRIANGLE:
                    y = triangle_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->partial_zoom[j]) + partials[j].visual_phase, 1.0, 2 * M_PI);
                    break;
                }
            }

            // position x,y values within region
            x = position_wave_x(i, &boxes->partials[j]);
            y = position_wave_y(y, &boxes->partials[j]);

            // if jump discontinuity occurs, draw vertical line
            if (jump_discontinuity(prev[j], y)) {
                SDL_RenderDrawLine(renderer, x, prev[j], x, y);
            }
            else {
                SDL_RenderDrawPoint(renderer, x, y);

            }
            // store previous y value
            prev[j] = y;
            // reset y to account for 0 freq
            y = 0.0;
        }
    }
}


void draw_additive(SDL_Renderer* renderer, Boxes* boxes, Wave* partials,
    State* state, Parameters* parameters) {
    // point for x,y value
    double x = 0.0;
    double y = 0.0;
    double prev = 0.0;

    // iterate over span of additive box width
    for (double i = 0.0; i <= boxes->additive.w; i += ADDITIVE_DRAW_INCREMENT) {
        for (int j = 0; j < state->active_partials; j++) {
            if (partials[j].frequency >= 1.0) {
                // select wave type to draw to screen, cumlative value
                switch (partials[j].type) {
                case SINE:
                    y += sin(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->additive_zoom) + partials[j].visual_phase);
                    break;
                case SQUARE:
                    y += square_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->additive_zoom) + partials[j].visual_phase);
                    break;
                case SAW:
                    y += saw_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->additive_zoom) + partials[j].visual_phase, 2 * M_PI);
                    break;
                case TRIANGLE:
                    y += triangle_wave(2 * M_PI * partials[j].frequency *
                        i / (boxes->partials->w * parameters->additive_zoom) + partials[j].visual_phase, 1.0, 2 * M_PI);
                    break;
                }
            }
        }

        // scale based on volume 
        y *= parameters->volume;


        // position x,y values within region
        x = position_wave_x(i, &boxes->additive);
        y = position_wave_y(y, &boxes->additive);



        // if jump discontinuity occurs, draw vertical line
        if (jump_discontinuity(prev, y)) {

            SDL_RenderDrawLine(renderer, x, prev, x, y);
        }
        else {
            SDL_RenderDrawPoint(renderer, x, y);

        }
        // store previous y value
        prev = y;
        // reset y to prevent further accumulation
        y = 0.0;
    }
}


void draw_wavetables(SDL_Renderer* renderer, Boxes* boxes) {
    double x = 0.0;
    double y = 0.0;
    double prev[WAVETABLE_COUNT] = { 0,0,0,0 };
    for (double i = 0.0; i <= boxes->wavetables->w; i += WAVETABLE_DRAW_INCREMENT) {
        for (int j = 0; j < WAVETABLE_COUNT; j++) {
            switch (j) {
            case SINE:
                y = sin(2 * M_PI * i / boxes->wavetables->w);
                break;
            case SQUARE:
                y = square_wave(2 * M_PI * i / boxes->wavetables->w);
                break;
            case SAW:
                y = saw_wave(2 * M_PI * i / boxes->wavetables->w, 2 * M_PI);
                break;
            case TRIANGLE:
                y = triangle_wave(2 * M_PI * i / boxes->wavetables->w, 1.0, 2 * M_PI);
                break;
            }

            // position x,y values within region
            x = position_wave_x(i, &boxes->wavetables[j]);
            y = position_wave_y(y, &boxes->wavetables[j]);

            // if jump discontinuity occurs, draw vertical line
            if (jump_discontinuity(prev[j], y)) {
                SDL_RenderDrawLine(renderer, x, prev[j], x, y);
            }
            else {
                SDL_RenderDrawPoint(renderer, x, y);

            }
            // store previous y value
            prev[j] = y;

        }
    }
}


void draw_textboxes(SDL_Renderer* renderer, Boxes* boxes, Text* text) {


    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;


    for (int i = 0; i < TEXTBOX_COUNT; i++) {
        // size text and create texture
        TTF_SizeText(text->font, text->text[i], &text->textboxes[i].w, &text->textboxes[i].h);

        surface = TTF_RenderText_Solid(text->font, text->text[i], text->color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        // draw textbox to screen
        SDL_RenderCopy(renderer, texture, NULL, &text->textboxes[i]);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

}

void draw_controls(SDL_Renderer* renderer, Boxes* boxes, Controls* controls,
    Parameters* parameters) {
    for (int i = 0; i < CONTROL_COUNT; i++) {
        // draw track ine
        SDL_RenderDrawLines(renderer, controls->tracks[i], 2);
        // draw knob
        SDL_RenderFillRect(renderer, &controls->knobs[i]);

    }
}
