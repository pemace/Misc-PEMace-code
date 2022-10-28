#include <Arduino.h>
#include "math.h"
#include "synth.h"

#include "synth.h"

namespace Synth
{
  Oscillator::Oscillator(float sampleRate) : SampleRateDependant(sampleRate)
  {
    phaseMin=-PI_FLOAT;
    phaseMax=PI_FLOAT;
    phase=0.;
    value=0.;
    setFrequency(A_TUNING);
  }

  float Oscillator::computeNext()
  { 
    value=compute();

    phase+=phaseIncrement;
    if(phase>phaseMax)
      phase=phaseMin;

    return value;
  }

  float Oscillator::getValue()
  {
    return value;
  }

  float Oscillator::getFrequency()
  {
    return frequency;
  }

  void Oscillator::setFrequency(float frequency)
  {
    this->frequency=frequency;
    phaseIncrement=(phaseMax-phaseMin)/sampleRate*frequency;
  }

  SimpleOscillator::SimpleOscillator(float sampleRate,Function &function) : Oscillator(sampleRate)
  {
    this->function=&function;
    value=function.compute(0.f);
  }

  void SimpleOscillator::setFunction(Function &function)
  {
    this->function=&function;
  }

  float SimpleOscillator::compute()
  {
    return function->compute(phase);
  }

  void SimpleOscillator::copy(SimpleOscillator &osc)
  {
    setFunction(*osc.function);
  }

  WaveTableOscillator::WaveTableOscillator() : WaveTableOscillator(SAMPLE_RATE_FLOAT,sineTable)
  {
  }
  WaveTableOscillator::WaveTableOscillator(float sampleRate,FunctionTable &function) : Oscillator(sampleRate)
  {
    setWaveTable(function);
  }

  void WaveTableOscillator::setWaveTable(FunctionTable &function)
  {
    this->function=&function;
    phase=function.getSize()/2.;
    phaseMin=0.f;
    phaseMax=function.getSize();
    setFrequency(frequency);
  }

  float WaveTableOscillator::compute()
  {
    return function->index(phase);
  }

  void WaveTableOscillator::copy(WaveTableOscillator &osc)
  {
    setWaveTable(*osc.function);
  }
  
 
  PitchPowerTable::PitchPowerTable(size_t size) : FunctionTable(size)
  {
    init(-2.f,2.f);
  }

  float PitchPowerTable::function(float x)
  {
    return pow(2,x/12.f);
  }

  PitchPowerTable pitchPowerTable(128);
}
