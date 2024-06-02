#include "synthesis.h"


void update_table_phase(Wave* wave) {
    wave->table_phase += wave->table_increment;     // apply phase increment
    if (wave->table_phase >= WAVETABLE_SIZE) {
        wave->table_phase -= WAVETABLE_SIZE;     // reset after full cycle
    }
}

void lfo_wobble(Wave* partial, Wave* wobble, Wavetables* wavetables) {
    switch (wobble->type) {
    case SINE:
        partial->table_phase += (LFO_AMP * wavetables->sine[(int)floor(wobble->table_phase)]);
        break;
    case SQUARE:
        partial->table_phase += (LFO_AMP * wavetables->square[(int)floor(wobble->table_phase)]);
        break;
    case SAW:
        partial->table_phase += (LFO_AMP * wavetables->saw[(int)floor(wobble->table_phase)]);
        break;
    case TRIANGLE:
        partial->table_phase += (LFO_AMP * wavetables->triangle[(int)floor(wobble->table_phase)]);
        break;

    }
}

double lfo_tremolo(Wave* tremolo, Wavetables* wavetables, double volume) {
    double modulated = 0.0;
    switch (tremolo->type) {
    case SINE:
        modulated = volume * wavetables->sine[(int)floor(tremolo->table_phase)];
        break;
    case SQUARE:
        modulated = volume * wavetables->square[(int)floor(tremolo->table_phase)];
        break;
    case SAW:
        modulated = volume * wavetables->saw[(int)floor(tremolo->table_phase)];
        break;
    case TRIANGLE:
        modulated = volume * wavetables->triangle[(int)floor(tremolo->table_phase)];
        break;

    }
    return modulated;
}

void synthesize_audio(PaStream* stream, State* state, Wave* partials,
    Wave* lfos, Parameters* parameters, Wavetables* wavetables) {

    float buffer[FRAMES_PER_BUFFER][2];
    float wave_value = 0.0;

    while(state->running) {
        for (int i = 0; i < SAMPLE_RATE / FRAMES_PER_BUFFER; i++) {         // loop over sample rate
            for (int j = 0; j < FRAMES_PER_BUFFER; j++) {                   // parse sample data
                for (int k = 0; k < state->active_partials; k++) {
                    if (partials[k].frequency > 0) {                        // prevents crash for lfo applied on 0 frequency
                        switch (partials[k].type) {
                        case SINE:
                            wave_value += wavetables->sine[(int)floor(partials[k].table_phase)];
                            break;
                        case SQUARE:
                            wave_value += wavetables->square[(int)floor(partials[k].table_phase)];
                            wave_value *= 0.10;
                            break;
                        case SAW:
                            wave_value += wavetables->saw[(int)floor(partials[k].table_phase)];
                            wave_value *= 0.10;
                            break;
                        case TRIANGLE:
                            wave_value += wavetables->triangle[(int)floor(partials[k].table_phase)];
                            break;
                        }
                        // modulate frequency with lfo wobble
                        // TO-DO, fix clipping with wobble under freq of 150.00
                        // for now, simply disable application for frequencies under 150hz
                        if (lfos[WOBBLE].frequency > 0 && partials[k].frequency >= 150.00) { 
                            lfo_wobble(&partials[k], &lfos[WOBBLE], wavetables);
                        }
                    }

                    update_table_phase(&partials[k]);
                }

                // update wobble phase
                if (lfos[WOBBLE].frequency > 0) {
                    update_table_phase(&lfos[WOBBLE]);
                }

                // modulate volume with lfo tremolo and update phase
                if (lfos[TREMOLO].frequency > 0) {
                    wave_value *= lfo_tremolo(&lfos[TREMOLO], wavetables, parameters->volume);
                    update_table_phase(&lfos[TREMOLO]);
                }
                else {
                    wave_value *= parameters->volume;
                }


                // divide by number of partials to ensure we don't exceed clip max
                if (state->active_partials > 0) {
                    wave_value /= state->active_partials;
                }

                wave_value *= 0.10;
                // write wave value to stereo output
                buffer[j][0] = wave_value;
                buffer[j][1] = wave_value;
                // reset wave accumulator
                wave_value = 0.0;
            }
            // write buffer to output stream
            Pa_WriteStream(stream, buffer, FRAMES_PER_BUFFER);
        }

    }
}
