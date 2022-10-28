#ifndef SYNTH_MONO_H
#define SYNTH_MONO_H

namespace Synth
{
  const unsigned int MONOSYNTH_MODULATION_DOWNSAMPLE=20;
  
  enum MonoSynthFloatParameters : int
  {
    GENERAL_VOLUME =0,
    
    OSC1_MIX, //1
    OSC1_TUNE, //2
    OSC1_FREQUENCY_MODULATION_LFO, //3
    OSC1_FREQUENCY_MODULATION_ENVELOPPE, //4

    OSC2_MIX, //5
    OSC2_TUNE, //6
    OSC2_FREQUENCY_MODULATION_LFO, //7
    OSC2_FREQUENCY_MODULATION_ENVELOPPE, //8

    OSC3_MIX, //9
    OSC3_TUNE, //10
    OSC3_FREQUENCY_MODULATION_LFO, //11
    OSC3_FREQUENCY_MODULATION_ENVELOPPE, //12

    VOLUME_ENVELOPPE_ATTACK, //13
    VOLUME_ENVELOPPE_DECAY, //14
    VOLUME_ENVELOPPE_SUSTAIN, //15
    VOLUME_ENVELOPPE_RELEASE, //16

    FILTER_CUTOFF, //17
    FILTER_CUTOFF_SLIDER, //18
    FILTER_CUTOFF_MODULATION_LFO, //19
    FILTER_CUTOFF_MODULATION_ENVELOPPE, //20
    FILTER_CUTOFF_MODULATION_MODWHEEL, //21
    FILTER_CUTOFF_MODULATION_MODWHEEL_LFO, //22
    FILTER_Q, //23

    MOD_ENVELOPPE_ATTACK, //24
    MOD_ENVELOPPE_DECAY, //25
    MOD_ENVELOPPE_SUSTAIN, //26
    MOD_ENVELOPPE_RELEASE, //27

    LFO_RATE, //28
    LFO_RATE_SLIDER, //29
    LFO_RATE_MODWHEEL, //30
    
    VIBRATO_RATE, //31
    VIBRATO_RATE_SLIDER, //32
    VIBRATO_RATE_MODWHEEL, //33
    VIBRATO_AMPLITUDE, //34
    VIBRATO_AMPLITUDE_SLIDER, //35
    VIBRATO_AMPLITUDE_MODWHEEL, //36
    
    GLIDE_TIME, //37

    PITCHBEND_RANGE, //38

    SLIDER_VALUE, //39

    OVERDRIVE_GAIN, //40

    OSC1_FREQUENCY_MULTIPLICATOR, //41
    OSC2_FREQUENCY_MULTIPLICATOR, //42
    OSC3_FREQUENCY_MULTIPLICATOR //43
  };

  enum MonoSynthIntParameters : int
  {
    OSC1_ON =0,
    OSC1_SEMI, //1
    OSC1_WAVEFORM, //2
    
    OSC2_ON, //3
    OSC2_SEMI, //4
    OSC2_WAVEFORM, //5
    
    OSC3_ON, //6
    OSC3_SEMI, //7
    OSC3_WAVEFORM, //8

    FILTER_ON, //9
    GLIDE_ON, //10
    LEGATO_ON, //11
    FILTER_FOLLOW_ON, //12
    OVERDRIVE_ON //13
  };

  enum WaveTableWaveform : int
  {
    WAVEFORM_SINE=0,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SAW,
    WAVEFORM_SQUARE
  };
  
  class WaveTableMonosynth : public Parameterizable, public Voice, public Mutexable
  {
    public:
      WaveTableMonosynth();
      float computeNext();
      
      void setIntParameter(unsigned int parameter,int value);
      void setFloatParameter(unsigned int parameter,float value);
      int getIntParameter(unsigned int parameter);
      float getFloatParameter(unsigned int parameter);

      virtual void noteOn(int channel,int noteCode,int velocity);
      virtual void noteOff(int channel,int noteCode,int velocity);
      virtual void controlChange(int channel,int ccCode,int value);
      virtual void pitchBend(int channel,int value);

      void init();
    protected:
      Adsr volumeEnveloppe;
      Adsr modEnveloppe;
      Ramp glideRamp;
      SecondOrderLowPassFilter filter;
      
      WaveTableOscillator osc[3];
      WaveTableOscillator vibratoLfo;
      WaveTableOscillator modLfo;

      SaturatorTable overdrive;

      boolean pushed;
      int noteCode;
      float baseFrequency;
      unsigned int downsample;
      float modWheel;
      float pitchBendValue;
  };
}

#endif
