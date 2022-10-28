#include <Arduino.h>
#include "synth_midi.h"

#define CODE_MIDI_PITCHBEND 0xE0
#define CODE_MIDI_CC 0xB0
#define CODE_MIDI_NOTEOFF 0x80
#define CODE_MIDI_NOTEON 0x90

#define MASQUE_MIDI_CANAL 0xF
#define MASQUE_MIDI_COMMANDE 0xF0
#define MASQUE_MIDI_EST_COMMANDE 0x80

namespace Synth
{
  MidiDecoder::MidiDecoder(Stream &stream,MidiResponsive &callback)
  {
    this->stream=&stream;
    this->callback=&callback;
    cur=0;
    command=-1;
  }
  
  void MidiDecoder::process()
  {
    if(stream->available()>0)
    {
      int input=stream->read();
      if((input&MASQUE_MIDI_EST_COMMANDE)!=0)
      {
        command=input;
        cur=0;
      }
      else
      {
        data[cur]=input;

        cur=1-cur;

        if(cur==0)
        {
          switch(command&MASQUE_MIDI_COMMANDE)
          {
            case CODE_MIDI_NOTEON:
              if(callback!=NULL)
              {
                callback->noteOn(command&MASQUE_MIDI_CANAL,data[0],data[1]);
              }
              break;
            case CODE_MIDI_NOTEOFF:
              if(callback!=NULL)
              {
                callback->noteOff(command&MASQUE_MIDI_CANAL,data[0],data[1]);
              }
              break;
            case CODE_MIDI_CC:
              if(callback!=NULL)
              {
                callback->controlChange(command&MASQUE_MIDI_CANAL,data[0],data[1]);
              }
              break;
            case CODE_MIDI_PITCHBEND:
              if(callback!=NULL)
              {
                callback->pitchBend(command&MASQUE_MIDI_CANAL,(data[0]<<7)|data[1]);
              }
              break;
          }
        }
      }
    }
  }
}
