#ifndef SYNTH_MUTEX_H
#define SYNTH_MUTEX_H

namespace Synth
{
  class Mutexable
  {
    public:
      Mutexable();
      boolean take();
      void give();
    private:
      SemaphoreHandle_t mutex;
      TickType_t delay;
  };
}

#endif
