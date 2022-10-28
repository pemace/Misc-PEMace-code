#include <Arduino.h>
#include "math.h"
#include "synth.h"

#include "synth.h"

namespace Synth
{
  Modulator::Modulator(float sampleRate) : SampleRateDependant(sampleRate)
  {
    this->sampleRate=sampleRate;
  }
  
  float Modulator::getValue()
  {
    return value;
  }

  Ramp::Ramp() : Ramp(SAMPLE_RATE_FLOAT)
  {
  }
  
  Ramp::Ramp(float sampleRate) : Modulator(sampleRate)
  {
    launched=0;
    value=0.f;
    set(1.f,0.f,1.f);
  }
  
  float Ramp::computeNext()
  {
    float r=value;

    if(launched)
    {
      value+=increment;
      
      if(increment>=0)
      {
        if(value>rampEnd-DELTA)
          value=rampEnd;
      } else
      {
        if(value<rampEnd+DELTA)
          value=rampEnd;
      }
    }
  
    return r;
  }
  
  
  void Ramp::set(float rampTime,float rampBegin,float rampEnd)
  {
    launched=0;
    value=rampBegin;
    
    this->rampBegin=rampBegin;
    this->rampEnd=rampEnd;
    this->rampTime=rampTime;
    
    if(abs(rampEnd-rampBegin)<DELTA)
      this->increment=0.f;
    else
      this->increment=(rampEnd-rampBegin)/rampTime/sampleRate;
  }

  void Ramp::noteOn()
  {
    launched=1;
    value=rampBegin;
  }
  
  void Ramp::noteOff()
  {
    launched=0;
    value=0.;
  }
  
  void Ramp::copy(const Ramp &ramp)
  {
    set(ramp.rampTime,ramp.rampBegin,ramp.rampEnd);
  }

  Adsr::Adsr() : Adsr(SAMPLE_RATE_FLOAT)
  {
    
  }
  
  Adsr::Adsr(float sampleRate) : Modulator(sampleRate)
  {
    set(0.01f,0.f,1.f,0.01f);
  }

  void Adsr::set(float attackTime,float decayTime,float sustainValue,float releaseTime)
  {
    state=SYNTH_ADSR_IDLE;
    value=0.f;
    
    this->attackTime=attackTime;
    this->decayTime=decayTime;
    this->sustainValue=sustainValue;
    this->releaseTime=releaseTime;

    if(attackTime<DELTA) attackIncrement=1.f;
    else attackIncrement=1.f/attackTime/sampleRate;

    if(decayTime<DELTA) decayIncrement=1.f-sustainValue;
    else decayIncrement=(sustainValue-1.f)/decayTime/sampleRate;

    if(releaseTime<DELTA) releaseIncrement=-1.f;
    else releaseIncrement=-1.f/decayTime/sampleRate;
  }

  float Adsr::computeNext()
  {
    float r=value;
    switch(state)
    {
      case SYNTH_ADSR_ATTACK:
        value+=attackIncrement;
        if(value>=1.f-DELTA)
        {
          value=1.f;
          state=SYNTH_ADSR_DECAY;
        }
        break;
      case SYNTH_ADSR_DECAY:
        value+=decayIncrement;
        if(value<DELTA)
        {
          value=0.f;
          state=SYNTH_ADSR_IDLE;
        } else if(value<sustainValue+DELTA)
        {
          value=sustainValue;
          state=SYNTH_ADSR_SUSTAIN;
        }
        break;
      case SYNTH_ADSR_RELEASE:
        value+=releaseIncrement;
        if(value<DELTA)
        {
          value=0.f;
          state=SYNTH_ADSR_IDLE;
        }
        break;
    }

    return r;
  }

  void Adsr::noteOn()
  {
    state=SYNTH_ADSR_ATTACK;
    value=0.f;
  }
  
  void Adsr::noteOff()
  {
    state=SYNTH_ADSR_RELEASE;
  }

  void Adsr::copy(const Adsr &adsr)
  {
    set(adsr.attackTime,adsr.decayTime,adsr.sustainValue,adsr.releaseTime);
  }


  Lfo::Lfo() : Lfo(SAMPLE_RATE_FLOAT)
  {
    
  }
  
  Lfo::Lfo(float sampleRate) : Modulator(sampleRate) , osc(sampleRate,sineTable)
  {
    center=0.f;
    amplitude=1.f;
    osc.setFrequency(1.f);
  }

  float Lfo::computeNext()
  {
    return value=center+amplitude*osc.computeNext();
  }

  void Lfo::setFrequency(float frequency)
  {
    osc.setFrequency(frequency);
  }
  
  void Lfo::setCenter(float center)
  {
    this->center=center;
  }
  void Lfo::setAmplitude(float amplitude)
  {
    this->amplitude=amplitude;
  }

  SaturatorTable::SaturatorTable(size_t size) : FunctionTable(size)
  {
    gain=1.0;
    init(0.f,1.f);
  }

  void SaturatorTable::setGain(float gain)
  {
    if(gain>0.1f)
      this->gain=gain;
    else
      gain=0.1f;
    init(0.f,1.f);
  }

  float SaturatorTable::function(float x)
  {
    if(x>=0)
      return (1-exp(-gain*x))/(1-exp(-gain));
    else
      return (1-exp(gain*x))/(1-exp(-gain));
  }
}
