#ifndef SYNTH_MIDI_H
#define SYNTH_MIDI_H

namespace Synth
{
  class MidiResponsive
  {
    public:
      virtual void noteOn(int canal,int code,int velocite)=0;
      virtual void noteOff(int canal,int code,int velocite)=0;
      virtual void controlChange(int canal,int code,int valeur)=0;
      virtual void pitchBend(int canal,int valeur)=0;
  };
  
  class MidiDecoder
  {
    public:
      MidiDecoder(Stream &stream,MidiResponsive &callback);
      void process();
    private:
      Stream *stream;
      MidiResponsive *callback;
      int command;
      int data[2];
      unsigned int cur;
  };
}

#endif
