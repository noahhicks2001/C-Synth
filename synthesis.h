#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "config.h"
#include "structs.h"

void update_table_phase(Wave* wave);
void lfo_wobble(Wave* partial, Wave* wobble, Wavetables* wavetables);
double lfo_tremolo(Wave* tremolo, Wavetables* wavetables, double volume);
void synthesize_audio(PaStream* stream, State* state, Wave* partials,
    Wave* lfos, Parameters* parameters, Wavetables* wavetables);

#endif
