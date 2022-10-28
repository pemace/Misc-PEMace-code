#ifndef SYNTH_MOD_H
#define SYNTH_MOD_H

namespace Synth
{
  class Modulator : public SampleRateDependant
  {
    public:
      Modulator(float sampleRate);
      virtual float computeNext()=0;
      float getValue();
    protected:
      float value;
  };

  class Launchable
  {
    public:
      virtual void noteOn()=0;
      virtual void noteOff()=0;
  };

  class Ramp : public Modulator, public Launchable
  {
    public:
      Ramp();
      Ramp(float sampleRate);
      float computeNext();

      void set(float rampTime,float rampBegin,float rampEnd);

      void noteOn();
      void noteOff();
      
      void copy(const Ramp &ramp);
    protected:
      boolean launched;
      float rampBegin,rampEnd;
      float rampTime;
      float increment;
  };

  typedef enum
  {
    SYNTH_ADSR_IDLE =0,
    SYNTH_ADSR_ATTACK,
    SYNTH_ADSR_DECAY,
    SYNTH_ADSR_SUSTAIN,
    SYNTH_ADSR_RELEASE
  } AdsrState;
  
  class Adsr : public Modulator, public Launchable
  {
    public:
      Adsr();
      Adsr(float sampleRate);
      float computeNext();

      void set(float attackTime,float decayTime,float sustainValue,float releaseTime);

      void noteOn();
      void noteOff();

      void copy(const Adsr &adsr);
    protected:
      AdsrState state;
      float value;
      float attackTime,decayTime,sustainValue,releaseTime;
      float attackIncrement,decayIncrement,releaseIncrement;
  };

  class Lfo : public Modulator
  {
    public:
      Lfo();
      Lfo(float sampleRate);
      float computeNext();
      
      void setFrequency(float frequency);
      void setCenter(float center);
      void setAmplitude(float amplitude);
    private:
      float center,amplitude;
      WaveTableOscillator osc;
  };

  class SaturatorTable : public FunctionTable
  {
    public:
      SaturatorTable(size_t size);
      void setGain(float gain);
    protected:
      float function(float x);
      float gain;
  };
}
#endif
