#ifndef SYNTH_OSC_H
#define SYNTH_OSC_H

namespace Synth
{
  // Oscillator, with phaseMin<=phase<phaseMax
  class Oscillator : public SampleRateDependant
  {
    public:
      Oscillator(float sampleRate);
      float computeNext();
      virtual float compute()=0;
      float getValue();
      float getFrequency();
      void setFrequency(float frequency);
    protected:
      float phaseMin,phaseMax;
      float phase;
      float phaseIncrement;
      float frequency;
      float value;
  };

  // Oscillator with real time computation, with -PI<=phase<PI
  class SimpleOscillator : public Oscillator
  {
    public:
      SimpleOscillator();
      SimpleOscillator(float sampleRate,Function &function);
      void setFunction(Function &function);
      virtual float compute();

      void copy(SimpleOscillator &osc);
    protected:
      Function *function;
  };

  // Oscillator with real time computation, with 0<=phase<tableSize
  class WaveTableOscillator : public Oscillator
  {
    public:
      WaveTableOscillator();
      WaveTableOscillator(float sampleRate,FunctionTable &function);
      virtual float compute();
      void setWaveTable(FunctionTable &function);

      void copy(WaveTableOscillator &osc);
    protected:
      FunctionTable *function;
  };

  class PitchPowerTable : public FunctionTable
  {
    public:
      PitchPowerTable(size_t size);
    protected:
      float function(float x);
  };

  extern PitchPowerTable pitchPowerTable;
}

#endif
