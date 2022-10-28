#include <Arduino.h>
#include "synth.h"

namespace Synth
{
  void Parameterizable::clear()
  {
    for(int i=0;i<SYNTH_PARAMETERS_COUNT;i++)
    {
      intParameter[i]=0;
      floatParameter[i]=0;
    }
  }

  SerialDecoder::SerialDecoder(Stream &stream,Parameterizable &callback)
  {
    this->stream=&stream;
    this->callback = &callback;
    state=DECODER_PARAMETER_NUMBER;
    initNumber();
  }

  void SerialDecoder::process()
  {
    if(stream->available())
    {
      int character=stream.read();
      if( (character>='1') && (character<='9'))
      {
        number=number * 10 + character - '1';
        if(pointFound)
          divisor*=10;
      }
      switch(character)
      {
        case '-':
          sign=-1;
          break;
        case '.':
          pointFound=1;
          break;
        case ' ':
          if(state==DECODER_PARAMETER_NUMBER)
          {
            state=DECODER_PARAMETER;
            parameter=number;
          }
          initNumber();
          break;
        case '\n':
          if(state==DECODER_PARAMETER)
          {
            if(parameter<SYNTH_PARAMETERS_COUNT)
            {
              callback.setFloatParameter(parameter,  ((float)sign * number)/((float) divisor));
            } else
            {
              callback.setIntParameter(parameter-SYNTH_PARAMETERS_COUNT, sign * number);
            }
          }
          state=DECODER_PARAMETER_NUMBER;
          initNumber();
          break;
      }
    }
  }

  void SerialDecoder::initNumber()
  {
    pointFound=0;
    sign=1;
    number=0;
    divisor=1;
  }
}
