#ifndef SYNTH_SDEC_H
#define SYNTH_SDEC_H


namespace Synth
{
  const unsigned int SYNTH_PARAMETERS_COUNT=64;
  
  class Parameterizable
  {
    public:
      void clear();
      virtual void setIntParameter(unsigned int parameter,int value)=0;
      virtual void setFloatParameter(unsigned int parameter,float value)=0;
    protected:
      float floatParameter[SYNTH_PARAMETERS_COUNT];
      int intParameter[SYNTH_PARAMETERS_COUNT];
  };
  
  const int SERIALDECODER_BUFFER_SIZE = 64;

  enum SerialDecoderState
  {
    DECODER_IDLE =0;
    DECODER_PARAMETER_NUMBER,
    DECODER_PARAMETER
  };
  
  class SerialDecoder
  {
    public:
      SerialDecoder(Stream &stream,Parameterizable &callback);
      void process();
    private:
      void initNumber();
      Stream *stream;
      callback *callback;

      SerialDecoderState state;

      unsigned int parameter;

      boolean pointFound;
      int sign;
      int number;
      int divisor;
  };
}

#endif
