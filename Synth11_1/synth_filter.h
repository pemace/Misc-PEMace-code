#ifndef SYNTH_FILTER_H
#define SYNTH_FILTER_H

namespace Synth
{
  const float MIN_FILTER_VALUE = 20.f;
  const float MAX_FILTER_VALUE = SAMPLE_RATE_FLOAT/2.f;

  
  
  class Filter : public SampleRateDependant
  {
    public:
      Filter(float sampleRate);
      virtual float computeNext(float signal)=0;
    protected:
  };
  
  class SecondOrderFilter : public Filter
  {
    public:
      SecondOrderFilter(float sampleRate,unsigned int oversample);
      virtual float computeNext(float signal);
      void set(float cInput,float cdInput_dt,float cd2Input_dt2,float cOutput,float cdOutput_dt,float cd2Output_dt2);
      void set(float cInput_0,float cInput_1,float cInput_2,float cOutput_1,float cOutput_2);
    protected:
      virtual float computeStep(float signal);
      unsigned int oversample;
      float previousSignal;
      
      float cInput[3];
      float cOutput[2];
      
      float input[2];
      float output[2];
  };

  class SecondOrderLowPassFilter : public SecondOrderFilter
  {
      public:
        SecondOrderLowPassFilter(float sampleRate,unsigned int oversample);
        void set(float frequency,float q);
        
        float getFrequency();
        float getQ();
      protected:
        float frequency;
        float q;
  };

  class LogFilter : public FunctionTable
  {
    public:
      LogFilter(size_t size);
    private:
      float function(float x);
  };

  extern LogFilter logFilter;

  class InvLogFilter : public FunctionTable
  {
    public:
      InvLogFilter(size_t size);
    private:
      float function(float x);
  };

  extern InvLogFilter invLogFilter;
}
#endif
