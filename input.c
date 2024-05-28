#include "input.h"


void select_wavetable(SDL_Point* position, State* state, Boxes* boxes) {
    if (SDL_PointInRect(position, &boxes->wavetables[SINE])) {
        state->selected_wavetable = SINE;
        printf("SINE\n");
    }
    else if (SDL_PointInRect(position, &boxes->wavetables[SQUARE])) {
        state->selected_wavetable = SQUARE;
        printf("SQUARE\n");
    }
    else if (SDL_PointInRect(position, &boxes->wavetables[SAW])) {
        state->selected_wavetable = SAW;
        printf("SAW\n");
    }
    else if (SDL_PointInRect(position, &boxes->wavetables[TRIANGLE])) {
        state->selected_wavetable = TRIANGLE;
        printf("TRIANGLE\n");
    }
}

void select_partial(SDL_Point* position, State* state, Boxes* boxes) {
    if (SDL_PointInRect(position, &boxes->partials[0])) {
        state->selected_partial = 0;
        printf("FIRST PARTIAL\n");
    }
    else if (SDL_PointInRect(position, &boxes->partials[1])) {
        state->selected_partial = 1;
        printf("SECOND PARTIAL\n");

    }
    else if (SDL_PointInRect(position, &boxes->partials[2])) {
        state->selected_partial = 2;
        printf("THIRD PARTIAL\n");

    }
}

void select_textbox(SDL_Point* position, State* state, Boxes* boxes) {
    if (SDL_PointInRect(position, &boxes->textboxes[0])) {
        state->selected_textbox = 0;
        printf("FIRST TEXTBOX\n");
    }
    else if (SDL_PointInRect(position, &boxes->textboxes[1])) {
        state->selected_textbox = 1;
        printf("SECOND TEXTBOX\n");

    }
    else if (SDL_PointInRect(position, &boxes->textboxes[2])) {
        state->selected_textbox = 2;
        printf("THIRD TEXTBOX\n");

    }
}

void check_mouse_inputs(SDL_Event* event, State* state, Wave* partials,
    Wave* lfos, Divisions* divisions, Boxes* boxes) {
    SDL_Point position;
    position.x = event->motion.x;
    position.y = event->motion.y;
    switch (event->button.button) {

    case SDL_BUTTON_LEFT:
        if (SDL_PointInRect(&position, &divisions->textboxes)) {
            select_textbox(&position, state, boxes);
        }
        else if ((SDL_PointInRect(&position, &divisions->wavetables))) {
            select_wavetable(&position, state, boxes);
            partials[state->selected_partial].type = state->selected_wavetable;
        }
        else if (SDL_PointInRect(&position, &divisions->partials)) {
            select_partial(&position, state, boxes);
        }
        break;

    case SDL_BUTTON_RIGHT:
        if (SDL_PointInRect(&position, &divisions->textboxes)) {
            // deselect textbox
            state->selected_textbox = -1;
        }
        else if (SDL_PointInRect(&position, &divisions->wavetables)) {
            select_wavetable(&position, state, boxes);
            lfos[WOBBLE].type = state->selected_wavetable;
            lfos[TREMOLO].type = state->selected_wavetable;
        }

    }
}

void check_keyboard_inputs(SDL_Event* event, State* state, Text* text,
    Parameters* parameters, Wave* partials, Wave* lfos, Controls* controls) {

    int text_length = strlen(text->text[state->selected_textbox]);
    switch (event->key.keysym.sym) {

    case SDLK_BACKSPACE:
        if (state->selected_textbox != -1 && text_length > 0) {
            text->text[state->selected_textbox][text_length - 1] = '\0';
        }
        break;
    case SDLK_EQUALS:
        if (state->selected_textbox != -1 && text_length == MAX_TEXT_LENGTH) {
            double freq = atof(text->text[state->selected_textbox]);
            if (freq >= 20.00 && freq <= 20000) {
                partials[state->selected_textbox].frequency = freq;
                partials[state->selected_textbox].table_increment = \
                    table_increment(partials[state->selected_textbox].frequency);
            }
            else {
                printf("INVALID FREQUENCY (enter between [20,20,000])\n");
            }

        }
        break;

    case SDLK_UP:
        if (state->active_partials < PARTIAL_COUNT) {
            state->active_partials++;
            printf("WAVES %i\n", state->active_partials);
        }
        break;

    case SDLK_DOWN:
        if (state->active_partials > 0) {
            state->active_partials--;
            printf("WAVES %i\n", state->active_partials);
        }
        break;

    case SDLK_q:
        if (parameters->volume >= 0.10) {
            parameters->volume -= 0.10;
            controls->knobs[VOLUME_CONTROL].y += 16;
            printf("VOLUME %f \n", parameters->volume);
        }
        break;

    case SDLK_w:
        if (parameters->volume <= 0.90) {
            parameters->volume += 0.10;
            controls->knobs[VOLUME_CONTROL].y -= 16;
            printf("VOLUME %f \n", parameters->volume);
        }
        break;

    case SDLK_e:
        if (parameters->wobble > 0.0) {   
            controls->knobs[WOBBLE_CONTROL].y += 8;
            parameters->wobble -= 0.05;
            lfos[WOBBLE].frequency -= 1.0;
            lfos[WOBBLE].table_increment = table_increment(lfos[WOBBLE].frequency);
            printf("WOBBLE FREQ %f\n", lfos[WOBBLE].frequency);

        }
        break;

    case SDLK_r:
        if (parameters->wobble < 1.00) {
            controls->knobs[WOBBLE_CONTROL].y -= 8;
            parameters->wobble += 0.05;
            lfos[WOBBLE].frequency += 1.0;
            lfos[WOBBLE].table_increment = table_increment(lfos[WOBBLE].frequency);
            printf("WOBBLE FREQ %f\n", lfos[WOBBLE].frequency);
        }
        break;

    case SDLK_t:
        if (parameters->tremolo > 0.0) { 
            controls->knobs[TREMOLO_CONTROL].y += 8;
            parameters->tremolo -= 0.05;
            lfos[TREMOLO].frequency -= 1.0;
            lfos[TREMOLO].table_increment = table_increment(lfos[TREMOLO].frequency);
            printf("TREMOLO FREQ %f\n", lfos[TREMOLO].frequency);
            printf("%f \n", parameters->tremolo);
        }
        break;

    case SDLK_y:
        if (parameters->tremolo <= 1.00) {
            controls->knobs[TREMOLO_CONTROL].y -= 8;
            parameters->tremolo += 0.05;
            lfos[TREMOLO].frequency += 1.0;
            lfos[TREMOLO].table_increment = table_increment(lfos[TREMOLO].frequency);
            printf("TREMOLO FREQ %f\n", lfos[TREMOLO].frequency);
        }
        break;
    case SDLK_a:
        if (parameters->additive_zoom >= 2.0) {
            parameters->additive_zoom -= 1.0;
            printf("ADDITIVE ZOOM %f\n", parameters->additive_zoom);

        }
        break;
    case SDLK_s:
        if (parameters->additive_zoom < 10000) {
            parameters->additive_zoom += 1.0;
            printf("ADDITIVE ZOOM %f\n", parameters->additive_zoom);
        }
        break;
    case SDLK_d:
        if (parameters->partial_zoom[state->selected_partial] >= 2.0) {
            parameters->partial_zoom[state->selected_partial] -= 1.0;
            printf("PARTIAL %i ZOOM %f\n", state->selected_partial + 1,
                parameters->partial_zoom[state->selected_partial]);
        }
        break;

    case SDLK_f:
        if (parameters->partial_zoom[state->selected_partial] < 1000) {
            parameters->partial_zoom[state->selected_partial] += 1.0;
            printf("PARTIAL %i ZOOM %f\n", state->selected_partial + 1,
                parameters->partial_zoom[state->selected_partial]);
        }
        break;

    }
}

void check_text_input(SDL_Event* event, State* state, Text* text) {
    if (state->selected_textbox == -1) {
        return;
    }
    else if ((isdigit(event->text.text[0]) || event->text.text[0] == '.') &&
        (strlen(text->text[state->selected_textbox]) < MAX_TEXT_LENGTH)) {
        // copy entered text to textbox
        strcat(text->text[state->selected_textbox], event->text.text);
    }
}
