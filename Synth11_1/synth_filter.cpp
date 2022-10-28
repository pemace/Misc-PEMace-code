#include <Arduino.h>
#include "math.h"
#include "synth.h"

namespace Synth
{
  Filter::Filter(float sampleRate) : SampleRateDependant(sampleRate)
  {
    
  }
  
  SecondOrderFilter::SecondOrderFilter(float sampleRate,unsigned int oversample) : Filter(sampleRate*oversample)
  {
    if(oversample>0)
      this->oversample=oversample;
    else
      this->oversample=1;
    
    for(int i=0;i<3;i++)
    {
      input[i]=0.f;
    }
    input[0]=input[1]=0.f;
    
    cOutput[0]=cOutput[1]=0.f;
    output[0]=output[1]=0.f;

    previousSignal=0.f;
  }

  float SecondOrderFilter::computeNext(float signal)
  {
    float inc=(signal-previousSignal)/oversample;
    float x=previousSignal+inc;

    if(oversample>1)
      for(int i=1;i<oversample;i++,x+=inc)
        computeStep(x);
      
    previousSignal=signal;
    return computeStep(signal);
  }

  float SecondOrderFilter::computeStep(float signal)
  {
    float filter=cInput[0]*signal + cInput[1] * input[0] + cInput[2] * input[1] + cOutput[0] * output[0] + cOutput[1] * output[1];

    input[1]=input[0];
    input[0]=signal;

    output[1]=output[0];
    output[0]=filter;
    
    return filter;
  }

  void SecondOrderFilter::set(float cInput_0,float cInput_1,float cInput_2,float cOutput_1,float cOutput_2)
  {
    cInput[0]=cInput_0;
    cInput[1]=cInput_1;
    cInput[2]=cInput_2;
    cOutput[0]=cOutput_1;
    cOutput[1]=cOutput_2;
  }

  void SecondOrderFilter::set(float cInput,float cdInput_dt,float cd2Input_dt2,float cOutput,float cdOutput_dt,float cd2Output_dt2)
  {
    float cdInput_dt_div=cdInput_dt*sampleRate;
    float cd2Input_dt2_div=cd2Input_dt2*sampleRate*sampleRate;

    float cdOutput_dt_div=cdOutput_dt*sampleRate;
    float cd2Output_dt2_div=cd2Output_dt2*sampleRate*sampleRate;

    float factor=1.f/(cOutput+cdOutput_dt_div+cd2Output_dt2_div);

    this->cInput[0]=factor*(cInput+cdInput_dt_div+cd2Input_dt2_div);
    this->cInput[1]=-factor*(cdInput_dt_div+2.f*cd2Input_dt2_div);
    this->cInput[2]=factor*cd2Input_dt2_div;

    this->cOutput[0]=factor*(cdOutput_dt_div+2.f*cd2Output_dt2_div);
    this->cOutput[1]=-factor*cd2Output_dt2_div;


    /*Serial.println(this->cInput[0],10);
    Serial.println(this->cInput[1],10);
    Serial.println(this->cInput[2],10);
    Serial.println(this->cOutput[0],10);
    Serial.println(this->cOutput[1],10);*/
    
  }

  SecondOrderLowPassFilter::SecondOrderLowPassFilter(float sampleRate,unsigned int oversample) : SecondOrderFilter(sampleRate,oversample)
  {
    set(MAX_FILTER_VALUE,0.5);
  }

  void SecondOrderLowPassFilter::set(float frequency,float q)
  {
    float inv_w0=1.f / ( DOUBLE_PI_FLOAT * frequency )*sampleRate;
    float inv_w0_2=inv_w0*inv_w0;
    float inv_qw0=inv_w0/q;

    float factor=1.f/(1.f+inv_qw0+inv_w0_2);
    
    this->cInput[0]=factor;
    this->cInput[1]=0.f;
    this->cInput[2]=0.f;

    this->cOutput[0]=factor*(2.f*inv_w0_2+inv_qw0);
    this->cOutput[1]=-factor*inv_w0_2;
    
    this->frequency=frequency;
    this->q=q;
  }

  float SecondOrderLowPassFilter::getFrequency()
  {
    return frequency;
  }

  float SecondOrderLowPassFilter::getQ()
  {
    return q;
  }

  LogFilter::LogFilter(size_t size) : FunctionTable(size)
  {
    init(0.f,1.f);
  }

  float LogFilter::function(float x)
  {
    return MIN_FILTER_VALUE * pow( MAX_FILTER_VALUE/MIN_FILTER_VALUE, x);
  }

  LogFilter logFilter(256);

  InvLogFilter::InvLogFilter(size_t size) : FunctionTable(size)
  {
    init(MIN_FILTER_VALUE,MAX_FILTER_VALUE);
  }

  float InvLogFilter::function(float x)
  {
    return log(x/MIN_FILTER_VALUE)/log(MAX_FILTER_VALUE/MIN_FILTER_VALUE);
  }

  InvLogFilter invLogFilter(256);
}
