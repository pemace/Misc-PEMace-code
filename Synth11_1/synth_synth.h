#ifndef SYNTH_SYNTH_H
#define SYNTH_SYNTH_H

namespace Synth
{
  class Voice : public MidiResponsive
  {
    public:
      virtual float computeNext()=0;
      virtual void noteOn(int channel,int noteCode,int velocity)=0;
      virtual void noteOff(int channel,int noteCode,int velocity)=0;
      virtual void controlChange(int channel,int ccCode,int value)=0;
      virtual void pitchBend(int channel,int value)=0;
    private:
  };
}

#endif
