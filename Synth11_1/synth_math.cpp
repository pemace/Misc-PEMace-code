#include <Arduino.h>
#include "math.h"
#include "synth.h"

namespace Synth
{
  SampleRateDependant::SampleRateDependant(float sampleRate)
  {
    this->sampleRate=sampleRate;
  }
  
  FunctionTable::FunctionTable(size_t tableSize)
  {
    size=tableSize;
    table=new float[size+1];

    xMin=0.f;
    xMax=0.f;
    xInc=1.f;
  }

  FunctionTable::~FunctionTable()
  {
    delete table;
  }

  void FunctionTable::init(float xMin,float xMax)
  {
    this->xMin=xMin;
    this->xMax=xMax;
    xInc=(xMax-xMin)/size;
    
    float x=xMin;
    for( int i;i<=size;i++,x+=xInc)
    {
      table[i]=function(x);
    }
  }

  float FunctionTable::compute(float x)
  {
    return index( (x - xMin)/xInc );
  }

  float FunctionTable::index(float index)
  {
    float iInf=floor(index);
    float dx=index-iInf;

    if(index>=size) return table[size];
    if(index<0.f) return table[0];

    uint32_t i=iInf;

    return table[i+1]*dx+table[i]*(1.f-dx);
  }

  size_t FunctionTable::getSize()
  {
    return size;
  }

  SineTable::SineTable(size_t size) : FunctionTable(size)
  {
    init(-PI_FLOAT,PI_FLOAT);
  }

  float SineTable::function(float x)
  {
    return sin(x);
  }

  SineTable sineTable(512);

  SawTable::SawTable(size_t size) : FunctionTable(size)
  {
    init(-PI_FLOAT,PI_FLOAT);
  }

  float SawTable::function(float x)
  {
    return x/PI_FLOAT;
  }

  SawTable sawTable(1);

  SquareTable::SquareTable(size_t size) : FunctionTable(size)
  {
    init(-PI_FLOAT,PI_FLOAT);
  }

  float SquareTable::function(float x)
  {
    return x>=1.f?1.f:-1.f;
  }

  SquareTable squareTable(128);

  TriangleTable::TriangleTable(size_t size) : FunctionTable(size)
  {
    init(-PI_FLOAT,PI_FLOAT);
  }

  float TriangleTable::function(float x)
  {
    if(x<-PI_FLOAT/2.f)
    {
      return -2.f-x/HALF_PI_FLOAT;
    } else if (x>PI_FLOAT/2.f)
    {
      return 2.f-x/HALF_PI_FLOAT;
    } else
    {
      return x/HALF_PI_FLOAT;
    }
  }

  TriangleTable triangleTable(4);
}
