#ifndef SYNTH_MATH_H
#define SYNTH_MATH_H

namespace Synth
{
  const float PI_FLOAT=3.141592f;
  const float DOUBLE_PI_FLOAT=2*PI_FLOAT;
  const float HALF_PI_FLOAT=0.5f*PI_FLOAT;

  const float DELTA=0.00001;

  class SampleRateDependant
  {
    public:
      SampleRateDependant(float sampleRate);
    protected:
      float sampleRate;
  };
  
  // Compute function each 'compute()' call
  class Function
  {
    public:
      virtual float compute(float x); // Compute the value of the function given x
    protected:
      virtual float function(float x)=0; // Floating point function
  };

  // Compute function within a table with interpolation
  class FunctionTable : public Function
  {
    public:
      FunctionTable(size_t tableSize); // create a blank table
      ~FunctionTable();

      void init(float xMin,float xMax); // init the table with x between xMin and xMax
      
      float compute(float x); // value interpolation given x
      float index(float index); // value interpolation given 

      size_t getSize();
      
    protected:
      virtual float function(float x)=0;
  
      float xMin,xMax,xInc;
      size_t size;
      float *table;
  };

  // Sine table
  class SineTable : public FunctionTable
  {
    public:
      SineTable(size_t size);
    protected:
      float function(float x);
  };
  
  extern SineTable sineTable;
  
  class SawTable : public FunctionTable
  {
    public:
      SawTable(size_t size);
    protected:
      float function(float x);
  };
  
  extern SawTable sawTable;

  class SquareTable : public FunctionTable
  {
    public:
      SquareTable(size_t size);
    protected:
      float function(float x);
  };
  
  extern SquareTable squareTable;

  class TriangleTable : public FunctionTable
  {
    public:
      TriangleTable(size_t size);
    protected:
      float function(float x);
  };
  
  extern TriangleTable triangleTable;
}


#endif
