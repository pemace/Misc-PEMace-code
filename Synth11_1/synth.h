#ifndef SYNTH_H
#define SYNTH_H

namespace Synth
{
  const float A_TUNING = 440.f;
  const uint32_t SAMPLE_RATE = 44100;
  
  const float SAMPLE_RATE_FLOAT = (float) SAMPLE_RATE;
  
}

#include "synth_mutex.h"
#include "synth_midi.h"
#include "synth_sdec.h"
#include "synth_math.h"
#include "synth_osc.h"
#include "synth_mod.h"
#include "synth_filter.h"
#include "synth_codes.h"
#include "synth_synth.h"
#include "synth_mono.h"

#endif
